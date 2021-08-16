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
	SAND,
	BEDROCK
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

void render(std::vector<std::vector<uint16_t>> &particles, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	std::vector<std::pair<int, int>> ignoreParticles;
	for (int i = 0; i < particles.size(); i++)
	{
		for (int j = 0; j < particles[0].size(); j++)
		{
			int x = i;
			int y = particles[0].size() - 1 - j;
			if (particles[x][y] == SAND)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
				if (int64_t(y) + 1 < particles[0].size()) {
					if (particles[x][int64_t(y) + 1] == EMPTY)
					{
						SDL_RenderDrawPoint(renderer, x, y + 1);
						particles[x][y] = EMPTY;
						particles[x][int64_t(y) + 1] = SAND;
					}
					else if (x - 1 > 0) {
						if (particles[x - 1][int64_t(y) + 1] == EMPTY)
						{
							SDL_RenderDrawPoint(renderer, x - 1, y + 1);
							particles[x][y] = EMPTY;
							particles[x - 1][int64_t(y) + 1] = SAND;
						}
					}
					else if (x + 1 < SCREEN_WIDTH) {
						if (particles[x + 1][int64_t(y) + 1] == EMPTY)
						{
							SDL_RenderDrawPoint(renderer, x - 1, y + 1);
							particles[x][y] = EMPTY;
							particles[x + 1][int64_t(y) + 1] = SAND;
						}
					}
					else
					{
						SDL_RenderDrawPoint(renderer, x, y);
					}
				}
				else
				{
					SDL_RenderDrawPoint(renderer, x, y);
				}
			}
		}
	}
	SDL_RenderPresent(renderer);
}

bool isIgnored(std::vector<std::pair<int, int>> ignorePos, int x, int y) {
	for (auto pos : ignorePos)
	{
		if (x == pos.first && y == pos.second)
		{
			return true;
		}
	}
	return false;
}

void draw(std::vector<std::vector<uint16_t>>& particles, SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	for (size_t x = 0; x < SCREEN_WIDTH; x++)
	{
		for (size_t y = 0; y < SCREEN_HEIGHT; y++)
		{
			if (particles[x][y] == SAND)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
				SDL_RenderDrawPoint(renderer, x, y);
			}
		}
	}
}

bool update(std::vector<std::vector<uint16_t>>& particles, SDL_Renderer* renderer) {
	int updated = 0;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	std::vector<std::vector<uint16_t>> p = particles;
	for (size_t x = 0; x < SCREEN_WIDTH; x++)
	{
		for (size_t y = 0; y < SCREEN_HEIGHT; y++)
		{
			if (p[x][y] == SAND)
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
				if (p[x][y + 1] == EMPTY)
				{
					SDL_RenderDrawPoint(renderer, x, y + 1);
					particles[x][y] = EMPTY;
					particles[x][y + 1] = SAND;
					updated++;
				}
				else if (p[x - 1][y + 1] == EMPTY)
				{
					SDL_RenderDrawPoint(renderer, x - 1, y + 1);
					particles[x][y] = EMPTY;
					particles[x - 1][y + 1] = SAND;
					updated++;
				}
				else if (p[x + 1][y + 1] == EMPTY)
				{
					SDL_RenderDrawPoint(renderer, x + 1, y + 1);
					particles[x][y] = EMPTY;
					particles[x + 1][y + 1] = SAND;
					updated++;
				}
				else
				{
					SDL_RenderDrawPoint(renderer, x, y);
				}

			}
			else if (p[x][y] == BEDROCK) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderDrawPoint(renderer, x, y);
			}
		}
	}
	return updated;
}

int main(int argc, char* args[])
{
	bool playing = true;

	std::vector<std::vector<uint16_t>> p;

	p.resize(SCREEN_WIDTH);
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		p[i].resize(SCREEN_HEIGHT, EMPTY);
	}
	//for (size_t i = 0; i < 100; i++)
	//{	
	//	p[400][i] = SAND;
	//}
	//for (int j = 0; j < p.size(); j++)
	//{
	//	for (int i = 0; i < p[0].size(); i++)
	//	{
	//		//p[i][p[0].size() - 1 - i] = SAND;
	//		p[j][i] = SAND;
	//	}
	//}

	for (size_t i = 0; i < SCREEN_WIDTH; i++)
	{
		p[i][SCREEN_HEIGHT - 1] = BEDROCK;
		p[i][0] = BEDROCK;
	}
	for (size_t i = 0; i < SCREEN_HEIGHT; i++)
	{
		p[0][i] = BEDROCK;
		p[SCREEN_WIDTH - 1][i] = BEDROCK;
	}






	if (initializationAndLoading())
	{
		int i = 0;
		while (playing)
		{
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_QUIT)
				{
					playing = false;
				}
			}
			update(p, renderer);
			SDL_RenderPresent(renderer);
			SDL_UpdateWindowSurface(window);
			SDL_Delay(10);
		
			if (i++ % 10 == 0)
			{
				/*p[500][500] = SAND;
				p[501][500] = SAND;
				p[501][501] = SAND;
				p[500][501] = SAND;*/
			}	
		}
	}

	end();

	return 0;
}