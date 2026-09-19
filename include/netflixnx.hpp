#pragma once
#include <switch.h>
#include <SDL2/SDL.h>

namespace nx {
enum class Screen { Home, Search, MyNetflix, Settings };
enum class View { Profiles, Browse, Details, Player };

struct AppState {
    Screen screen = Screen::Home;
    View view = View::Profiles;
    int focusedCard = 0, focusedRow = 0, detailsAction = 0;
    int searchKey = 0, searchLength = 0, profile = 0;
    float scrollX[2] = {0,0}, targetScrollX[2] = {0,0};
    float focusPulse = 0.0f;
    bool playerPaused = false, running = true, touchWasDown = false;
    Uint32 lastInputTick = 0;
};
void update(AppState&, float);
void handleInput(AppState&, PadState&, SDL_Renderer*);
void render(SDL_Renderer*, const AppState&);
}
