#pragma once
#include <vector>

enum Tiles {
	EMPTY,
	SAND,
	BEDROCK
};


class dust {
public:
	static void update(std::vector<std::vector<uint16_t>> particles, std::vector<std::vector<uint16_t>> particlesCopy, int x, int y, Tiles type);
};