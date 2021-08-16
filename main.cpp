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
			else if (particles[x][y] == BEDROCK) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				SDL_RenderDrawPoint(renderer, x, y);
			}
		}
	}
}

bool update(std::vector<std::vector<uint16_t>>& particles) {
	int updated = 0;
	std::vector<std::vector<uint16_t>> p = particles;
	for (size_t x = 0; x < SCREEN_WIDTH; x++)
	{
		for (size_t y = 0; y < SCREEN_HEIGHT; y++)
		{
			if (p[x][y] == SAND)
			{
				if (p[x][y + 1] == EMPTY)
				{
					particles[x][y] = EMPTY;
					particles[x][y + 1] = SAND;
					updated++;
				}
				else if (p[x - 1][y + 1] == EMPTY)
				{
					particles[x][y] = EMPTY;
					particles[x - 1][y + 1] = SAND;
					updated++;
				}
				else if (p[x + 1][y + 1] == EMPTY)
				{
					particles[x][y] = EMPTY;
					particles[x + 1][y + 1] = SAND;
					updated++;
				}
			}
		}
	}
	return updated;
}

int main(int argc, char* args[])
{
	bool playing = true, updating = true, lClickHeld = false;
	SDL_Point mouseCoord;
	std::vector<std::vector<uint16_t>> p;

	p.resize(SCREEN_WIDTH);
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		p[i].resize(SCREEN_HEIGHT, EMPTY);
	}

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


	for (size_t i = 1; i < SCREEN_WIDTH - 1; i++)
	{
		for (size_t j = 1; j < SCREEN_HEIGHT / 2 - 1; j++)
		{
			p[i][j] = SAND;
		}
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
				if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
				{
					updating = updating ? false : true;
				}
				if (e.type == SDL_MOUSEMOTION)
				{
					mouseCoord.x = e.motion.x;
					mouseCoord.y = e.motion.y;
					
				}
				if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
				{
					lClickHeld = true;
				}
				if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT)
				{
					lClickHeld = false;
				}
			}
			if (lClickHeld)
			{
				if (p[mouseCoord.x][mouseCoord.y] != BEDROCK)
				{
					p[mouseCoord.x][mouseCoord.y] = SAND;
					printf("%d, %d\n", mouseCoord.x, mouseCoord.y);
					if (!updating)
					{
						draw(p, renderer);
						SDL_RenderPresent(renderer);
					}
				}
			}
			if (updating)
			{
				if (update(p))
				{
					draw(p, renderer);
					SDL_RenderPresent(renderer);
				}
			}
			
			SDL_UpdateWindowSurface(window);
			SDL_Delay(10);
		}
	}

	end();

	return 0;
}