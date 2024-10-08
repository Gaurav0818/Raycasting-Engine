#pragma once
#include <memory>
#include <stdint.h>
#include <SDL.h>
#include <chrono>

#include "../Logger/Logger.h"
#include "../Core.h"

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
    void RenderMap();
    void RenderRays();
    
    void DrawPixel(int x, int y, uint32_t& color) const;
    void DrawRect(int x, int y, int width, int height, uint32_t color) const;
    void DrawLine(double x0, double y0, double x1, double y1, uint32_t color) const;

    void CastAllRays();
    void RenderWallProjection();
    void ClearColorBuffer(uint32_t color);
    void RenderColorBuffer();

private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    
    std::unique_ptr<Player> m_player = nullptr;
    std::unique_ptr<Map> m_map = nullptr;
    std::vector<Ray> m_rays;

    int m_winWidth;
    int m_winHeight;
    int m_NumOfRays;
    double m_deltaTime;
    double m_fixedDeltaTime = FIXED_UPDATE_INTERVAL;
    
    bool m_isGameRunning = false;

    uint32_t* m_colorBuffer = nullptr;
    SDL_Texture* m_colorBufferTexture = nullptr;
    uint32_t* m_textures[NUM_TEXTURES];

    TextureManager* m_textureManager = nullptr;
};
