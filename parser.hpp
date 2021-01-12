#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>

constexpr std::size_t BLOCKS_PER_SECTION = 16 * 16 * 16;
constexpr std::size_t SECTIONS_PER_CHUNK = 16;
constexpr std::size_t CHUNKS_PER_REGION = 32 * 32;

struct Chunk
{
	std::optional<std::uint16_t const*> sections[SECTIONS_PER_CHUNK];
};

struct Region
{
	std::optional<Chunk> chunks[CHUNKS_PER_REGION];
};

inline
Chunk parseChunk(std::uint16_t*& data)
{
	auto bitmask = *data++;
	Chunk chunk;

	for(auto i = 0; i != SECTIONS_PER_CHUNK; ++i)
	{
		if(bitmask & (1 << i))
		{
			chunk.sections[i] = data;
			data += BLOCKS_PER_SECTION;
		}
	}

	return chunk;
}

inline
Region parseRegion(std::uint8_t const* data)
{
	Region region;

	// skip chunk bitmap
	auto chunkPtr = data + CHUNKS_PER_REGION / 8;

	for(auto i = 0; i != CHUNKS_PER_REGION; ++i)
	{
		if(data[i / 8] & (1 << (i % 8)))
			region.chunks[i] = parseChunk(*(std::uint16_t**)&chunkPtr);
	}

	return region;
}
