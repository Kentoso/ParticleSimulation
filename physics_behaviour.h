#pragma once
#include <vector>

enum Tiles {
	EMPTY,
	SAND,
	BEDROCK,
	WATER,
	STONE,
	ACID,
	PLANT
};


class dust {
public:
	static int update(std::vector<std::vector<uint16_t>>& particles, std::vector<std::vector<uint16_t>>& particlesCopy, int x, int y, Tiles type);
};

class liquid {
public:
	static void horizontalMovement(std::vector<std::vector<uint16_t>>& particles, int x, int y, int k, Tiles type);
	static void goUp(std::vector<std::vector<uint16_t>>& particles, int x, int y, int k, int i, Tiles type);
	static int update(std::vector<std::vector<uint16_t>>& particles, std::vector<std::vector<uint16_t>>& particlesCopy, int x, int y, Tiles type);
};

class acid {
public:
	static void horizontalMovement(std::vector<std::vector<uint16_t>>& particles, int x, int y, int k, Tiles type);
	static int update(std::vector<std::vector<uint16_t>>& particles, std::vector<std::vector<uint16_t>>& particlesCopy, int x, int y, Tiles type);
};

class plant {
public:
	static int update(std::vector<std::vector<uint16_t>>& particles, std::vector<std::vector<uint16_t>>& particlesCopy, int x, int y, Tiles type);
};