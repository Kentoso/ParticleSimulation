#include <SDL.h>
#include <iostream>
#include <vector>
#include "particle.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Window* window;
SDL_Surface* gScreenSurface;
SDL_Renderer* renderer;
SDL_Event e;

enum Tiles {
	EMPTY, 
	SAND
};

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
		}
	}
	return true;
}

bool load() {
	return true;
}

void end() {
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);

	SDL_Quit();
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
void render(std::vector<particle*> particles, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	for (auto p : particles)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawPoint(renderer, p->x, p->y);
		if (!(p->y + 1>= SCREEN_HEIGHT))
		{
			++p->y;
		}
	}
}
int main(int argc, char* args[])
{
	bool playing = true;

	std::vector<particle*> particles;
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 200; j < 400; j++)
		{
			particles.push_back(new particle(i, j));
		}
	}
	if (initializationAndLoading())
	{
		while (playing)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					playing = false;
				}
			}
			render(particles, renderer);
			SDL_RenderPresent(renderer);
			SDL_UpdateWindowSurface(window);
			SDL_Delay(17);
		}
	}
	for (auto particle : particles)
	{
		delete particle;
	}
	end();

	return 0;
}