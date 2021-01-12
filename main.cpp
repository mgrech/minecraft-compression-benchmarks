#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <regex>

#include <mio/mio.hpp>

#include "parser.hpp"

namespace fs = std::filesystem;

[[noreturn]]
void fatalError(char const* fmt, ...)
{
	va_list list;
	va_start(list, fmt);
	std::vfprintf(stderr, fmt, list);
	va_end(list);

	std::exit(EXIT_FAILURE);
}

std::regex const REGION_FILENAME_PATTERN(R"(([-]?\d+)\.([-]\d+)\.bin)");

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

void onRegion(Region const& region)
{
	// TODO: do something more interesting instead

	auto chunks = 0;
	auto sections = 0;

	for(auto& chunk : region.chunks)
	{
		if(!chunk)
			continue;

		++chunks;

		for(auto& section : chunk->sections)
		{
			if(!section)
				continue;

			++sections;
		}
	}

	std::printf("this region has %d chunks and %d sections\n", chunks, sections);
}

int main(int argc, char** argv)
{
	auto args = std::vector(argv, argv + argc);

	if(args.size() != 2)
		fatalError("invalid args, expected %s <region-dir>\n", args[0]);

	forEachRegionFile(args[1], [](fs::path const& path)
	{
		std::printf("parsing region file '%s' ...\n", path.filename().u8string().c_str());

		std::error_code errc;
		auto mapping = mio::make_mmap_source(path.string(), errc);

		if(errc)
			fatalError("failed to load region file '%s': %s\n", path.string().c_str(), errc.message().c_str());

		auto region = parseRegion((std::uint8_t const*)mapping.data());
		onRegion(region);
	});
}
