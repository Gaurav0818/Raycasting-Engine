#include "Map.h"

bool Map::HasWallAt(double x, double y)
{
    if(x < 0 || x >= MAP_NUM_COLS * TILE_SIZE || y < 0 || y >= MAP_NUM_ROWS * TILE_SIZE)
    {
        return true;
    }
    int mapGridIndexX = floor(x / TILE_SIZE);
    int mapGridIndexY = floor(y / TILE_SIZE);
    return m_grid[mapGridIndexY][mapGridIndexX] != 0; 
}
