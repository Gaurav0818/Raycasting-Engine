#include "Map.h"

bool Map::HasWallAt(float x, float y)
{
    if(x < 0 || x >= WINDOW_WIDTH || y < 0 || y >= WINDOW_HEIGHT)
    {
        return true;
    }
    int mapGridIndexX = floor(x / TILE_SIZE);
    int mapGridIndexY = floor(y / TILE_SIZE);
    return grid[mapGridIndexY][mapGridIndexX] != 0;
}

void Map::RenderMap(SDL_Renderer* renderer)
{
    for (int i = 0; i < MAP_NUM_ROWS; i++)
    {
        for (int j = 0; j < MAP_NUM_COLS; j++)
        {
            int tileX = j * TILE_SIZE;
            int tileY = i * TILE_SIZE;
            int tileColor = grid[i][j] != 0 ? 255 : 0;
            SDL_SetRenderDrawColor(renderer, tileColor, tileColor, tileColor, 255);
            SDL_Rect mapTileRect = {tileX, tileY, TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(renderer, &mapTileRect);
        }
    }
}
