#include "textures.h"

void TextureManager::loadWallTexture() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        upng_t* upng = upng_new_from_file(textureFileNames[i].c_str());
        if (upng != NULL) {
            upng_decode(upng);
            if (upng_get_error(upng) == UPNG_EOK) {
                wallTextures[i].upngTexture = upng;
                wallTextures[i].width = upng_get_width(upng);
                wallTextures[i].height = upng_get_height(upng);
                wallTextures[i].texture_buffer = (uint32_t*)upng_get_buffer(upng);
                if (wallTextures[i].texture_buffer == nullptr) {
                    Logger::Error("Failed to load texture buffer for texture: " + std::string(textureFileNames[i]));
                }
            } else {
                Logger::Error("Failed to decode texture: " + std::string(textureFileNames[i]));
            }
        }
    }
}

void TextureManager::freeWallTexture() {
    for (int i = 0; i < NUM_TEXTURES; i++) {
        upng_free(wallTextures[i].upngTexture);
    }
}