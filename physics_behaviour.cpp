#include "physics_behaviour.h"

int dust::update(std::vector<std::vector<uint16_t>>& particles, std::vector<std::vector<uint16_t>>& p, int x, int y, Tiles type) {
	int updated = 0;
	if (p[x][y + 1] == EMPTY)
	{
		auto tile = particles[x][y + 1];
		particles[x][y] = tile;
		particles[x][y + 1] = type;
		updated++;
	}
	else if (p[x - 1][y + 1] == EMPTY && std::rand() % 2 == 0)
	{
		auto tile = particles[x - 1][y + 1];
		particles[x][y] = tile;
		particles[x - 1][y + 1] = type;
		updated++;
	}
	else if (p[x + 1][y + 1] == EMPTY && std::rand() % 2 == 1)
	{
		auto tile = particles[x + 1][y + 1];
		particles[x][y] = tile;
		particles[x + 1][y + 1] = type;
		updated++;
	}
	return updated;
}

void liquid::goUp(std::vector<std::vector<uint16_t>>& particles, int x, int y, int k, int i, Tiles type) {
	auto p = particles;
	for (size_t i = 1; i <= 2; i++)
	{
		if (p[x][y - i] == EMPTY)
		{
			particles[x][y] = EMPTY;
			particles[x + i * k][y - i] = type;
			break;
		}
	}
}

void liquid::horizontalMovement(std::vector<std::vector<uint16_t>>& particles, int x, int y, int k, Tiles type) {
	int max = 1;
	for (size_t i = 1; i <= 20; i++)
	{
		if (particles[x + i * k][y] != EMPTY)
		{
			max = i - 1;
			break;
		}
	}
	max *= k;
	auto tile = particles[x + max][y];
	particles[x][y] = tile;
	particles[x + max][y] = type;
}

int liquid::update(std::vector<std::vector<uint16_t>>& particles, std::vector<std::vector<uint16_t>>& p, int x, int y, Tiles type) {
	int updated = 0;
	if (p[x][y + 1] == EMPTY)
	{
		auto tile = particles[x][y + 1];
		particles[x][y] = tile;
		particles[x][y + 1] = type;
		updated++;
	}
	else if (p[x - 1][y + 1] == EMPTY && std::rand() % 2 == 0)
	{
		auto tile = particles[x - 1][y + 1];
		particles[x][y] = tile;
		particles[x - 1][y + 1] = type;
		updated++;
	}
	else if (p[x + 1][y + 1] == EMPTY && std::rand() % 2 == 1)
	{
		auto tile = particles[x + 1][y + 1];
		particles[x][y] = tile;
		particles[x + 1][y + 1] = type;
		updated++;
	}
	else if (particles[x + 1][y] == EMPTY && std::rand() % 2 == 0 || particles[x][y - 1] == WATER && std::rand() % 2 == 0) {
		/*int n = 1;
		int max = 1;
		for (size_t i = 1; i <= 20; i++)
		{
			n = i;
			if (particles[x + n][y] != EMPTY)
			{
				max = n - 1;
				break;
			}
		}
		auto tile = particles[x + max][y];
		particles[x][y] = tile;
		particles[x + max][y] = type;*/
		horizontalMovement(particles, x, y, 1, type);
		updated++;
	}
	else if (particles[x - 1][y] == EMPTY && std::rand() % 2 == 1 || particles[x][y - 1] == WATER && std::rand() % 2 == 1) {
		//int n = 1;
		//int max = 1;
		//for (size_t i = 1; i <= 20; i++)
		//{
		//	n = i;
		//	if (particles[x - n][y] != EMPTY)
		//	{
		//		max = n - 1;
		//		break;
		//	}
		//}
		//auto tile = particles[x - max][y];
		//particles[x][y] = tile;
		//particles[x - max][y] = type;
		horizontalMovement(particles, x, y, -1, type);
		updated++;
	}
	else if (p[x][y - 1] == SAND || p[x][y - 1] == ACID)
	{
		auto tile = particles[x][y - 1];
		particles[x][y] = tile;
		particles[x][y - 1] = type;
		updated++;
	}
	return updated;
}

void acid::horizontalMovement(std::vector<std::vector<uint16_t>>& particles, int x, int y, int k, Tiles type) {
	int max = 1;
	for (size_t i = 1; i <= 20; i++)
	{
		if (particles[x + i * k][y] != EMPTY)
		{
			max = i - 1;
			break;
		}
	}
	max *= k;
	particles[x][y] = EMPTY;
	particles[x + max][y] = type;
}

int acid::update(std::vector<std::vector<uint16_t>>& particles, std::vector<std::vector<uint16_t>>& p, int x, int y, Tiles type) {
	int updated = 0;

	if (p[x][y + 1] == EMPTY)
	{
		auto tile = particles[x][y + 1];
		particles[x][y] = tile;
		particles[x][y + 1] = type;
		updated++;
	}
	else if (p[x][y + 1] != BEDROCK && p[x][y + 1] != ACID)
	{
		auto tile = particles[x][y + 1];
		particles[x][y] = EMPTY;
		particles[x][y + 1] = EMPTY;
		updated++;
	}
	else if (p[x - 1][y + 1] == EMPTY && std::rand() % 2 == 0)
	{
		auto tile = particles[x - 1][y + 1];
		particles[x][y] = tile;
		particles[x - 1][y + 1] = type;
		updated++;
	}
	else if (p[x + 1][y + 1] == EMPTY && std::rand() % 2 == 1)
	{
		auto tile = particles[x + 1][y + 1];
		particles[x][y] = tile;
		particles[x + 1][y + 1] = type;
		updated++;
	}
	else if (particles[x + 1][y] == EMPTY && std::rand() % 2 == 0 || particles[x][y - 1] == ACID && std::rand() % 2 == 0) {
		acid::horizontalMovement(particles, x, y, 1, type);
		updated++;
	}
	else if (particles[x - 1][y] == EMPTY && std::rand() % 2 == 1 || particles[x][y - 1] == ACID && std::rand() % 2 == 1) {
		acid::horizontalMovement(particles, x, y, -1, type);
		updated++;
	}

	if (particles[x][y - 1] != BEDROCK && particles[x][y - 1] != EMPTY && particles[x][y - 1] != ACID) {
		particles[x][y - 1] = EMPTY;
		particles[x][y] = EMPTY;
		updated++;
	}

	return updated;
}

int plant::update(std::vector<std::vector<uint16_t>>& particles, std::vector<std::vector<uint16_t>>& p, int x, int y, Tiles type) {
	int updated = 0;

	for (int i = 0; i <= 1; i++)
	{
		for (int j = 0; j <= 1; j++)
		{
			if (i == 0 && j == 0)
			{
				continue;
			}
			if (particles[x + i][y + j] == WATER)
			{
				particles[x + i][y + j] = PLANT;
				updated++;
			}
			if (particles[x - i][y - j] == WATER)
			{
				particles[x - i][y - j] = PLANT;
				updated++;
			}
		}
	}

	return updated;
}