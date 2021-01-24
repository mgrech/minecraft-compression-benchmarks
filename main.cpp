#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <filesystem>
#include <regex>
#include <utility>

#include <mio/mio.hpp>

#include "compressors/null.hpp"
#include "compressors/brotli.hpp"
#include "compressors/bzip2.hpp"
#include "compressors/libdeflate.hpp"
#include "compressors/lz4.hpp"
#include "compressors/zlib.hpp"
#include "compressors/zstd.hpp"
#include "parser.hpp"
#include "schemes/vanilla.hpp"
#include "schemes/opt1.hpp"
#include "schemes/opt2.hpp"

namespace fs = std::filesystem;

std::regex const REGION_FILENAME_PATTERN(R"(([-]?\d+)\.([-]?\d+)\.bin)");

[[noreturn]]
inline
void fatalError(char const* fmt, ...)
{
	va_list list;
	va_start(list, fmt);
	std::vfprintf(stderr, fmt, list);
	va_end(list);

	std::exit(EXIT_FAILURE);
}

template <typename FileHandler>
void forEachRegionFile(fs::path const& directory, FileHandler handler)
{
	for(auto&& entry : fs::directory_iterator(directory))
	{
		if(!entry.is_regular_file())
			continue;

		auto filename = entry.path().filename().u8string();
		std::smatch match;

		if(std::regex_match(filename, match, REGION_FILENAME_PATTERN))
			handler(entry.path());
	}
}

void stats(std::vector<Region> const& regions)
{
	std::size_t chunkCount = 0;
	std::size_t sectionCount = 0;
	std::size_t sectionBitDepthCounts[16] = {};
	std::size_t size = 0;

	for(auto& region : regions)
	{
		for(auto& chunk : region.chunks)
		{
			if(!chunk)
				continue;

			++chunkCount;

			for(auto& section : chunk->sections)
			{
				if(!section)
					continue;

				++sectionCount;
				size += sizeof **section * BLOCKS_PER_SECTION;

				auto palette = createPalette(*section, BLOCKS_PER_SECTION);
				auto bits = ceillog2(palette.size);
				++sectionBitDepthCounts[bits];
			}
		}
	}

	std::printf("size: %.2f GiB\n", size / 1024.f / 1024.f / 1024.f);
	std::printf("regions: %zu\n", regions.size());
	std::printf("chunks: %zu\n", chunkCount);
	std::printf("sections: %zu\n", sectionCount);
	std::printf("bits per section:\n");

	for(auto i = 0; i != 16; ++i)
	{
		if(sectionBitDepthCounts[i])
			std::printf("\t%d: %zu\n", i, sectionBitDepthCounts[i]);
	}

	std::printf("\n");
}

template <typename Scheme>
void benchmark(std::vector<Region> const& regions, Scheme scheme)
{
	auto startTime = std::chrono::high_resolution_clock::now();

	std::size_t size = 0;

	for(auto& region : regions)
	{
		scheme.beginRegion(region);

		for(auto& chunk : region.chunks)
		{
			if(!chunk)
				continue;

			scheme.beginChunk(*chunk);

			for(auto& section : chunk->sections)
			{
				if(!section)
					continue;

				size += scheme.section(*section);
			}

			size += scheme.endChunk();
		}

		size += scheme.endRegion();
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() / 1000.f;

	std::printf("scheme: %s\n", scheme.name().c_str());
	std::printf("size: %.2f MiB\n", size / 1024.f / 1024.f);
	std::printf("time: %.2f s\n", duration);
	std::printf("\n");
}

int main(int argc, char** argv)
{
	auto args = std::vector(argv, argv + argc);

	if(args.size() != 2)
		fatalError("invalid args, expected %s <region-dir>\n", args[0]);

	std::vector<mio::mmap_source> mappings;
	std::vector<Region> regions;

	forEachRegionFile(args[1], [&mappings, &regions](fs::path const& path)
	{
		std::printf("loading region file '%s' ...\n", path.filename().u8string().c_str());

		std::error_code errc;
		auto mapping = mio::make_mmap_source(path.string(), errc);

		if(errc)
			fatalError("failed to load region file '%s': %s\n", path.string().c_str(), errc.message().c_str());

		auto region = parseRegion((std::uint8_t const*)mapping.data());
		mappings.emplace_back(std::move(mapping));
		regions.emplace_back(region);
	});

	std::printf("done loading regions\n");
	std::printf("\n");

	stats(regions);
	benchmark(regions, VanillaCompressionScheme());
	benchmark(regions, Opt1CompressionScheme());

	benchmark(regions, Opt2CompressionScheme<NullCompressor>());

	//for(int i = 1; i <= 250; i += 10)
	//	benchmark(regions, Opt2CompressionScheme<Bzip2Compressor>(i));

	for(int i = 0; i <= 8; ++i)
		benchmark(regions, Opt2CompressionScheme<BrotliCompressor>(i));

	for(int i = 1; i <= 8; ++i)
		benchmark(regions, Opt2CompressionScheme<ZlibCompressor>(i));

	for(int i = 1; i <= 9; ++i)
		benchmark(regions, Opt2CompressionScheme<LibDeflateCompressor>(i));

	for(int i = 0; i <= 12; ++i)
		benchmark(regions, Opt2CompressionScheme<ZstdCompressor>(i));

	benchmark(regions, Opt2CompressionScheme<Lz4Compressor>(0));
}
