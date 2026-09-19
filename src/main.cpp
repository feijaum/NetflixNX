#include "netflixnx.hpp"

int main(int argc, char** argv) {
    socketInitializeDefault();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
        socketExit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "NetflixNX", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    PadState pad;
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeDefault(&pad);

    nx::AppState state;
    while (appletMainLoop() && state.running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) state.running = false;
        }

        padUpdate(&pad);
        nx::handleInput(state, pad, renderer);
        nx::render(renderer, state);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    socketExit();
    return 0;
}
