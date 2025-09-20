#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

class Entity {
public:
    Entity(SDL_Texture *tex, const float sx, const float sy, const float sw, const float sh, const int frames)
        : sprite_sheet(tex), sprite_rect{sx, sy, sw, sh}, frame_count(frames), frame(0) {
    }

    void render(SDL_Renderer *renderer) const {
        float src_x = sprite_rect.x + static_cast<float>(frame) * sprite_rect.w;
        if (direction == Direction::Left) {
            src_x += sprite_rect.w * static_cast<float>(frame_count);
        }
        const SDL_FRect src = {
            src_x,
            sprite_rect.y,
            sprite_rect.w,
            sprite_rect.h
        };
        const SDL_FRect dst = {x, y, src.w, src.h};
        SDL_RenderTexture(renderer, sprite_sheet, &src, &dst);
    }
    void move(const float _x) {
        if (state == State::Jumping) return;
        state = State::Walking;
        direction = _x < 0 ? Direction::Left : Direction::Right;
        x += _x;
    }
    void jump() {
        if (state == State::Jumping) return;
        state = State::Jumping;
        jump_frame = 0;
    }
    void stop() {
        if (state == State::Jumping) return;
        state = State::Idle;
    }
    void update() {
        if (state == State::Walking) {
            frame = (frame + 1) % frame_count;
        }
        if (state == State::Jumping) {
            jump_frame++;
            if (jump_frame <= 10) y -= 1;
            else if (jump_frame <= 20) y += 1;
            else {
                state = State::Idle;
                jump_frame = 0;
            }
        }
    }

private:
    enum class Direction { Left, Right };

    enum class State { Idle = 0, Walking = 1, Jumping = 2 };

    float x = 100;
    float y = 100;
    float speed = 1.0f;

    SDL_Texture *sprite_sheet;
    SDL_FRect sprite_rect;
    int frame_count;
    int frame;
    int jump_frame = 0;
    Direction direction = Direction::Left;
    State state = State::Idle;
};

int main(int arc, char *argv[]) {
    const int width = 320;
    const int height = 200;
    const int scale = 5;
    SDL_Window *window;
    SDL_Renderer *renderer;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (!SDL_CreateWindowAndRenderer("Pitfall", width * scale, height * scale, SDL_WINDOW_OPENGL, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }
    SDL_SetRenderScale(renderer, scale, scale);

    // load gfx
    SDL_Surface *surface = SDL_LoadBMP("../assets/spritesheet.bmp");
    if (!surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        return 3;
    }
    SDL_SetSurfaceColorKey(surface, true, SDL_MapSurfaceRGB(surface, 255, 0, 255));

    SDL_Texture *spritesheet = SDL_CreateTextureFromSurface(renderer, surface);
    if (!spritesheet) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return 3;
    }
    SDL_DestroySurface(surface);
    SDL_SetTextureScaleMode(spritesheet, SDL_SCALEMODE_NEAREST);

    // load snd

    Entity player(spritesheet, 0, 0, 16, 16, 4);

    bool run = true;
    SDL_Event event;
    while (run) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT ||
                event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_Q) {
                run = false;
            }
            if (event.type == SDL_EVENT_KEY_UP) {
                player.stop();
            }
        }

        const bool *key_states = SDL_GetKeyboardState(nullptr);
        if (key_states[SDL_SCANCODE_LEFT]) player.move(-1);
        if (key_states[SDL_SCANCODE_RIGHT]) player.move(1);
        if (key_states[SDL_SCANCODE_UP]) player.jump();

        SDL_SetRenderDrawColor(renderer, 0, 0, 50, 0);
        SDL_RenderClear(renderer);
        player.update();
        player.render(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(50);
    }

    SDL_DestroyTexture(spritesheet);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}