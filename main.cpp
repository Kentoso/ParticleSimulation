#include <SDL.h>
#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include "physics_behaviour.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
int lastWater = 0;

SDL_Window* window;
SDL_Surface* gScreenSurface;
SDL_Renderer* renderer;
SDL_Event e;

//enum Tiles {
//	EMPTY, 
//	SAND,
//	BEDROCK
//};

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
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
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
			else if (particles[x][y] == WATER) {
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				SDL_RenderDrawPoint(renderer, x, y);
			}
			else if (particles[x][y] == STONE) {
				SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
				SDL_RenderDrawPoint(renderer, x, y);
			}
		}
	}
}

bool update(std::vector<std::vector<uint16_t>>& particles) {
	int updated = 0;
	int water = 0;
	std::vector<std::vector<uint16_t>> p = particles;
	for (size_t x = 0; x < SCREEN_WIDTH; x++)
	{
		for (size_t y = 0; y < SCREEN_HEIGHT; y++)
		{
			if (p[x][y] == SAND)
			{
				if (p[x][y + 1] == EMPTY)
				{
					auto tile = particles[x][y + 1];
					particles[x][y] = tile;
					particles[x][y + 1] = SAND;
					updated++;
				}
				else if (p[x - 1][y + 1] == EMPTY && std::rand() % 2 == 0)
				{
					auto tile = particles[x - 1][y + 1];
					particles[x][y] = tile;
					particles[x - 1][y + 1] = SAND;
					updated++;
				}
				else if (p[x + 1][y + 1] == EMPTY && std::rand() % 2 == 1)
				{
					auto tile = particles[x + 1][y + 1];
					particles[x][y] = tile;
					particles[x + 1][y + 1] = SAND;
					updated++;
				}
			}
			if (p[x][y] == WATER)
			{
				water++;
				if (p[x][y + 1] == EMPTY)
				{
					auto tile = particles[x][y + 1];
					particles[x][y] = tile;
					particles[x][y + 1] = WATER;
					updated++;
				}
				else if (p[x - 1][y + 1] == EMPTY && std::rand() % 2 == 0)
				{
					auto tile = particles[x - 1][y + 1];
					particles[x][y] = tile;
					particles[x - 1][y + 1] = WATER;
					updated++;
				}
				else if (p[x + 1][y + 1] == EMPTY && std::rand() % 2 == 1)
				{
					auto tile = particles[x + 1][y + 1];
					particles[x][y] = tile;
					particles[x + 1][y + 1] = WATER;
					updated++;
				}
				else if (particles[x + 1][y] == EMPTY && std::rand() % 2 == 0 || particles[x][y - 1] == WATER && std::rand() % 2 == 0) {
					int n = 1;
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
					particles[x + max][y] = WATER;
					updated++;
				}
				else if (particles[x - 1][y] == EMPTY && std::rand() % 2 == 1 || particles[x][y - 1] == WATER && std::rand() % 2 == 1) {
					int n = 1;
					int max = 1;
					for (size_t i = 1; i <= 20; i++)
					{
						n = i;
						if (particles[x - n][y] != EMPTY)
						{
							max = n - 1;
							break;
						}
					}
					auto tile = particles[x - max][y];
					particles[x][y] = tile;
					particles[x - max][y] = WATER;
					updated++;
				}
				else if (p[x][y - 1] == SAND)
				{
					auto tile = particles[x][y - 1];
					particles[x][y] = tile;
					particles[x][y - 1] = WATER;
					updated++;
				}
			}
			
		}
	}
	lastWater = water;
	printf("WATER: %d\n", water);
	return updated;
}

int main(int argc, char* args[])
{
	bool playing = true, updating = true, lClickHeld = false, rClickHeld = false;
	Tiles currentElement = SAND;
	SDL_Point mouseCoord;
	int brushSize = 1;
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


	//for (size_t i = 1; i < SCREEN_WIDTH - 1; i++)
	//{
	//	for (size_t j = 1; j < SCREEN_HEIGHT / 2 - 1; j++)
	//	{
	//		p[i][j] = SAND;
	//	}
	//}

	for (size_t i = 400; i < 500; i++)
	{
		p[1][i] = WATER;
	}

	for (size_t i = 400; i < 500; i++)
	{
		p[i][400] = STONE;
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
				if (e.type == SDL_KEYDOWN)
				{
					switch (e.key.keysym.sym)
					{
					case (SDLK_SPACE):
						updating = updating ? false : true;
						break;
					case (SDLK_1):
						currentElement = SAND;
						break;
					case (SDLK_2):
						currentElement = WATER;
						break;
					case (SDLK_3):
						currentElement = STONE;
						break;
					}
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
				if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_RIGHT)
				{
					rClickHeld = true;
				}
				if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_RIGHT)
				{
					rClickHeld = false;
				}
				if (e.type == SDL_MOUSEWHEEL)
				{	
					if (e.wheel.y > 0)
					{
						brushSize++;
					}
					else if (e.wheel.y < 0)
					{
						if (brushSize > 1) brushSize--;
					}
					//std::cout << "BRUSH: " << brushSize << "\r" << std::flush;
				}
			}
			if (lClickHeld)
			{
				if (p[mouseCoord.x][mouseCoord.y] != BEDROCK)
				{
					if (brushSize == 1)
					{
						p[mouseCoord.x][mouseCoord.y] = currentElement;
					}
					else
					{
						for (size_t i = 0; i < brushSize * 2; i++)
						{
							for (size_t j = 0; j < brushSize * 2; j++)
							{
								long distanceX = std::abs(brushSize - long(i));
								long distanceY = std::abs(brushSize - long(j));
								if (distanceX * distanceX + distanceY * distanceY <= brushSize * brushSize 
									&& mouseCoord.x + i - brushSize > 0 
									&& mouseCoord.x + i - brushSize < SCREEN_WIDTH - 1
									&& mouseCoord.y + j - brushSize > 0
									&& mouseCoord.y + j - brushSize < SCREEN_HEIGHT - 1)
								{
									p[mouseCoord.x + i - brushSize][mouseCoord.y + j - brushSize] = currentElement;
								}
							}
						}
					}
					if (!updating)
					{
						draw(p, renderer);
						SDL_RenderPresent(renderer);
					}
				}
			}
			if (rClickHeld) 
			{
				if (p[mouseCoord.x][mouseCoord.y] != BEDROCK)
				{
					if (brushSize == 1)
					{
						p[mouseCoord.x][mouseCoord.y] = EMPTY;
					}
					else
					{
						for (size_t i = 0; i < brushSize * 2; i++)
						{
							for (size_t j = 0; j < brushSize * 2; j++)
							{
								long distanceX = std::abs(brushSize - long(i));
								long distanceY = std::abs(brushSize - long(j));
								if (distanceX * distanceX + distanceY * distanceY <= brushSize * brushSize
									&& mouseCoord.x + i - brushSize > 0
									&& mouseCoord.x + i - brushSize < SCREEN_WIDTH - 1
									&& mouseCoord.y + j - brushSize > 0
									&& mouseCoord.y + j - brushSize < SCREEN_HEIGHT - 1)
								{
									p[mouseCoord.x + i - brushSize][mouseCoord.y + j - brushSize] = EMPTY;
								}
							}
						}
					}
					if (!updating)
					{
						draw(p, renderer);
						SDL_RenderPresent(renderer);
					}
				}
			}
			//printf("%d\n", p[mouseCoord.x][mouseCoord.y]);
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