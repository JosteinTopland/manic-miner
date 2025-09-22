#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Entity {
public:
    Entity(SDL_Texture *tex, const int sx, const int sy, const int sw, const int sh, const int frames)
        : sprite_sheet(tex), sprite_rect{sx, sy, sw, sh}, frame_count(frames), frame(0) {
    }

    void render(SDL_Renderer *renderer) const {
        int src_x = sprite_rect.x + frame * sprite_rect.w;
        if (direction == Direction::Left) {
            src_x += sprite_rect.w * frame_count;
        }
        const SDL_Rect src = {
            src_x,
            sprite_rect.y,
            sprite_rect.w,
            sprite_rect.h
        };
        const SDL_Rect dst = {x, y, src.w, src.h};
        SDL_RenderCopy(renderer, sprite_sheet, &src, &dst);
    }
    void move(const int _x) {
        if (state & State::Jumping) return;
        x += _x;
        direction = _x < 0 ? Direction::Left : Direction::Right;
        state |= State::Walking;
    }
    void jump() {
        if (state & State::Jumping) return;
        state |= State::Jumping;
        jump_frame = 0;
    }
    void stop() {
        if (state == State::Jumping) return;
        state &= ~State::Walking;
    }
    void update() {
        if (state & State::Walking) {
            frame = (frame + 1) % frame_count;
        } else {
            frame = 0;
        }
        if (state & State::Jumping) {
            jump_frame++;
            if (jump_frame <= 10) y -= 1;
            else if (jump_frame <= 20) y += 1;
            else {
                state &= ~State::Jumping;
                jump_frame = 0;
            }
        }
    }

private:
    enum class Direction { Left, Right };

    enum State { Idle = 0, Walking = 1, Jumping = 2 };

    int x = 100;
    int y = 100;
    int speed = 1.0f;

    SDL_Texture *sprite_sheet;
    SDL_Rect sprite_rect;
    int frame_count;
    int frame;
    int jump_frame = 0;
    Direction direction = Direction::Left;
    int state = State::Idle;
};

int main(int arc, char *argv[]) {
    const int width = 320;
    const int height = 200;
    const int scale = 5;
    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 1;
    }

    if (SDL_CreateWindowAndRenderer(width * scale, height * scale, SDL_WINDOW_OPENGL, &window, &renderer) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 1;
    }
    SDL_SetWindowTitle(window, "Manic Miner");
    SDL_RenderSetScale(renderer, scale, scale);

    // load gfx
    SDL_Surface *surface = SDL_LoadBMP("../assets/spritesheet.bmp");
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        return 3;
    }
    SDL_SetColorKey(surface, true, SDL_MapRGB(surface->format, 255, 0, 255));

    SDL_Texture *spritesheet = SDL_CreateTextureFromSurface(renderer, surface);
    if (!spritesheet) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return 3;
    }
    SDL_FreeSurface(surface);
    SDL_SetTextureScaleMode(spritesheet, SDL_ScaleModeNearest);

    // load snd
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);
    Mix_Chunk *snd = Mix_LoadWAV("../assets/CHIMES.WAV");

    Entity player(spritesheet, 0, 0, 16, 16, 4);

    bool run = true;
    while (run) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) {
                run = false;
            }
            if (event.type == SDL_KEYUP) {
                player.stop();
            }
        }

        const Uint8 *key_states = SDL_GetKeyboardState(nullptr);
        if (key_states[SDL_SCANCODE_LEFT]) player.move(-1);
        if (key_states[SDL_SCANCODE_RIGHT]) player.move(1);
        if (key_states[SDL_SCANCODE_UP]) {
            player.jump();
            Mix_PlayChannel(-1, snd, 0);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 50, 0);
        SDL_RenderClear(renderer);
        player.update();
        player.render(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(50);
    }

    Mix_FreeChunk(snd);
    SDL_DestroyTexture(spritesheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}