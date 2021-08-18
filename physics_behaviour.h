#pragma once
#include <vector>

enum Tiles {
	EMPTY,
	SAND,
	BEDROCK,
	WATER,
	STONE
};


class dust {
public:
	static int update(std::vector<std::vector<uint16_t>>& particles, std::vector<std::vector<uint16_t>>& particlesCopy, int x, int y, Tiles type);
};