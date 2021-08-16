#include "physics_behaviour.h"

int dust::update(std::vector<std::vector<uint16_t>> particles, std::vector<std::vector<uint16_t>> particlesCopy, int x, int y, Tiles type) {
	int updated = 0;
	if (particlesCopy[x][y + 1] == EMPTY)
	{
		particles[x][y] = EMPTY;
		particles[x][y + 1] = type;
		updated++;
	}
	else if (particlesCopy[x - 1][y + 1] == EMPTY)
	{
		particles[x][y] = EMPTY;
		particles[x - 1][y + 1] = type;
		updated++;
	}
	else if (particlesCopy[x + 1][y + 1] == EMPTY)
	{
		particles[x][y] = EMPTY;
		particles[x + 1][y + 1] = type;
		updated++;
	}
	return updated;
}