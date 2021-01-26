#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>

#include <immintrin.h>

struct Palette
{
	std::uint16_t size = 0;
	std::uint16_t values[256];

	Palette()
	{
		for(auto& value : values)
			value = 0xffff;
	}
};

inline
bool tryCreatePaletteVectorized(std::uint16_t const* data, std::size_t count, Palette* out)
{
	std::size_t paletteSize = 0;
	__m256i palette1 = _mm256_set1_epi16(-1);
	__m256i palette2 = _mm256_set1_epi16(-1);
	__m256i palette3 = _mm256_set1_epi16(-1);

	for(std::size_t i = 0; i != count;)
	{
		auto value = data[i];

		do ++i;
		while(i != count && data[i] == value);

		auto next = _mm256_set1_epi16(value);
		auto mask1 = _mm256_movemask_epi8(_mm256_cmpeq_epi16(palette1, next));
		auto mask2 = _mm256_movemask_epi8(_mm256_cmpeq_epi16(palette2, next));
		auto mask3 = _mm256_movemask_epi8(_mm256_cmpeq_epi16(palette3, next));
		auto mask = mask1 | mask2 | mask3;

		if(mask != 0)
			// value already in palette
			continue;

		if(paletteSize == 48)
		{
			// palette registers full, fall back to non-vector version
			_mm256_storeu_si256((__m256i*)out->values, palette1);
			_mm256_storeu_si256((__m256i*)(out->values + 16), palette2);
			_mm256_storeu_si256((__m256i*)(out->values + 32), palette3);
			out->size = paletteSize;
			return false;
		}

		// store palette entry at current index via memory roundtrip
		alignas(__m256i) std::uint16_t buf[48];
		_mm256_store_si256((__m256i*)buf, palette1);
		_mm256_store_si256((__m256i*)(buf + 16), palette2);
		_mm256_store_si256((__m256i*)(buf + 32), palette3);
		buf[paletteSize++] = value;
		palette1 = _mm256_load_si256((__m256i*)buf);
		palette2 = _mm256_load_si256((__m256i*)(buf + 16));
		palette3 = _mm256_load_si256((__m256i*)(buf + 32));
	}

	_mm256_storeu_si256((__m256i*)out->values, palette1);
	_mm256_storeu_si256((__m256i*)(out->values + 16), palette2);
	_mm256_storeu_si256((__m256i*)(out->values + 32), palette3);
	out->size = paletteSize;
	return true;
}

inline
Palette createPalette(std::uint16_t const* data, std::size_t count, bool vectorized)
{
	Palette palette;

	if(vectorized && tryCreatePaletteVectorized(data, count, &palette))
		return palette;

	auto p = palette.values;
	auto size = palette.size;

	for(std::size_t i = 0; i != count;)
	{
		auto value = data[i];

		do ++i;
		while(i != count && data[i] == value);

		auto it = std::find(p, p + size, value);

		if(it != p + size)
			continue;

		// in theory, we could get chunks with more than 256 distinct values, but not in test data for now
		assert(size < 256);

		p[size++] = value;
	}

	palette.size = size;
	return palette;
}

inline
void palettizeVectorized(Palette const& palette, std::uint16_t const* in, std::size_t count, std::uint16_t* out)
{
	auto p = (__m256i*)palette.values;
	__m256i palette0 = _mm256_loadu_si256(p);
	__m256i palette1 = _mm256_loadu_si256(p + 1);
	__m256i palette2 = _mm256_loadu_si256(p + 2);
	__m256i palette3 = _mm256_loadu_si256(p + 3);

	for(std::size_t i = 0; i != count;)
	{
		auto value = in[i];

		auto next = _mm256_set1_epi16(value);
		auto mask0 = _mm256_movemask_epi8(_mm256_cmpeq_epi16(palette0, next));
		auto mask1 = _mm256_movemask_epi8(_mm256_cmpeq_epi16(palette1, next));
		auto mask2 = _mm256_movemask_epi8(_mm256_cmpeq_epi16(palette2, next));
		auto mask3 = _mm256_movemask_epi8(_mm256_cmpeq_epi16(palette3, next));

		auto idx0 = mask0 == 0 ? 0 : __builtin_ctz(mask0) / 2;
		auto idx1 = mask1 == 0 ? 0 : __builtin_ctz(mask1) / 2 + 16;
		auto idx2 = mask2 == 0 ? 0 : __builtin_ctz(mask2) / 2 + 32;
		auto idx3 = mask3 == 0 ? 0 : __builtin_ctz(mask3) / 2 + 48;

		std::uint16_t index = idx0 | idx1 | idx2 | idx3;

		do out[i++] = index;
		while(i != count && in[i] == value);
	}
}

inline
void palettize(Palette const& palette, std::uint16_t const* in, std::size_t count, std::uint16_t* out, bool vectorize)
{
	if(vectorize && palette.size <= 64)
		return palettizeVectorized(palette, in, count, out);

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
