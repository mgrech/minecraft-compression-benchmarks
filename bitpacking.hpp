#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>

inline
std::size_t bitpack16to8(std::uint16_t const* in, std::size_t count, std::uint8_t* out)
{
	for(std::size_t i = 0; i != count; ++i)
		out[i] = (std::uint8_t)in[i];

	return count;
}

inline
std::size_t bitpack16to7(std::uint16_t const* in, std::size_t count, std::uint8_t* out)
{
	// take 9 7-bit values at a time, packing them into a 64-bit value and leaving 1 bit empty

	auto loopCount = count / 9;
	auto remainingCount = count % 9;

	for(std::size_t i = 0; i != loopCount; ++i)
	{
		auto block = in + 12 * i;
		auto i0 = (std::uint64_t)block[0];
		auto i1 = (std::uint64_t)block[1];
		auto i2 = (std::uint64_t)block[2];
		auto i3 = (std::uint64_t)block[3];
		auto i4 = (std::uint64_t)block[4];
		auto i5 = (std::uint64_t)block[5];
		auto i6 = (std::uint64_t)block[6];
		auto i7 = (std::uint64_t)block[7];
		auto i8 = (std::uint64_t)block[8];

		auto next = (i0 <<  0) | (i1 <<  7) | (i2  << 14) | (i3  << 21)
		          | (i4 << 28) | (i5 << 35) | (i6  << 42) | (i7  << 49)
		          | (i8 << 56);

		std::memcpy(out, &next, sizeof next);
	}

	if(remainingCount == 0)
		return loopCount * 8;

	std::uint64_t final = 0;

	for(std::size_t i = 0; i != remainingCount; ++i)
		final |= (std::uint64_t)in[i] << (i * 7);

	std::memcpy(out, &final, sizeof final);
	return loopCount * 8 + sizeof final;
}

inline
std::size_t bitpack16to6(std::uint16_t const* in, std::size_t count, std::uint8_t* out)
{
	// take 10 6-bit values at a time, packing them into a 64-bit value and leaving 4 bits empty

	auto loopCount = count / 10;
	auto remainingCount = count % 10;

	for(std::size_t i = 0; i != loopCount; ++i)
	{
		auto block = in + 12 * i;
		auto i0 = (std::uint64_t)block[0];
		auto i1 = (std::uint64_t)block[1];
		auto i2 = (std::uint64_t)block[2];
		auto i3 = (std::uint64_t)block[3];
		auto i4 = (std::uint64_t)block[4];
		auto i5 = (std::uint64_t)block[5];
		auto i6 = (std::uint64_t)block[6];
		auto i7 = (std::uint64_t)block[7];
		auto i8 = (std::uint64_t)block[8];
		auto i9 = (std::uint64_t)block[9];

		auto next = (i0 <<  0) | (i1 <<  6) | (i2  << 12) | (i3  << 18)
		          | (i4 << 24) | (i5 << 30) | (i6  << 36) | (i7  << 42)
		          | (i8 << 48) | (i9 << 54);

		std::memcpy(out, &next, sizeof next);
	}

	if(remainingCount == 0)
		return loopCount * 8;

	std::uint64_t final = 0;

	for(std::size_t i = 0; i != remainingCount; ++i)
		final |= (std::uint64_t)in[i] << (i * 6);

	std::memcpy(out, &final, sizeof final);
	return loopCount * 8 + sizeof final;
}

inline
std::size_t bitpack16to5(std::uint16_t const* in, std::size_t count, std::uint8_t* out)
{
	// take 12 5-bit values at a time, packing them into a 64-bit value and leaving 4 bits empty

	auto loopCount = count / 12;
	auto remainingCount = count % 12;

	for(std::size_t i = 0; i != loopCount; ++i)
	{
		auto block = in + 12 * i;
		auto i0 = (std::uint64_t)block[0];
		auto i1 = (std::uint64_t)block[1];
		auto i2 = (std::uint64_t)block[2];
		auto i3 = (std::uint64_t)block[3];
		auto i4 = (std::uint64_t)block[4];
		auto i5 = (std::uint64_t)block[5];
		auto i6 = (std::uint64_t)block[6];
		auto i7 = (std::uint64_t)block[7];
		auto i8 = (std::uint64_t)block[8];
		auto i9 = (std::uint64_t)block[9];
		auto i10 = (std::uint64_t)block[10];
		auto i11 = (std::uint64_t)block[11];

		auto next = (i0 <<  0) | (i1 <<  5) | (i2  << 10) | (i3  << 15)
		          | (i4 << 20) | (i5 << 25) | (i6  << 30) | (i7  << 35)
		          | (i8 << 40) | (i9 << 45) | (i10 << 50) | (i11 << 55);

		std::memcpy(out, &next, sizeof next);
	}

	if(remainingCount == 0)
		return loopCount * 8;

	std::uint64_t final = 0;

	for(std::size_t i = 0; i != remainingCount; ++i)
		final |= (std::uint64_t)in[i] << (i * 5);

	std::memcpy(out, &final, sizeof final);
	return loopCount * 8 + sizeof final;
}

inline
std::size_t bitpack16to4(std::uint16_t const* in, std::size_t count, std::uint8_t* out)
{
	assert(count % 2 == 0);
	count /= 2;

	for(std::size_t i = 0; i != count; ++i)
	{
		auto i0 = (std::uint8_t)in[2 * i];
		auto i1 = (std::uint8_t)in[2 * i + 1];
		out[i] = i0 | (i1 << 4);
	}

	return count;
}

