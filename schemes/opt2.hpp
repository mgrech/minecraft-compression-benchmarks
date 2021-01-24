#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include <zstd.h>

#include "../bitpacking.hpp"

template <typename Compressor>
struct Opt2CompressionScheme
{
	Compressor _compressor;
	std::vector<std::uint8_t> _chunkBuffer;
	std::size_t _bufferUsed = 0;

	template <typename... P>
	explicit Opt2CompressionScheme(P&&... p)
	: _compressor(std::forward<P>(p)...)
	, _chunkBuffer(BLOCKS_PER_SECTION * SECTIONS_PER_CHUNK)
	{}

	std::string name() const
	{
		return "opt2:" + _compressor.name();
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
		// use a buffer bigger than necessary for better performance with some compression algorithms
		std::uint8_t compressedBuffer[8 * BLOCKS_PER_SECTION * SECTIONS_PER_CHUNK];
		auto size = _compressor.compress(_chunkBuffer.data(), _bufferUsed, compressedBuffer, sizeof compressedBuffer);
		_bufferUsed = 0;
		return size;
	}

	std::size_t section(std::uint16_t const* data)
	{
		auto palette = createPalette(data, BLOCKS_PER_SECTION);

		std::uint16_t buf[BLOCKS_PER_SECTION];
		palettize(palette, data, BLOCKS_PER_SECTION, buf);

		auto size = bitpackOptimized(palette.size, buf, BLOCKS_PER_SECTION, _chunkBuffer.data() + _bufferUsed);
		_bufferUsed += size;

		return 0;
	}
};
