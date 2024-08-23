#pragma once
#include <memory>
#include <stdint.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include "../Logger/Logger.h"
#include "../Constant.h"

#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Ray/Ray.h"

#include "../Texture/textures.h"

class Game
{
public:
    Game();
    ~Game();

    void Initialize();
    void Run();
    void Destroy();
    
private:
    
    void Setup();
    void LoadLevel();
    void ProcessInput();
    void Update();
    void FixedUpdate();
    void Render();

    void CastAllRays();
    void Generate3DProjection();
    void ClearColorBuffer(uint32_t color);
    void RenderColorBuffer();

private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    
    std::unique_ptr<Player> m_player = nullptr;
    std::unique_ptr<Map> m_map = nullptr;
    Ray m_rays[NUM_RAYS];
    
    bool m_isGameRunning = false;
    uint32_t m_ticksLastFrame = 0;
    double m_deltaTime;

    uint32_t* m_colorBuffer = nullptr;
    SDL_Texture* m_colorBufferTexture = nullptr;
    uint32_t* m_textures[NUM_TEXTURES];
};
