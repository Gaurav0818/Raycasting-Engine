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
    m_window = SDL_CreateWindow(
        "SDL2 Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
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
    
    // Game loop
    while(m_isGameRunning)
    {
        ProcessInput();
        FixedUpdate();
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
    m_player = std::make_unique<Player>(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 5, 5,
                0, 0, PI / 2,
                100, 45 * (PI / 180));

    m_map = std::make_unique<Map>();

    // allocate the memory for the color buffer
    m_colorBuffer = new Uint32[WINDOW_WIDTH * WINDOW_HEIGHT];

    m_colorBufferTexture = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        WINDOW_WIDTH,
        WINDOW_HEIGHT);

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
}

void Game::FixedUpdate()
{
    // waste some time until we reach the target frame time
    //while(!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));

    // compute how long we have until the target frame time in milliseconds
    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - m_ticksLastFrame);

    // only delay if we are too fast
    if(timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(timeToWait);
    }
	
    m_deltaTime = (SDL_GetTicks() - m_ticksLastFrame) / 1000.0;
    m_ticksLastFrame = SDL_GetTicks();
    m_player->MovePlayer(m_deltaTime, m_map);
    CastAllRays();
}

void Game::Render()
{
    SDL_SetRenderDrawColor(m_renderer,0,0,0,255);
    SDL_RenderClear(m_renderer);

    Generate3DProjection();

    RenderColorBuffer();
    ClearColorBuffer(0x00000000);
	
    // draw
    m_map->RenderMap(m_renderer);
    m_player->RenderPlayer(m_renderer);
    for (auto ray : m_rays)
    {
        ray.RenderRay(m_renderer, m_player);
    }
	
    SDL_RenderPresent(m_renderer);
}

void Game::CastAllRays()
{
    double rayAngle = m_player->m_rotationAngle - (FOV_ANGLE / 2);
	
    for(int i = 0; i < NUM_RAYS; i++)
    {
        m_rays[i].CastRay(rayAngle, m_player, m_map);
        rayAngle += FOV_ANGLE / NUM_RAYS;
    }
}

void Game::Generate3DProjection()
{
    for(int x = 0; x < NUM_RAYS; x++)
    {
        double perpDistance = m_rays[x].m_distance * std::cos(m_rays[x].m_rayAngle - m_player->m_rotationAngle);
        double distanceToProjectionPlane = (WINDOW_WIDTH / 2) / std::tan(FOV_ANGLE / 2);
        double projectedWallHeight = (TILE_SIZE / perpDistance) * distanceToProjectionPlane;
		
        int wallStripHeight = static_cast<int>(projectedWallHeight);
		
        int wallTopPixel = (WINDOW_HEIGHT / 2) - (wallStripHeight / 2);
        wallTopPixel = wallTopPixel < 0 ? 0 : wallTopPixel;
		
        int wallBottomPixel = (WINDOW_HEIGHT / 2) + (wallStripHeight / 2);
        wallBottomPixel = wallBottomPixel > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomPixel;

        // draw the wall
        int textureOffsetX;
        if(m_rays[x].m_wasHitVertical)
            textureOffsetX = static_cast<int>(m_rays[x].m_wallHitY) % TEXTURE_WIDTH;
        else
            textureOffsetX = static_cast<int>(m_rays[x].m_wallHitX) % TEXTURE_WIDTH;

        // get the correct texture id number from the map
        int texNum = m_rays[x].m_wallHitContent - 1;
        for(int y = wallTopPixel; y < wallBottomPixel; y++)
        {
            int distanceFromTop = y + (wallStripHeight / 2) - (WINDOW_HEIGHT / 2);
            int textureOffsetY = distanceFromTop * TEXTURE_HEIGHT / wallStripHeight;
    
            uint32_t texelColor = m_textureManager->wallTextures[texNum].texture_buffer[(TEXTURE_WIDTH * textureOffsetY) + textureOffsetX];
            m_colorBuffer[(WINDOW_WIDTH * y) + x] = texelColor;
        }
		
        // draw the ceiling
        for(int y= 0; y < wallTopPixel; y++)
            m_colorBuffer[(WINDOW_WIDTH * y) + x] = encodeRGBA(0, 0, 150, 255); 
		
        // draw the floor
        for(int y = wallBottomPixel; y < WINDOW_HEIGHT; y++)
            m_colorBuffer[(WINDOW_WIDTH * y) + x] = encodeRGBA(0, 100, 0, 255); 
    }
}

void Game::ClearColorBuffer(uint32_t color)
{
    for(int x =0; x < WINDOW_WIDTH; x++)
    {
        for(int y = 0; y < WINDOW_HEIGHT; y++)
        {
            m_colorBuffer[(WINDOW_WIDTH * y) + x] = color;
        }
    }
}

void Game::RenderColorBuffer()
{
    SDL_UpdateTexture(
        m_colorBufferTexture,
        NULL,
        m_colorBuffer,
        (int)((uint32_t)WINDOW_WIDTH * sizeof(uint32_t))
        );
    SDL_RenderCopy(m_renderer, m_colorBufferTexture, NULL, NULL);
}
