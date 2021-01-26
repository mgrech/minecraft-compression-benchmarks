#include <cstdint>

#include <gtest/gtest.h>

#include "../bitpacking.hpp"

TEST(bitpacking, pack16to1)
{
	std::uint16_t in[8] = {0, 1, 0, 1, 0, 1, 0, 1};
	std::uint8_t buf[] = {0, 0xff};
	auto size = bitpack16to1(in, sizeof in / sizeof *in, buf);
	ASSERT_EQ(size, 1);
	ASSERT_EQ(buf[0], 0b10101010);
	ASSERT_EQ(buf[1], 0xff);
}

TEST(bitpacking, pack16to2)
{
	std::uint16_t in[4] = {0b00, 0b01, 0b10, 0b11};
	std::uint8_t buf[] = {0, 0xff};
	auto size = bitpack16to2(in, sizeof in / sizeof *in, buf);
	ASSERT_EQ(size, 1);
	ASSERT_EQ(buf[0], 0b11'10'01'00);
	ASSERT_EQ(buf[1], 0xff);
}

TEST(bitpacking, pack16to3_single)
{
	std::uint16_t in[4] = {0b010, 0b101, 0b010, 0b101};
	std::uint64_t buf[] = {0, 0xffff'ffff'ffff'ffff};
	auto size = bitpack16to3(in, sizeof in / sizeof *in, (std::uint8_t*)buf);
	ASSERT_EQ(size, 8);
	ASSERT_EQ(buf[0], 0b101'010'101'010);
	ASSERT_EQ(buf[1], 0xffff'ffff'ffff'ffff);
}

TEST(bitpacking, pack16to3_multiple)
{
	std::uint16_t in[22];

	for(auto& elem : in)
		elem = 0b110;

	std::uint64_t buf[] = {0, 0, 0xffff'ffff'ffff'ffff};
	auto size = bitpack16to3(in, sizeof in / sizeof *in, (std::uint8_t*)buf);
	ASSERT_EQ(size, 16);
	ASSERT_EQ(buf[0], 0b0'110'110'110'110'110'110'110'110'110'110'110'110'110'110'110'110'110'110'110'110'110);
	ASSERT_EQ(buf[1], 0b110);
	ASSERT_EQ(buf[2], 0xffff'ffff'ffff'ffff);
}

TEST(bitpacking, pack16to4)
{
	std::uint16_t in[4] = {0b1010, 0b1010, 0b0101, 0b0101};
	std::uint8_t buf[] = {0, 0, 0xff};
	auto size = bitpack16to4(in, sizeof in / sizeof *in, buf);
	ASSERT_EQ(size, 2);
	ASSERT_EQ(buf[0], 0b1010'1010);
	ASSERT_EQ(buf[1], 0b0101'0101);
	ASSERT_EQ(buf[2], 0xff);
}

TEST(bitpacking, pack16to5_single)
{
	std::uint16_t in[2] = {0b10101, 0b11111};
	std::uint64_t buf[] = {0, 0xffff'ffff'ffff'ffff};
	auto size = bitpack16to5(in, sizeof in / sizeof *in, (std::uint8_t*)buf);
	ASSERT_EQ(size, 8);
	ASSERT_EQ(buf[0], 0b11111'10101);
	ASSERT_EQ(buf[1], 0xffff'ffff'ffff'ffff);
}

TEST(bitpacking, pack16to5_multiple)
{
	std::uint16_t in[13];

	for(auto& elem : in)
		elem = 0b11011;

	std::uint64_t buf[] = {0, 0, 0xffff'ffff'ffff'ffff};
	auto size = bitpack16to5(in, sizeof in / sizeof *in, (std::uint8_t*)buf);
	ASSERT_EQ(size, 16);
	ASSERT_EQ(buf[0], 0b0'11011'11011'11011'11011'11011'11011'11011'11011'11011'11011'11011'11011);
	ASSERT_EQ(buf[1], 0b11011);
	ASSERT_EQ(buf[2], 0xffff'ffff'ffff'ffff);
}

TEST(bitpacking, pack16to6_single)
{
	std::uint16_t in[2] = {0b101010, 0b111000};
	std::uint64_t buf[] = {0, 0xffff'ffff'ffff'ffff};
	auto size = bitpack16to6(in, sizeof in / sizeof *in, (std::uint8_t*)buf);
	ASSERT_EQ(size, 8);
	ASSERT_EQ(buf[0], 0b111000'101010);
	ASSERT_EQ(buf[1], 0xffff'ffff'ffff'ffff);
}

TEST(bitpacking, pack16to6_multiple)
{
	std::uint16_t in[11];

	for(auto& elem : in)
		elem = 0b111000;

	std::uint64_t buf[] = {0, 0, 0xffff'ffff'ffff'ffff};
	auto size = bitpack16to6(in, sizeof in / sizeof *in, (std::uint8_t*)buf);
	ASSERT_EQ(size, 16);
	ASSERT_EQ(buf[0], 0b0000'111000'111000'111000'111000'111000'111000'111000'111000'111000'111000);
	ASSERT_EQ(buf[1], 0b111000);
	ASSERT_EQ(buf[2], 0xffff'ffff'ffff'ffff);
}

TEST(bitpacking, pack16to7_single)
{
	std::uint16_t in[2] = {0b1111111, 0b1010101};
	std::uint64_t buf[] = {0, 0xffff'ffff'ffff'ffff};
	auto size = bitpack16to7(in, sizeof in / sizeof *in, (std::uint8_t*)buf);
	ASSERT_EQ(size, 8);
	ASSERT_EQ(buf[0], 0b1010101'1111111);
	ASSERT_EQ(buf[1], 0xffff'ffff'ffff'ffff);
}

TEST(bitpacking, pack16to7_multiple)
{
	std::uint16_t in[10];

	for(auto& elem : in)
		elem = 0b1111000;

	std::uint64_t buf[] = {0, 0, 0xffff'ffff'ffff'ffff};
	auto size = bitpack16to7(in, sizeof in / sizeof *in, (std::uint8_t*)buf);
	ASSERT_EQ(size, 16);
	ASSERT_EQ(buf[0], 0b0'1111000'1111000'1111000'1111000'1111000'1111000'1111000'1111000'1111000);
	ASSERT_EQ(buf[1], 0b1111000);
	ASSERT_EQ(buf[2], 0xffff'ffff'ffff'ffff);
}

TEST(bitpacking, pack16to8)
{
	std::uint16_t in[2] = {0xaa, 0xbb};
	std::uint8_t buf[] = {0, 0, 0xff};
	auto size = bitpack16to8(in, sizeof in / sizeof *in, buf);
	ASSERT_EQ(size, 2);
	ASSERT_EQ(buf[0], 0xaa);
	ASSERT_EQ(buf[1], 0xbb);
	ASSERT_EQ(buf[2], 0xff);
}
