#pragma once
#include <cstdint>
#include <vector>
#include "physics_behaviour.h"
#include <SDL.h>

class map_helper {
public:
	static void createBoundingBox(int screenWidth, int screenHeight, std::vector<std::vector<uint16_t>>& p, Tiles type);

	static void useBrush(std::vector<std::vector<uint16_t>>& p, SDL_Point mouseCoord, int brushSize, Tiles material, int SCREEN_WIDTH, int SCREEN_HEIGHT);
};