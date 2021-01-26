#include <cstdint>

#include <gtest/gtest.h>

#include "../palette.hpp"

void testCreatePalette(bool vectorize)
{
	std::uint16_t data[] = {0, 1, 3, 3, 3, 2, 0xffff};
	auto palette = createPalette(data, sizeof data / sizeof *data - 1, vectorize);
	ASSERT_EQ(palette.size, 4);
	ASSERT_EQ(palette.values[0], 0);
	ASSERT_EQ(palette.values[1], 1);
	ASSERT_EQ(palette.values[2], 3);
	ASSERT_EQ(palette.values[3], 2);
}

TEST(palettization, createPalette_slow)
{
	testCreatePalette(false);
}

TEST(palettization, createPalette_fast)
{
	testCreatePalette(true);
}

void testPalettize(bool vectorize)
{
	std::uint16_t data[] = {1, 1, 7, 2, 7, 7, 0, 1, 0xff};

	Palette palette;
	palette.size = 4;
	palette.values[0] = 1;
	palette.values[1] = 7;
	palette.values[2] = 2;
	palette.values[3] = 0;

	constexpr auto count = sizeof data / sizeof *data - 1;
	std::uint16_t buf[count + 1];
	buf[count] = 0xff;
	palettize(palette, data, count, buf, vectorize);

	std::uint16_t expected[] = {0, 0, 1, 2, 1, 1, 3, 0};

	ASSERT_EQ(buf[0], expected[0]);
	ASSERT_EQ(buf[1], expected[1]);
	ASSERT_EQ(buf[2], expected[2]);
	ASSERT_EQ(buf[3], expected[3]);
	ASSERT_EQ(buf[4], expected[4]);
	ASSERT_EQ(buf[5], expected[5]);
	ASSERT_EQ(buf[6], expected[6]);
	ASSERT_EQ(buf[7], expected[7]);
	ASSERT_EQ(buf[count], 0xff);
}

TEST(palettization, palettize_slow)
{
	testPalettize(false);
}

TEST(palettization, palettize_fast)
{
	testPalettize(true);
}