inline
std::size_t bitpack16to3(std::uint16_t const* in, std::size_t count, std::uint8_t* out)
{
	// take 21 3-bit values at a time, packing them into a 64-bit value and leaving 1 bit empty

	auto loopCount = count / 21;
	auto remainingCount = count % 21;

	for(std::size_t i = 0; i != loopCount; ++i)
	{
		auto block = in + 21 * i;
		auto i0 = (std::uint64_t)block[0];
		auto i1 = (std::uint64_t)block[1];
		auto i2 = (std::uint64_t)block[2];
		auto i3 = (std::uint64_t)block[3];
		auto i4 = (std::uint64_t)block[4];
		auto i5 = (std::uint64_t)block[5];
		auto i6 = (std::uint64_t)block[6];
		auto i7 = (std::uint64_t)block[7];
		auto i8 = (std::uint64_t)block[8];
		auto i9 = (std::uint64_t)block[9];
		auto i10 = (std::uint64_t)block[10];
		auto i11 = (std::uint64_t)block[11];
		auto i12 = (std::uint64_t)block[12];
		auto i13 = (std::uint64_t)block[13];
		auto i14 = (std::uint64_t)block[14];
		auto i15 = (std::uint64_t)block[15];
		auto i16 = (std::uint64_t)block[16];
		auto i17 = (std::uint64_t)block[17];
		auto i18 = (std::uint64_t)block[18];
		auto i19 = (std::uint64_t)block[19];
		auto i20 = (std::uint64_t)block[20];

		auto next = (i0  <<  0) | (i1  <<  3) | (i2  <<  6) | (i3  <<  9)
			  | (i4  << 12) | (i5  << 15) | (i6  << 18) | (i7  << 21)
			  | (i8  << 24) | (i9  << 27) | (i10 << 30) | (i11 << 33)
			  | (i12 << 36) | (i13 << 39) | (i14 << 42) | (i15 << 45)
			  | (i16 << 48) | (i17 << 51) | (i18 << 54) | (i19 << 57)
			  | (i20 << 60);

		std::memcpy(out, &next, sizeof next);
	}

	if(remainingCount == 0)
		return loopCount * 8;

	std::uint64_t final = 0;

	for(std::size_t i = 0; i != remainingCount; ++i)
		final |= (std::uint64_t)in[i] << (i * 3);

	std::memcpy(out, &final, sizeof final);
	return loopCount * 8  + sizeof final;
}

inline
std::size_t bitpack16to2(std::uint16_t const* in, std::size_t count, std::uint8_t* out)
{
	assert(count % 4 == 0);
	count /= 4;

	for(std::size_t i = 0; i != count; ++i)
	{
		auto i0 = (std::uint8_t)in[2 * i];
		auto i1 = (std::uint8_t)in[2 * i + 1];
		auto i2 = (std::uint8_t)in[2 * i + 2];
		auto i3 = (std::uint8_t)in[2 * i + 3];
		out[i] = i0 | (i1 << 2) | (i2 << 4) | (i3 << 6);
	}

	return count;
}

inline
std::size_t bitpack16to1(std::uint16_t const* in, std::size_t count, std::uint8_t* out)
{
	assert(count % 8 == 0);
	count /= 8;

	for(std::size_t i = 0; i != count; ++i)
	{
		auto i0 = (std::uint8_t)in[2 * i];
		auto i1 = (std::uint8_t)in[2 * i + 1];
		auto i2 = (std::uint8_t)in[2 * i + 2];
		auto i3 = (std::uint8_t)in[2 * i + 3];
		auto i4 = (std::uint8_t)in[2 * i + 4];
		auto i5 = (std::uint8_t)in[2 * i + 5];
		auto i6 = (std::uint8_t)in[2 * i + 6];
		auto i7 = (std::uint8_t)in[2 * i + 7];
		out[i] = i0 | (i1 << 1) | (i2 << 2) | (i3 << 3) | (i4 << 4) | (i5 << 5) | (i6 << 6) | (i7 << 7);
	}

	return count;
}

inline
int ceillog2(std::size_t x)
{
	assert(x != 0);
	return sizeof x * 8 - 1 - __builtin_clzll(2 * x - 1);
}

inline
std::size_t bitpackVanilla(std::size_t distincts, std::uint16_t const* in, std::size_t count, std::uint8_t* out)
{
	switch(ceillog2(distincts))
	{
	case 0: case 1: case 2: case 3: case 4:
		return bitpack16to4(in, count, out);

	case 5: return bitpack16to5(in, count, out);
	case 6: return bitpack16to6(in, count, out);
	case 7: return bitpack16to7(in, count, out);
	case 8: return bitpack16to8(in, count, out);

	default: break;
	}

	// this should not happen with test data
	assert(false);
	__builtin_unreachable();
}

inline
std::size_t bitpackOptimized(std::size_t distincts, std::uint16_t const* in, std::size_t count, std::uint8_t* out)
{
	switch(ceillog2(distincts))
	{
	// if there is only a single distinct value, we don't need to store anything
	case 0: return 0;

	case 1: return bitpack16to1(in, count, out);
	case 2: return bitpack16to2(in, count, out);
	case 3: return bitpack16to3(in, count, out);
	case 4: return bitpack16to4(in, count, out);
	case 5: return bitpack16to5(in, count, out);
	case 6: return bitpack16to6(in, count, out);
	case 7: return bitpack16to7(in, count, out);
	case 8: return bitpack16to8(in, count, out);

	default: break;
	}

	// this should not happen with test data
	assert(false);
	__builtin_unreachable();
}
