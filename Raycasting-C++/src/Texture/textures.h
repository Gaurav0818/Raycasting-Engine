#pragma once

#include <stdint.h>
#include "../Core.h"
#include "../upng/upng.h"
#include "../Logger/Logger.h"

class TextureManager {
public:
    void loadWallTexture();
    void freeWallTexture();
    
    std::string textureFileNames[NUM_TEXTURES] = {
        "./././images/redbrick.png",
        "./././images/purplestone.png",
        "./././images/mossystone.png",
        "./././images/graystone.png",
        "./././images/colorstone.png",
        "./././images/bluestone.png",
        "./././images/wood.png",
        "./././images/eagle.png"
    };

    struct Texture {
        upng_t* upngTexture;
        int width;
        int height;
        uint32_t* texture_buffer;
    };

    Texture wallTextures[NUM_TEXTURES];
};