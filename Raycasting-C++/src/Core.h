#pragma once
#include <math.h>

#define PI 3.14159265
#define TWO_PI 6.28318530

#define TILE_SIZE 64
#define MAP_NUM_ROWS 13
#define MAP_NUM_COLS 20
#define NUM_TEXTURES 8

#define FOV_ANGLE (60 * (PI / 180))

#define MINIMAP_SCALE_FACTOR 0.2

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)
#define FIXED_UPDATE_INTERVAL 0.01667 // 1 / 60

inline uint32_t encodeRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (a << 24) | (b << 16) | (g << 8) | r;
}