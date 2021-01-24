#pragma once

#include <cstddef>
#include <cstring>
#include <exception>

struct NullCompressor
{
	std::string name() const
	{
		return "null";
	}

	std::size_t compress(void const* in, std::size_t inSize, void* out, std::size_t outSize)
	{
		if(outSize < inSize)
		{
			std::fprintf(stderr, "null compressor: not enough buffer space\n");
			std::terminate();
		}

		std::memcpy(out, in, inSize);
		return inSize;
	}
};
