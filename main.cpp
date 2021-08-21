#include <SDL.h>
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include "physics_behaviour.h"
#include "map_helper.h"
#include "event_handler.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
int lastWater = 0;

SDL_Window* window;
SDL_Surface* gScreenSurface;
SDL_Renderer* renderer;
SDL_Event e;

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1,
				SDL_RENDERER_SOFTWARE);
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
		}
	}
	return true;
}

void initParticleGrid(std::vector<std::vector<uint16_t>>& p) {
	p.resize(SCREEN_WIDTH);
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		p[i].resize(SCREEN_HEIGHT, EMPTY);
	}
}

bool load() {
	return true;
}

bool initializationAndLoading() {
	if (!init())
	{
		printf("Failed to initialize!\n");
		return false;
	}
	else
	{
		if (!load())
		{
			printf("Failed to load media!\n");
			return false;
		}
	}
	return true;
}



void draw(std::vector<std::vector<uint16_t>>& particles, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	for (size_t x = 0; x < SCREEN_WIDTH; x++)
	{
		for (size_t y = 0; y < SCREEN_HEIGHT; y++)
		{
			if (particles[x][y] == EMPTY)
			{
				continue;
			}
			switch (particles[x][y])
			{
			case(SAND):
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
				break;
			case(BEDROCK):
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				break;
			case(WATER):
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				break;
			case(STONE):
				SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
				break;
			case(ACID):
				SDL_SetRenderDrawColor(renderer, 0, 100, 20, 255);
				break;
			case(PLANT):
				SDL_SetRenderDrawColor(renderer, 120, 210, 80, 255);
				break;
			}
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
}

bool update(std::vector<std::vector<uint16_t>>& particles, bool inverse) {
	int updated = 0;
	int water = 0;
	std::vector<std::vector<uint16_t>> p = particles;
	if (!inverse)
	{
		for (size_t x = 0; x < SCREEN_WIDTH; x++)
		{
			for (size_t y = 0; y < SCREEN_HEIGHT; y++)
			{
				if (p[x][y] == SAND)
				{
					updated += dust::update(particles, p, x, y, SAND);
				}
				else if (p[x][y] == WATER)
				{
					water++;
					updated += liquid::update(particles, p, x, y, WATER);
				}
				else if (p[x][y] == ACID) {
					updated += acid::update(particles, p, x, y, ACID);
				}
				else if (p[x][y] == PLANT) {
					updated += plant::update(particles, p, x, y, ACID);
				}
			}
		}
	}
	else
	{
		for (int x = SCREEN_WIDTH - 1; x >= 0; x--)
		{
			for (int y = SCREEN_HEIGHT - 1; y >= 0; y--)
			{
				if (p[x][y] == SAND)
				{
					updated += dust::update(particles, p, x, y, SAND);
				}
				else if (p[x][y] == WATER)
				{
					water++;
					updated += liquid::update(particles, p, x, y, WATER);
				}
				else if (p[x][y] == ACID) {
					updated += acid::update(particles, p, x, y, ACID);
				}
				else if (p[x][y] == PLANT) {
					updated += plant::update(particles, p, x, y, ACID);
				}
			}
		}
	}
	
	lastWater = water;
	return updated;
}

void end() {
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	bool playing = true, updating = true, wasUpdated = false, inverse = false;

	bool mouseButtonsActive[] = { false, false, false };
	SDL_Point mouseCoord;

	Tiles currentMaterial = SAND;
	int brushSize = 1;
	int frame = 1;
	std::vector<std::vector<uint16_t>> p;
	initParticleGrid(p);

	map_helper::createBoundingBox(SCREEN_WIDTH, SCREEN_HEIGHT, p, BEDROCK);

	if (initializationAndLoading())
	{
		while (playing)
		{
			auto buttons = SDL_GetMouseState(&mouseCoord.x, &mouseCoord.y);
			mouseButtonsActive[0] = buttons & SDL_BUTTON(1);
			mouseButtonsActive[1] = buttons & SDL_BUTTON(2);
			mouseButtonsActive[2] = buttons & SDL_BUTTON(3);

			while (SDL_PollEvent(&e))
			{
				if (e.type == SDL_QUIT)
				{
					playing = false;
				}
				if (e.type == SDL_KEYDOWN)
				{
					if (event_handler::clear(e, p))
					{
						map_helper::createBoundingBox(SCREEN_WIDTH, SCREEN_HEIGHT, p, BEDROCK);
						draw(p, renderer);
						SDL_RenderPresent(renderer);
					}
					
					event_handler::pause(e, updating);
					currentMaterial = event_handler::chooseMaterial(e);
				}
				if (e.type == SDL_MOUSEWHEEL)
				{
					event_handler::changeBrushSize(e, brushSize);
				}
			}
			if (mouseButtonsActive[0])
			{
				map_helper::useBrush(p, mouseCoord, brushSize, currentMaterial, SCREEN_WIDTH, SCREEN_HEIGHT);
				if (!updating || !wasUpdated)
				{
					draw(p, renderer);
					SDL_RenderPresent(renderer);
				}
			}
			if (mouseButtonsActive[2]) 
			{
				map_helper::useBrush(p, mouseCoord, brushSize, EMPTY, SCREEN_WIDTH, SCREEN_HEIGHT);
				if (!updating || !wasUpdated)
				{
					draw(p, renderer);
					SDL_RenderPresent(renderer);
				}
			}
			auto preFrameTime = SDL_GetTicks();

			if (updating)
			{
				if (update(p, inverse))
				{
					wasUpdated = true;
					inverse = inverse ? false : true;

					SDL_RenderPresent(renderer);
					if (frame)
					{
						draw(p, renderer);
					}
				}	
				else
				{
					wasUpdated = false;
				}
			}
			frame = frame == 0 ? 1 : 0;
			SDL_UpdateWindowSurface(window);
			//SDL_Delay(10);
			auto delay = (SDL_GetTicks() - preFrameTime) > 10 ? (SDL_GetTicks() - preFrameTime) : 10;
			printf("%d\n", delay);
			SDL_Delay(delay);
		}
	}

	end();

	return 0;
}