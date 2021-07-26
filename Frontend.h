#pragma once
#include "SDL.h"
class Frontend
{
public:
	Frontend(int displayWidth, int displayHeight, int textureWidth, int textureHeight);
	void refresh(void const* buffer, int pitch);
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};

