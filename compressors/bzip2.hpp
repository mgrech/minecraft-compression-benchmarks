#pragma once

#include <cstddef>
#include <string>

#include <bzlib.h>

class Bzip2Compressor
{
	int _level;

public:
	explicit Bzip2Compressor(int level)
	: _level(level)
	{}

	std::string name() const
	{
		return "bzip2/" + std::to_string(_level);
	}

	std::size_t compress(void const* in, std::size_t inSize, void* out, std::size_t outSize)
	{
		unsigned outSize2 = outSize;

		if(BZ2_bzBuffToBuffCompress((char*)out, &outSize2, (char*)in, inSize, 9, 0, _level) != BZ_OK)
		{
			std::fprintf(stderr, "bzip2 compression failed\n");
			std::terminate();
		}

		return outSize2;
	}
};
