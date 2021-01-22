#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>

struct Palette
{
	std::uint16_t size = 0;
	std::uint16_t values[256];
};

inline
Palette createPalette(std::uint16_t const* data, std::size_t count)
{
	Palette palette;

	auto begin = palette.values;
	auto end = begin;

	for(std::size_t i = 0; i != count;)
	{
		auto value = data[i];

		do ++i;
		while(i != count && data[i] == value);

		auto it = std::find(begin, end, value);

		if(it != end)
			continue;

		// in theory, we could get chunks with more than 256 distinct values, but not in test data for now
		assert(palette.size < 256);

		*end++ = value;
		++palette.size;
	}

	return palette;
}

inline
void palettize(Palette const& palette, std::uint16_t const* in, std::size_t count, std::uint16_t* out)
{
	auto begin = palette.values;
	auto end = begin + palette.size;

	for(std::size_t i = 0; i != count;)
	{
		auto value = in[i];

		auto it = std::find(begin, end, value);
		assert(it != end);

		auto index = it - begin;

		do out[i++] = index;
		while(i != count && in[i] == value);
	}
}
