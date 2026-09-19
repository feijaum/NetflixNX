#include "netflixnx.hpp"
#include <algorithm>

namespace nx {
static SDL_Rect cardRect(int i) {
    return SDL_Rect{70 + i * 230, 250, 200, 285};
}

static void selectScreen(AppState& s, int delta) {
    int v = static_cast<int>(s.screen);
    v = (v + delta + 4) % 4;
    s.screen = static_cast<Screen>(v);
    s.focusedCard = 0;
}

void handleInput(AppState& s, PadState& pad, SDL_Renderer*) {
    const u64 down = padGetButtonsDown(&pad);
    if (down & (HidNpadButton_Right | HidNpadButton_StickLRight))
        s.focusedCard = std::min(4, s.focusedCard + 1);
    if (down & (HidNpadButton_Left | HidNpadButton_StickLLeft))
        s.focusedCard = std::max(0, s.focusedCard - 1);
    if (down & HidNpadButton_R) selectScreen(s, +1);
    if (down & HidNpadButton_L) selectScreen(s, -1);
    if (down & HidNpadButton_Plus) s.running = false;

    HidTouchScreenState touch{};
    if (hidGetTouchScreenStates(&touch, 1) > 0 && touch.count > 0) {
        const int x = touch.touches[0].x;
        const int y = touch.touches[0].y;
        for (int i = 0; i < 5; ++i) {
            SDL_Rect r = cardRect(i);
            if (x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h) {
                s.focusedCard = i;
                break;
            }
        }
    }
}

void render(SDL_Renderer* r, const AppState& s) {
    SDL_SetRenderDrawColor(r, 12, 12, 14, 255);
    SDL_RenderClear(r);

    // Header / navigation rail.
    SDL_SetRenderDrawColor(r, 25, 25, 29, 255);
    SDL_Rect header{0, 0, 1280, 105};
    SDL_RenderFillRect(r, &header);

    // Active section indicator.
    SDL_SetRenderDrawColor(r, 235, 235, 235, 255);
    SDL_Rect active{70 + static_cast<int>(s.screen) * 150, 82, 110, 4};
    SDL_RenderFillRect(r, &active);

    // Hero placeholder.
    SDL_SetRenderDrawColor(r, 35, 35, 42, 255);
    SDL_Rect hero{70, 135, 1140, 80};
    SDL_RenderFillRect(r, &hero);

    // Media cards. No Netflix artwork is bundled.
    for (int i = 0; i < 5; ++i) {
        SDL_Rect card = cardRect(i);
        if (i == s.focusedCard) {
            SDL_SetRenderDrawColor(r, 245, 245, 245, 255);
            SDL_Rect focus{card.x - 5, card.y - 5, card.w + 10, card.h + 10};
            SDL_RenderFillRect(r, &focus);
        }
        SDL_SetRenderDrawColor(r, 55 + i * 8, 55 + i * 8, 62 + i * 8, 255);
        SDL_RenderFillRect(r, &card);
    }

    // Footer hint bar.
    SDL_SetRenderDrawColor(r, 24, 24, 28, 255);
    SDL_Rect footer{0, 665, 1280, 55};
    SDL_RenderFillRect(r, &footer);
}
}
