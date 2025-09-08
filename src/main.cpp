#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

class Sprite {
public:
    Sprite(SDL_Texture *tex, const float sx, const float sy, const float sw, const float sh)
        : texture(tex), src{sx, sy, sw, sh} {
    }

    void draw(SDL_Renderer *renderer, const float x, const float y) const {
        const SDL_FRect dst = {x, y, src.w, src.h};
        SDL_RenderTexture(renderer, texture, &src, &dst);
    }

private:
    SDL_Texture *texture;
    SDL_FRect src;
};

int main(int, char *[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (!SDL_CreateWindowAndRenderer("Pitfall", 320, 200, SDL_WINDOW_OPENGL, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    SDL_Surface *surface = SDL_LoadBMP("../assets/spritesheet.bmp");
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        return 3;
    }

    SDL_Texture *spritesheet = SDL_CreateTextureFromSurface(renderer, surface);
    if (!spritesheet) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return 3;
    }
    SDL_DestroySurface(surface);

    const Sprite player(spritesheet, 1, 11, 16, 32);

    while (true) {
        SDL_PollEvent(&event);
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_Q) {
            break;
        };


        SDL_SetRenderDrawColor(renderer, 0, 100, 0, 0);
        SDL_RenderClear(renderer);
        player.draw(renderer, 10, 10);
        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }

    SDL_DestroyTexture(spritesheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
