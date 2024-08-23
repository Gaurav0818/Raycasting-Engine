#include "Game.h"

#include <iostream>

Game::Game()
{
    Logger::Info("Game Created");
}

Game::~Game()
{
    Logger::Info("Game Destroyed");
}

void Game::Initialize()
{
    if( SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Logger::Error("Error Initializing SDL. " + std::string(SDL_GetError()));
        m_isGameRunning = false;
        return;
    }
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    m_winWidth = display_mode.w;
    m_winHeight = display_mode.h;
    m_window = SDL_CreateWindow(
        "SDL2 Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        m_winWidth, m_winHeight,
        0);

    if(!m_window)
    {
        Logger::Error("Error Creating SDL Window. " + std::string(SDL_GetError()));
        m_isGameRunning = false;
        return;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    if(!m_renderer)
    {
        Logger::Error("Error Creating SDL Renderer. " + std::string(SDL_GetError()));
        m_isGameRunning = false;
        return;
    }

    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    m_isGameRunning = true;
}

void Game::Run()
{
    Setup();

    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    auto lastFixedUpdateTime = lastFrameTime;
    
    // Game loop
    while(m_isGameRunning)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> deltaTime = currentTime - lastFrameTime;
        m_deltaTime = deltaTime.count();
        lastFrameTime = currentTime;
        
        ProcessInput();
        Update();
        
        std::chrono::duration<double> fixedUpdateTime = currentTime - lastFixedUpdateTime;
        
        if (fixedUpdateTime.count() >= FIXED_UPDATE_INTERVAL)
        {
            FixedUpdate();
            lastFixedUpdateTime = currentTime;
        }
        
        Render();
    }
}

void Game::Destroy()
{
    m_textureManager->freeWallTexture();
    
    delete m_textureManager;
    delete[] m_colorBuffer;
    
    SDL_DestroyTexture(m_colorBufferTexture);
	
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Game::Setup()
{
    m_NumOfRays = m_winWidth;
    m_rays.resize(m_NumOfRays);
    
    m_player = std::make_unique<Player>(m_winWidth / 2, m_winHeight / 2, 5, 5,
                0, 0, PI / 2,
                100, 45 * (PI / 180));

    m_map = std::make_unique<Map>();

    // allocate the memory for the color buffer
    m_colorBuffer = new Uint32[m_winWidth * m_winHeight];
    
    m_colorBufferTexture = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        m_winWidth,
        m_winHeight);

    m_textureManager = new TextureManager();
    m_textureManager->loadWallTexture();
}

void Game::LoadLevel()
{
    
}

void Game::ProcessInput()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type)
    {
    case SDL_QUIT:
        m_isGameRunning = false;
        break;
        
    case SDL_KEYDOWN:
        if(event.key.keysym.sym == SDLK_ESCAPE)
            m_isGameRunning = false;
        if(event.key.keysym.sym == SDLK_RIGHT && event.key.keysym.sym == SDLK_LEFT)
            m_player->m_turnDirection = 0;
        else
        {
            if(event.key.keysym.sym == SDLK_RIGHT)
                m_player->m_turnDirection = 1;
            if(event.key.keysym.sym == SDLK_LEFT)
                m_player->m_turnDirection = -1;
        }
        if(event.key.keysym.sym == SDLK_UP && event.key.keysym.sym == SDLK_DOWN)
            m_player->m_walkDirection = 0;
        else
        {
            if(event.key.keysym.sym == SDLK_UP)
                m_player->m_walkDirection = 1;
            if(event.key.keysym.sym == SDLK_DOWN)
                m_player->m_walkDirection = -1;
        }
        break;
		
    case SDL_KEYUP:
        if(event.key.keysym.sym == SDLK_UP)
            m_player->m_walkDirection = 0;
        if(event.key.keysym.sym == SDLK_DOWN)
            m_player->m_walkDirection = 0;
        if(event.key.keysym.sym == SDLK_RIGHT)
            m_player->m_turnDirection = 0;
        if(event.key.keysym.sym == SDLK_LEFT)
            m_player->m_turnDirection = 0;
        break;
		
    default:
        break;
    }
}

void Game::Update()
{
    m_player->MovePlayer(m_deltaTime, m_map);
    CastAllRays();
}

void Game::FixedUpdate()
{

}

void Game::Render()
{
    SDL_SetRenderDrawColor(m_renderer,0,0,0,255);
    SDL_RenderClear(m_renderer);

    RenderWallProjection();
    RenderMap();
    RenderColorBuffer();
    ClearColorBuffer(0x00000000);
	
    // draw
    for (auto ray : m_rays)
    {
        ray.RenderRay(m_renderer, m_player);
    }
    m_player->RenderPlayer(m_renderer);

    SDL_RenderPresent(m_renderer);
}

