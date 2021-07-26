#pragma once
#include "SDL.h"
class Frontend
{
public:
	Frontend(int displayWidth, int displayHeight, int textureWidth, int textureHeight);
	void refresh(void const* buffer, int pitch);
	~Frontend() {
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};

