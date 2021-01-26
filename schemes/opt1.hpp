#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>

#include "../bitpacking.hpp"
#include "../palette.hpp"
#include "../compressors/zlib.hpp"

struct Opt1CompressionScheme
{
	ZlibCompressor _compressor;
	std::vector<std::uint8_t> _chunkBuffer;
	std::size_t _bufferUsed = 0;

	Opt1CompressionScheme()
	: _compressor(-1)
	, _chunkBuffer(BLOCKS_PER_SECTION * SECTIONS_PER_CHUNK)
	{}

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
		std::uint8_t compressedBuffer[8 * BLOCKS_PER_SECTION * SECTIONS_PER_CHUNK];
		auto size = _compressor.compress(_chunkBuffer.data(), _bufferUsed, compressedBuffer, sizeof compressedBuffer);
		_bufferUsed = 0;
		return size;
	}

	std::size_t section(std::uint16_t const* data)
	{
		auto palette = createPalette(data, BLOCKS_PER_SECTION, false);

		std::uint16_t buf[BLOCKS_PER_SECTION];
		palettize(palette, data, BLOCKS_PER_SECTION, buf, false);

		auto size = bitpackOptimized(palette.size, buf, BLOCKS_PER_SECTION, _chunkBuffer.data() + _bufferUsed);
		_bufferUsed += size;

		return 0;
	}
};
