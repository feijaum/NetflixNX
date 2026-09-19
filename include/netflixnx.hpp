#pragma once
#include <switch.h>
#include <SDL2/SDL.h>

namespace nx {
enum class Screen { Home, Search, MyNetflix, Settings };
enum class View { Browse, Details, Player };

struct AppState {
    Screen screen = Screen::Home;
    View view = View::Browse;
    int focusedCard = 0;
    int focusedRow = 0;
    int detailsAction = 0;
    bool playerPaused = false;
    bool running = true;
    bool touchWasDown = false;
    Uint32 lastInputTick = 0;
};

void handleInput(AppState& state, PadState& pad, SDL_Renderer* renderer);
void render(SDL_Renderer* renderer, const AppState& state);
}
