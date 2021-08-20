#pragma once
#include <SDL.h>
#include "physics_behaviour.h"
#include <vector>
#include <cstdint>
class event_handler {
public:

	static bool clear(SDL_Event& event, std::vector<std::vector<uint16_t>>& p);

	static void pause(SDL_Event& event, bool& updating);
	
	static Tiles chooseMaterial(SDL_Event& event);
		
	static void changeBrushSize(SDL_Event& event, int& brushSize);
};