void Game::RenderMap()
{
    for (int i = 0; i < MAP_NUM_ROWS; i++)
    {
        for (int j = 0; j < MAP_NUM_COLS; j++)
        {
            int tileX = j * TILE_SIZE;
            int tileY = i * TILE_SIZE;
            int tileColor = m_map->m_grid[i][j] != 0 ? 255 : 0;
            DrawRect(
                    static_cast<int>(MINIMAP_SCALE_FACTOR * tileX),
                    static_cast<int>(MINIMAP_SCALE_FACTOR * tileY),
                    static_cast<int>(MINIMAP_SCALE_FACTOR * TILE_SIZE),
                    static_cast<int>(MINIMAP_SCALE_FACTOR * TILE_SIZE),
                    encodeRGBA(tileColor, tileColor, tileColor, 255));
        }
    }
}

void Game::CastAllRays()
{
    for(int col = 0; col < m_NumOfRays; col++)
    {
        double rayAngle = m_player->m_rotationAngle + atan((col - m_NumOfRays/2) / ((m_winWidth / 2) / tan(FOV_ANGLE / 2)));
        m_rays[col].CastRay(rayAngle, m_player, m_map);
    }
}

void Game::DrawPixel(int x, int y, uint32_t color) const
{
    m_colorBuffer[(m_winWidth * y) + x] = color;
}

void Game::DrawRect(int x, int y, int width, int height, uint32_t color) const
{
    for(int i = x; i < x + width; i++)
    {
        for(int j = y; j < y + height; j++)
        {
            DrawPixel(i, j, color);
        }
    }
}

void Game::RenderWallProjection()
{
    for(int x = 0; x < m_NumOfRays; x++)
    {
        double perpDistance = m_rays[x].m_distance * std::cos(m_rays[x].m_rayAngle - m_player->m_rotationAngle);
        double projectedWallHeight = (TILE_SIZE / perpDistance) * ((m_winWidth / 2) / tan(FOV_ANGLE / 2));
		
        int wallStripHeight = static_cast<int>(projectedWallHeight);
		
        int wallTopPixel = (m_winHeight / 2) - (wallStripHeight / 2);
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;
		
        int wallBottomPixel = (m_winHeight / 2) + (wallStripHeight / 2);
        wallBottomPixel = wallBottomPixel > m_winHeight ? m_winHeight : wallBottomPixel;

        // get the correct texture id number from the map
        int texNum = m_rays[x].m_wallHitContent - 1;
        
        if(texNum < 0) texNum = 0;
        
        int textureWidth = m_textureManager->wallTextures[texNum].width;
        int textureHeight = m_textureManager->wallTextures[texNum].height;
        
        // draw the wall
        int textureOffsetX;
        if(m_rays[x].m_wasHitVertical)
            textureOffsetX = static_cast<int>(m_rays[x].m_wallHitY) % textureWidth;
        else
            textureOffsetX = static_cast<int>(m_rays[x].m_wallHitX) % textureHeight;
        
        for(int y = wallTopPixel; y < wallBottomPixel; y++)
        {
            int distanceFromTop = y + (wallStripHeight / 2) - (m_winHeight / 2);
            int textureOffsetY = distanceFromTop * textureHeight / wallStripHeight;
    
            uint32_t texelColor = m_textureManager->wallTextures[texNum].texture_buffer[(textureWidth * textureOffsetY) + textureOffsetX];
            DrawPixel(x,y,texelColor);
        }
		
        // draw the ceiling
        for(int y= 0; y < wallTopPixel; y++)
            DrawPixel(x, y,encodeRGBA(0, 0, 150, 255)); 
		
        // draw the floor
        for(int y = wallBottomPixel; y < m_winHeight; y++)
            DrawPixel(x, y, encodeRGBA(0, 100, 0, 255)); 
    }
}

void Game::ClearColorBuffer(uint32_t color)
{
    for(int i =0; i < m_winWidth* m_winHeight; i++)
        m_colorBuffer[i] = color;
}

void Game::RenderColorBuffer()
{
    SDL_UpdateTexture(
        m_colorBufferTexture,
        NULL,
        m_colorBuffer,
        (int)((uint32_t)m_winWidth * sizeof(uint32_t))
        );
    SDL_RenderCopy(m_renderer, m_colorBufferTexture, NULL, NULL);
}
