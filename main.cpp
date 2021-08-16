#include <SDL.h>
#include <iostream>
#include <vector>
#include <utility>

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

void end() {
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);

	SDL_Quit();
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

bool isParticleIgnored(std::vector<std::pair<int, int>> ignoreParticles, int x, int y) {
	for (auto p : ignoreParticles)
	{
		if (p.first == x || p.second == y)
		{
			return true;
		}
	}
	return false;
}
void render(std::vector<std::vector<uint8_t>> &particles, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	std::vector<std::pair<int, int>> ignoreParticles;
	for (int i = 0; i < particles.size(); i++)
	{
		for (int j = 0; j < particles[0].size(); j++)
		{
			if (particles[i][j] == SAND && !isParticleIgnored(ignoreParticles, i, j))
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				if (int64_t(j) + 1 < particles[0].size()) {
					if (particles[i][int64_t(j) + 1] == SAND) 
					{
						SDL_RenderDrawPoint(renderer, i, j);
					}
					else
					{
						SDL_RenderDrawPoint(renderer, i, j + 1);
						particles[i][j] = EMPTY;
						particles[i][int64_t(j) + 1] = SAND;
						ignoreParticles.push_back(std::make_pair(int(i), int(j + 1)));
					}
				}
				else
				{
					SDL_RenderDrawPoint(renderer, i, j);
				}
			}
		}
	}
	SDL_RenderPresent(renderer);
}
int main(int argc, char* args[])
{
	bool playing = true;

	//std::vector<particle*> particles;
	//for (int i = 0; i < SCREEN_WIDTH; i++)
	//{
	//	for (int j = 200; j < 400; j++)
	//	{
	//		particles.push_back(new particle(i, j));
	//	}
	//}

	std::vector<std::vector<uint8_t>> p;
	//for (int i = 0; i < SCREEN_WIDTH; i++)
	//{
	//	std::vector<uint8_t> vecX;
	//	p.push_back(vecX);
	//	for (int j = 0; j < SCREEN_HEIGHT; j++)
	//	{
	//		p[i].push_back(EMPTY);
	//	}
	//}
	p.resize(SCREEN_WIDTH);
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		p[i].resize(SCREEN_HEIGHT, EMPTY);
	}

	for (int i = 0; i < p[0].size(); i++)
	{
		p[i][0] = SAND;
		p[i][1] = SAND;

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
			render(p, renderer);

			SDL_UpdateWindowSurface(window);
			SDL_Delay(10);
		}
	}
	//for (auto particle : particles)
	//{
	//	delete particle;
	//}
	end();

	return 0;
}