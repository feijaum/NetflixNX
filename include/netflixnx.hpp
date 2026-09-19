#pragma once
#include <switch.h>
#include <SDL2/SDL.h>

namespace nx {
enum class Screen { Home, Search, MyNetflix, Settings };

struct AppState {
    Screen screen = Screen::Home;
    int focusedCard = 0;
    bool running = true;
};

void handleInput(AppState& state, PadState& pad, SDL_Renderer* renderer);
void render(SDL_Renderer* renderer, const AppState& state);
}
