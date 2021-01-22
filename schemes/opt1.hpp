#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>

#include <libdeflate.h>

#include "../bitpacking.hpp"
#include "../palette.hpp"

struct Opt1CompressionScheme
{
	libdeflate_compressor* _compressor;
	std::vector<std::uint8_t> _chunkBuffer;

	Opt1CompressionScheme()
	: _compressor(libdeflate_alloc_compressor(6))
	{
		_chunkBuffer.reserve(BLOCKS_PER_SECTION * SECTIONS_PER_CHUNK);
	}

	std::string name() const
	{
		return "opt1";
	}

	void beginRegion(Region const& region)
	{
	}

	std::size_t endRegion()
	{
		return 0;
	}

	void beginChunk(Chunk const& chunk)
	{
	}

	std::size_t endChunk()
	{
		std::uint8_t compressedBuffer[BLOCKS_PER_SECTION * SECTIONS_PER_CHUNK];
		auto size = libdeflate_zlib_compress(_compressor, _chunkBuffer.data(), _chunkBuffer.size(), compressedBuffer, sizeof compressedBuffer);
		_chunkBuffer.clear();
		return size;
	}

	std::size_t section(std::uint16_t const* data)
	{
		auto palette = createPalette(data, BLOCKS_PER_SECTION);

		std::uint16_t buf[BLOCKS_PER_SECTION];
		palettize(palette, data, BLOCKS_PER_SECTION, buf);

		auto buf2 = (std::uint8_t*)buf;
		auto packedSize = bitpackOptimized(palette.size, buf, BLOCKS_PER_SECTION, buf2);
		_chunkBuffer.insert(_chunkBuffer.end(), buf2, buf2 + packedSize);

		return 0;
	}
};
