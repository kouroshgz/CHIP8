#include "Frontend.h"

Frontend::Frontend(int displayWidth, int displayHeight, int textureWidth, int textureHeight) {
	SDL_Init(SDL_INIT_VIDEO);

	// create display window
	window = SDL_CreateWindow("CHIP-8 EMU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, displayWidth*8, displayHeight*8, SDL_WINDOW_SHOWN);
	// create hardware accelerated renderer 
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	// create texture for rendering context
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
}

void Frontend::refresh(void const* buffer, int pitch){
	SDL_UpdateTexture(texture, nullptr, buffer, pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);

	
}