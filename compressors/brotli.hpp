#pragma once

#include <cstddef>
#include <cstdio>
#include <exception>
#include <string>

#include <brotli/encode.h>

class BrotliCompressor
{
	int _level;

public:
	explicit BrotliCompressor(int level)
	: _level(level)
	{}

	std::string name() const
	{
		return "brotli/" + std::to_string(_level);
	}

	std::size_t compress(void const* in, std::size_t inSize, void* out, std::size_t outSize)
	{
		if(!BrotliEncoderCompress(_level, BROTLI_DEFAULT_WINDOW, BROTLI_MODE_GENERIC, inSize, (unsigned char const*)in, &outSize, (unsigned char*)out))
		{
			std::fprintf(stderr, "brotli compressor: compression failed\n");
			std::terminate();
		}

		return outSize;
	}
};
