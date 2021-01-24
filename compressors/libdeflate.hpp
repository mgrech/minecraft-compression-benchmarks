#pragma once

#include <cstddef>
#include <string>

#include <libdeflate.h>

class LibDeflateCompressor
{
	libdeflate_compressor* _compressor;
	int _level;

public:
	explicit LibDeflateCompressor(int level)
	: _compressor(libdeflate_alloc_compressor(level))
	, _level(level)
	{}

	std::string name() const
	{
		return "libdeflate/" + std::to_string(_level);
	}

	std::size_t compress(void const* in, std::size_t inSize, void* out, std::size_t outSize)
	{
		return libdeflate_zlib_compress(_compressor, in, inSize, out, outSize);
	}
};
