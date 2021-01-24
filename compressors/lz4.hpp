#pragma once

#include <cstddef>
#include <string>

#include <lz4.h>

class Lz4Compressor
{
	int _level;

public:
	explicit Lz4Compressor(int level)
		: _level(level)
	{}

	std::string name() const
	{
		return "lz4/" + std::to_string(_level);
	}

	std::size_t compress(void const* in, std::size_t inSize, void* out, std::size_t outSize)
	{
		auto size = LZ4_compress_fast((char const*)in, (char*)out, inSize, outSize, _level);

		if(size == 0)
		{
			std::fprintf(stderr, "lz4 compression failed\n");
			std::terminate();
		}

		return size;
	}
};
