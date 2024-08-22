#include <iostream>
#include <list>
#include <SDL.h>

#include "src/Constant.h"
#include "src/map.h"
#include "src/player.h"
#include "src/Ray.h"

Player* player = nullptr;
Map* map = nullptr;
Ray rays[NUM_RAYS];

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool isGameRunning = false;

void CastAllRays()
{
	float rayAngle = player->rotationAngle - (FOV_ANGLE / 2);
	
	for(int i = 0; i < NUM_RAYS; i++)
	{
		rays[i].CastRay(rayAngle, player, map);
		rayAngle += FOV_ANGLE / NUM_RAYS;
	}
}
int InitializeWindow()
{
	if( SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout<<"SDL_Init Error: "<<SDL_GetError()<<std::endl;
		return FALSE;
	}
	window = SDL_CreateWindow(
		"SDL2 Window",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		0);

	if(!window)
	{
		std::cout<<"SDL_CreateWindow Error: "<<SDL_GetError()<<std::endl;
		return FALSE;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if(!renderer)
	{
		std::cout<<"SDL_CreateRenderer Error: "<<SDL_GetError()<<std::endl;
		return FALSE;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	return true;
}

void DestroyWindow()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Setup()
{
	player = new Player(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 5, 5,
		0, 0, PI / 2,
		100, 45 * (PI / 180));
	map = new Map();
	
}

void ProcessInput()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch(event.type)
	{
		case SDL_QUIT:
			isGameRunning = false;
			break;
		
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_ESCAPE)
				isGameRunning = false;
			if(event.key.keysym.sym == SDLK_RIGHT && event.key.keysym.sym == SDLK_LEFT)
				player->turnDirection = 0;
			else
			{
				if(event.key.keysym.sym == SDLK_RIGHT)
					player->turnDirection = 1;
				if(event.key.keysym.sym == SDLK_LEFT)
					player->turnDirection = -1;
			}
			if(event.key.keysym.sym == SDLK_UP && event.key.keysym.sym == SDLK_DOWN)
				player->walkDirection = 0;
			else
			{
				if(event.key.keysym.sym == SDLK_UP)
					player->walkDirection = 1;
				if(event.key.keysym.sym == SDLK_DOWN)
					player->walkDirection = -1;
			}
			break;
		
		case SDL_KEYUP:
			if(event.key.keysym.sym == SDLK_UP)
				player->walkDirection = 0;
			if(event.key.keysym.sym == SDLK_DOWN)
				player->walkDirection = 0;
			if(event.key.keysym.sym == SDLK_RIGHT)
				player->turnDirection = 0;
			if(event.key.keysym.sym == SDLK_LEFT)
				player->turnDirection = 0;
			break;
		
		default:
			break;
	}
}

Uint32 ticksLastFrame = 0;

void Update()
{
	// waste some time until we reach the target frame time
	//while(!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));

	// compute how long we have until the target frame time in milliseconds
	int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);

	// only delay if we are too fast
	if(timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME)
	{
		SDL_Delay(timeToWait);
	}
	
	float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
	ticksLastFrame = SDL_GetTicks();
	player->MovePlayer(deltaTime, map);
	CastAllRays();
}

void Render()
{
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);

	// TODO:
	// render all game objects for the current frame
	map->RenderMap(renderer);
	player->RenderPlayer(renderer);
	for (auto ray : rays)
	{
		ray.RenderRay(renderer, player);
	}
	
	SDL_RenderPresent(renderer);
}

int main(int argc, char* args[])
{
	// Initialize SDL
	isGameRunning =  InitializeWindow();

	Setup();
	
	// Game loop
	while(isGameRunning)
	{
		ProcessInput();
		Update();
		Render();
	}

	DestroyWindow();
	
	return 0;
}