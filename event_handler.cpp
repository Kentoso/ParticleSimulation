#include "event_handler.h"

bool event_handler::clear(SDL_Event& event, std::vector<std::vector<uint16_t>>& p) {
	if (event.key.keysym.sym == SDLK_r)
	{
		for (size_t i = 0; i < p.size(); i++)
		{
			for (size_t j = 0; j < p[0].size(); j++)
			{
				p[i][j] = EMPTY;
			}
		}
		return true;
	}
	return false;
}

void event_handler::pause(SDL_Event& event, bool& updating) {
	if (event.key.keysym.sym == SDLK_SPACE)
	{
		updating = updating ? false : true;
	}
}
Tiles event_handler::chooseMaterial(SDL_Event& event) {
	switch (event.key.keysym.sym)
	{
	case (SDLK_1):
		return SAND;
		break;
	case (SDLK_2):
		return WATER;
		break;
	case (SDLK_3):
		return STONE;
		break;
	case (SDLK_4):
		return ACID;
		break;
	case (SDLK_5):
		return PLANT;
		break;
	}
}
void event_handler::changeBrushSize(SDL_Event& event, int& brushSize) {
	if (event.wheel.y > 0)
	{
		brushSize++;
	}
	else if (event.wheel.y < 0)
	{
		if (brushSize > 1) brushSize--;
	}
}