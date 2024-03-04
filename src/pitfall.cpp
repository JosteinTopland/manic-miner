#include "pitfall.h"
#include <SDL2/SDL.h>
#include <iostream>

Pitfall::Pitfall() {
	init();
}

void Pitfall::init() {
	float windowScale = 4.0f;
	int width = 320 * (int)windowScale;
	int height = 200 * (int)windowScale;

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &window, &renderer);
	SDL_RenderSetScale(renderer, windowScale, windowScale);

	SDL_Surface* surface = SDL_LoadBMP("assets/spritesheet.bmp");
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
	sprites = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

void Pitfall::input(bool& run) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_q:
				run = false;
				break;
			}
		}
	}

}

void Pitfall::mainLoop() {
	bool run = true;
	while (run) {
		int ticks = SDL_GetTicks();

		this->input(run);
		this->render();
		
		int fps = 60;
		int delay = 1000 / fps - (SDL_GetTicks() - ticks);
		if (delay > 0) SDL_Delay(delay);
	}

}

void Pitfall::render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	int col = 2;
	int row = 8;
	int w = 24;
	int h = 21;

	int x, y;
	SDL_GetMouseState(&x, &y);
	x /= 4;
	y /= 4;

	SDL_Rect src = { col * w, row * h, w, h };
	SDL_Rect dst = { x, y, w, h };
	SDL_RenderCopy(renderer, sprites, &src, &dst);

	SDL_RenderPresent(renderer);
}

void Pitfall::quit() {
	SDL_DestroyTexture(sprites);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	Pitfall pitfall;
	pitfall.mainLoop();
	pitfall.quit();

	return 0;
}