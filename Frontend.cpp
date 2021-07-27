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

bool Frontend::getInput(uint8_t* keypad) {
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}


			const uint8_t* state = SDL_GetKeyboardState(NULL);
			if (state[SDL_SCANCODE_ESCAPE])
				quit = true;
			if (state[SDL_SCANCODE_1]) {
				keypad[1] = 1;
			}
			else {
				keypad[1] = 0;
			}

			if (state[SDL_SCANCODE_2]) {
				keypad[2] = 1;
			}
			else {
				keypad[2] = 0;
			}

			if (state[SDL_SCANCODE_3]) {
				keypad[3] = 1;
			}
			else {
				keypad[3] = 0;
			}

			if (state[SDL_SCANCODE_Q]) {
				keypad[4] = 1;
			}
			else {
				keypad[4] = 0;
			}
			break;
		}
		break;
	}

	return quit;

}

void Frontend::refresh(void const* buffer, int pitch){
	SDL_UpdateTexture(texture, nullptr, buffer, pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);

	
}