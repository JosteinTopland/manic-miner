#ifndef PITFALL_H
#define	PITFALL_H

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

class Pitfall {
public:
	Pitfall();
	void init();
	void mainLoop();
	void quit();
private:
	void input(bool& run);
	void render();

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* sprites;
};

#endif