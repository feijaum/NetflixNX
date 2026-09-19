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
    int searchKey = 0;
    int searchLength = 0;
    int profile = 0;
    float scrollX[2] = {0.0f, 0.0f};
    float targetScrollX[2] = {0.0f, 0.0f};
    bool playerPaused = false;
    bool running = true;
    bool touchWasDown = false;
    Uint32 lastInputTick = 0;
};

void update(AppState& state, float dt);
void handleInput(AppState& state, PadState& pad, SDL_Renderer* renderer);
void render(SDL_Renderer* renderer, const AppState& state);
}
