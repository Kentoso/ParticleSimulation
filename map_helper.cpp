#include "map_helper.h"

void map_helper::createBoundingBox(int screenWidth, int screenHeight, std::vector<std::vector<uint16_t>>&p, Tiles type) {
	for (size_t i = 0; i < screenWidth; i++)
	{
		p[i][screenHeight - 1] = type;
		p[i][0] = type;
	}
	for (size_t i = 0; i < screenHeight; i++)
	{
		p[0][i] = type;
		p[screenWidth - 1][i] = type;
	}

}

void map_helper::useBrush(std::vector<std::vector<uint16_t>>& p, SDL_Point mouseCoord, int brushSize, Tiles material, int SCREEN_WIDTH, int SCREEN_HEIGHT) {
	if (p[mouseCoord.x][mouseCoord.y] != BEDROCK)
	{
		if (brushSize == 1)
		{
			p[mouseCoord.x][mouseCoord.y] = material;
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
						p[mouseCoord.x + i - brushSize][mouseCoord.y + j - brushSize] = material;
					}
				}
			}
		}
	}
}