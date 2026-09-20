#include "netflixnx.hpp"

int main(int, char**) {
    // Keep networking optional during startup. The UI shell must be able to
    // launch even if the console is offline or socket initialization fails.
    const bool socketsReady = R_SUCCEEDED(socketInitializeDefault());

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        if (socketsReady) socketExit();
        return 2;
    }

    SDL_Window* window = SDL_CreateWindow(
        "NetflixNX", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        1280, 720, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Quit();
        if (socketsReady) socketExit();
        return 3;
    }

    // The Switch SDL port can reject an explicitly accelerated renderer
    // depending on the runtime/graphics backend. Prefer accelerated, but
    // always fall back to SDL's default renderer so the shell can boot.
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        renderer = SDL_CreateRenderer(window, -1, 0);
    }
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        if (socketsReady) socketExit();
        return 4;
    }

    PadState pad;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);

    nx::AppState state;
    Uint32 previous = SDL_GetTicks();

    while (appletMainLoop() && state.running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) state.running = false;
        }

        const Uint32 now = SDL_GetTicks();
        float dt = static_cast<float>(now - previous) / 1000.0f;
        previous = now;
        if (dt > 0.05f) dt = 0.05f;

        padUpdate(&pad);
        nx::handleInput(state, pad);
        nx::update(state, dt);
        nx::render(renderer, state);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    if (socketsReady) socketExit();
    return 0;
}
