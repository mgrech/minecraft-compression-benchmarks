#pragma once

#include <cstddef>
#include <string>

#include <zstd.h>

class ZstdCompressor
{
	ZSTD_CCtx* _ctx;
	int _level;

public:
	explicit ZstdCompressor(int level)
	: _ctx(ZSTD_createCCtx())
	, _level(level)
	{}

	~ZstdCompressor()
	{
		ZSTD_freeCCtx(_ctx);
	}

	std::string name() const
	{
		return "zstd/" + std::to_string(_level);
	}

	std::size_t compress(void const* in, std::size_t inSize, void* out, std::size_t outSize)
	{
		return ZSTD_compressCCtx(_ctx, out, outSize, in, inSize, _level);
	}
};
