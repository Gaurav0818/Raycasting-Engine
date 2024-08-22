#include <iostream>
#include <SDL.h>
#include "src/Constant.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool isGameRunning = false;

int InitializeWindow()
{
	if( SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout<<"SDL_Init Error: "<<SDL_GetError()<<std::endl;
		return FALSE;
	}
	window = SDL_CreateWindow(
		"SDL2 Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS);

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

int main(int argc, char* args[])
{
	// Initialize SDL
	isGameRunning =  InitializeWindow();

	//setup();
	
	// Game loop
	while(isGameRunning)
	{
		//processInput();
		//update();
		//render();
	}

	DestroyWindow();
	
	return 0;
}