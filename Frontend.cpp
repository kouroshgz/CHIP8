#include "Frontend.h"

Frontend::Frontend(int displayWidth, int displayHeight, int textureWidth, int textureHeight) {
	SDL_Init(SDL_INIT_VIDEO);

	// create display window
	window = SDL_CreateWindow("CHIP-8 EMU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, displayWidth, displayHeight, SDL_WINDOW_SHOWN);
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
				keypad[0] = 1;
			}
			else {
				keypad[0] = 0;
			}

			if (state[SDL_SCANCODE_2]) {
				keypad[1] = 1;
			}
			else {
				keypad[1] = 0;
			}

			if (state[SDL_SCANCODE_3]) {
				keypad[2] = 1;
			}
			else {
				keypad[2] = 0;
			}
			if (state[SDL_SCANCODE_4]) {
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

			if (state[SDL_SCANCODE_W]) {
				keypad[5] = 1;
			}
			else {
				keypad[5] = 0;
			}

			if (state[SDL_SCANCODE_E]) {
				keypad[6] = 1;
			}
			else {
				keypad[6] = 0;
			}

			if (state[SDL_SCANCODE_R]) {
				keypad[7] = 1;
			}
			else {
				keypad[7] = 0;
			}

			if (state[SDL_SCANCODE_A]) {
				keypad[8] = 1;
			}
			else {
				keypad[8] = 0;
			}

			if (state[SDL_SCANCODE_S]) {
				keypad[9] = 1;
			}
			else {
				keypad[9] = 0;
			}

			if (state[SDL_SCANCODE_D]) {
				keypad[0xA] = 1;
			}
			else {
				keypad[0xA] = 0;
			}

			if (state[SDL_SCANCODE_F]) {
				keypad[0xB] = 1;
			}
			else {
				keypad[0xB] = 0;
			}

			if (state[SDL_SCANCODE_Z]) {
				keypad[0xC] = 1;
			}
			else {
				keypad[0xC] = 0;
			}

			if (state[SDL_SCANCODE_X]) {
				keypad[0xD] = 1;
			}
			else {
				keypad[0xD] = 0;
			}

			if (state[SDL_SCANCODE_C]) {
				keypad[0xE] = 1;
			}
			else {
				keypad[0xE] = 0;
			}

			if (state[SDL_SCANCODE_V]) {
				keypad[0xF] = 1;
			}
			else {
				keypad[0xF] = 0;
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