#pragma once
#include <memory>
#include <SDL_render.h>
#include <SDL_video.h>

#include "../Logger/Logger.h"
#include "../Constant.h"

#include "../Player/Player.h"
#include "../Map/Map.h"
#include "../Ray/Ray.h"

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

public:
    //double GetDeltaTime() { return m_deltaTime; }

private:
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    
    std::unique_ptr<Player> m_player = nullptr;
    std::unique_ptr<Map> m_map = nullptr;
    Ray m_rays[NUM_RAYS];
    
    bool m_isGameRunning = false;
    Uint32 m_ticksLastFrame = 0;
    Uint32 m_ticksLastFixedFrame = 0;
    double m_deltaTime;

    Uint32* m_colorBuffer = nullptr;
    SDL_Texture* m_colorBufferTexture = nullptr;
};
