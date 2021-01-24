#pragma once

#include <cstddef>
#include <cstdio>
#include <exception>
#include <string>

#include <zlib.h>

class ZlibCompressor
{
	int _level;

public:
	explicit ZlibCompressor(int level)
	: _level(level)
	{}

	std::string name() const
	{
		return "zlib/" + std::to_string(_level);
	}

	std::size_t compress(void const* in, std::size_t inSize, void* out, std::size_t outSize)
	{
		auto code = compress2((unsigned char*)out, &outSize, (unsigned char const*)in, inSize, _level);

		if(code != Z_OK)
		{
			std::fprintf(stderr, "zlib: compression failure\n");
			std::terminate();
		}

		return outSize;
	}
};
