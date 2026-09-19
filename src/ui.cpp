#include "netflixnx.hpp"
#include <algorithm>
#include <cmath>

namespace nx {
static constexpr int kCols = 5;
static constexpr int kRows = 2;

static SDL_Rect cardRect(int row, int col, bool focused = false) {
    const int w = focused ? 222 : 210;
    const int h = focused ? 126 : 118;
    const int baseX = 54 + col * 238;
    const int baseY = 382 + row * 150;
    return SDL_Rect{baseX - (w - 210) / 2, baseY - (h - 118) / 2, w, h};
}

static SDL_Rect navRect(int i) { return SDL_Rect{52 + i * 145, 44, 124, 42}; }
static SDL_Rect detailsButton(int i) { return SDL_Rect{62 + i * 190, 286, 170, 48}; }

static void fill(SDL_Renderer* r, SDL_Rect rect, Uint8 c) {
    SDL_SetRenderDrawColor(r, c, c, c, 255);
    SDL_RenderFillRect(r, &rect);
}

static void selectScreen(AppState& s, int delta) {
    int v = static_cast<int>(s.screen);
    s.screen = static_cast<Screen>((v + delta + 4) % 4);
    s.focusedCard = 0;
    s.focusedRow = 0;
    s.view = View::Browse;
}

static void openFocused(AppState& s) {
    if (s.view == View::Browse) {
        s.view = View::Details;
        s.detailsAction = 0;
    } else if (s.view == View::Details && s.detailsAction == 0) {
        s.view = View::Player;
        s.playerPaused = false;
    } else if (s.view == View::Player) {
        s.playerPaused = !s.playerPaused;
    }
}

static void back(AppState& s) {
    if (s.view == View::Player) s.view = View::Details;
    else if (s.view == View::Details) s.view = View::Browse;
}

void handleInput(AppState& s, PadState& pad, SDL_Renderer*) {
    const u64 down = padGetButtonsDown(&pad);
    if (down) s.lastInputTick = SDL_GetTicks();

    if (down & HidNpadButton_Plus) { s.running = false; return; }
    if (down & HidNpadButton_B) back(s);
    if (down & HidNpadButton_A) openFocused(s);

    if (s.view == View::Browse) {
        if (down & (HidNpadButton_Right | HidNpadButton_StickLRight))
            s.focusedCard = std::min(kCols - 1, s.focusedCard + 1);
        if (down & (HidNpadButton_Left | HidNpadButton_StickLLeft))
            s.focusedCard = std::max(0, s.focusedCard - 1);
        if (down & (HidNpadButton_Down | HidNpadButton_StickLDown))
            s.focusedRow = std::min(kRows - 1, s.focusedRow + 1);
        if (down & (HidNpadButton_Up | HidNpadButton_StickLUp))
            s.focusedRow = std::max(0, s.focusedRow - 1);
        if (down & HidNpadButton_R) selectScreen(s, +1);
        if (down & HidNpadButton_L) selectScreen(s, -1);
    } else if (s.view == View::Details) {
        if (down & (HidNpadButton_Right | HidNpadButton_StickLRight))
            s.detailsAction = std::min(1, s.detailsAction + 1);
        if (down & (HidNpadButton_Left | HidNpadButton_StickLLeft))
            s.detailsAction = std::max(0, s.detailsAction - 1);
    }

    HidTouchScreenState touch{};
    hidGetTouchScreenStates(&touch, 1);
    const bool touchDown = touch.count > 0;
    if (touchDown && !s.touchWasDown) {
        const int x = touch.touches[0].x, y = touch.touches[0].y;
        s.lastInputTick = SDL_GetTicks();

        if (s.view == View::Browse) {
            for (int i = 0; i < 4; ++i) {
                SDL_Rect n = navRect(i);
                if (x >= n.x && x < n.x+n.w && y >= n.y && y < n.y+n.h) {
                    s.screen = static_cast<Screen>(i); s.focusedCard = 0; s.focusedRow = 0;
                }
            }
            for (int row = 0; row < kRows; ++row) for (int col = 0; col < kCols; ++col) {
                SDL_Rect c = cardRect(row, col);
                if (x >= c.x && x < c.x+c.w && y >= c.y && y < c.y+c.h) {
                    if (s.focusedRow == row && s.focusedCard == col) openFocused(s);
                    else { s.focusedRow = row; s.focusedCard = col; }
                }
            }
        } else if (s.view == View::Details) {
            for (int i = 0; i < 2; ++i) {
                SDL_Rect b = detailsButton(i);
                if (x >= b.x && x < b.x+b.w && y >= b.y && y < b.y+b.h) {
                    s.detailsAction = i; if (i == 0) openFocused(s);
                }
            }
        } else {
            openFocused(s);
        }
    }
    s.touchWasDown = touchDown;
}

static void renderNav(SDL_Renderer* r, const AppState& s) {
    SDL_SetRenderDrawColor(r, 10,10,12,245);
    SDL_Rect top{0,0,1280,105}; SDL_RenderFillRect(r,&top);
    for (int i=0;i<4;++i) {
        SDL_Rect n=navRect(i);
        fill(r,n,i==static_cast<int>(s.screen)?54:24);
        if (i==static_cast<int>(s.screen)) {
            SDL_SetRenderDrawColor(r,229,9,20,255);
            SDL_Rect line{n.x,n.y+n.h+7,n.w,4}; SDL_RenderFillRect(r,&line);
        }
    }
    // Minimal N-shaped brand mark placeholder; no proprietary logo asset is bundled.
    SDL_SetRenderDrawColor(r,229,9,20,255);
    SDL_Rect mark{1185,34,30,50}; SDL_RenderFillRect(r,&mark);
}

static void renderBrowse(SDL_Renderer* r, const AppState& s) {
    renderNav(r,s);

    // Cinematic hero gradient approximation.
    for (int y=105;y<355;y+=10) {
        Uint8 c=static_cast<Uint8>(31 - (y-105)/18);
        SDL_SetRenderDrawColor(r,c,c,c+3,255);
        SDL_Rect band{0,y,1280,10}; SDL_RenderFillRect(r,&band);
    }
    fill(r,SDL_Rect{54,145,440,22},205);
    fill(r,SDL_Rect{54,181,330,13},100);
    fill(r,SDL_Rect{54,205,390,13},80);
    SDL_SetRenderDrawColor(r,235,235,235,255);
    SDL_Rect play{54,248,154,52}; SDL_RenderFillRect(r,&play);
    fill(r,SDL_Rect{222,248,154,52},68);

    for(int row=0;row<kRows;++row) {
        fill(r,SDL_Rect{54,352+row*150,170,12},125);
        for(int col=0;col<kCols;++col) {
            const bool f=(row==s.focusedRow&&col==s.focusedCard);
            SDL_Rect c=cardRect(row,col,f);
            if(f) {
                SDL_SetRenderDrawColor(r,245,245,245,255);
                SDL_Rect border{c.x-4,c.y-4,c.w+8,c.h+8}; SDL_RenderFillRect(r,&border);
            }
            SDL_SetRenderDrawColor(r,static_cast<Uint8>(43+row*12+col*6),
                                   static_cast<Uint8>(45+col*5),
                                   static_cast<Uint8>(52+row*10),255);
            SDL_RenderFillRect(r,&c);
            // Faux title/meta bars keep the prototype asset-free.
            fill(r,SDL_Rect{c.x+12,c.y+c.h-31,c.w-50,7},155);
            fill(r,SDL_Rect{c.x+12,c.y+c.h-17,c.w-88,5},92);
        }
    }
}

static void renderDetails(SDL_Renderer* r, const AppState& s) {
    SDL_SetRenderDrawColor(r,10,10,12,255); SDL_RenderClear(r);
    SDL_SetRenderDrawColor(r,31,31,37,255);
    SDL_Rect backdrop{0,0,1280,430}; SDL_RenderFillRect(r,&backdrop);
    // Dark left gradient/panel for metadata.
    SDL_SetRenderDrawColor(r,14,14,16,245);
    SDL_Rect panel{0,0,650,430}; SDL_RenderFillRect(r,&panel);
    fill(r,SDL_Rect{62,82,420,34},215);
    fill(r,SDL_Rect{62,135,300,12},115);
    fill(r,SDL_Rect{62,160,470,10},88);
    fill(r,SDL_Rect{62,180,430,10},78);
    for(int i=0;i<2;++i) {
        SDL_Rect b=detailsButton(i);
        if(i==s.detailsAction) {
            SDL_SetRenderDrawColor(r,245,245,245,255);
            SDL_Rect border{b.x-4,b.y-4,b.w+8,b.h+8}; SDL_RenderFillRect(r,&border);
        }
        fill(r,b,i==0?210:64);
    }
    fill(r,SDL_Rect{62,475,200,15},125);
    for(int i=0;i<4;++i) fill(r,SDL_Rect{62+i*290,510,260,145},48+i*7);
}

static void renderPlayer(SDL_Renderer* r, const AppState& s) {
    SDL_SetRenderDrawColor(r,0,0,0,255); SDL_RenderClear(r);
    fill(r,SDL_Rect{48,42,360,16},115);
    // Player transport shell.
    fill(r,SDL_Rect{64,620,1152,5},95);
    fill(r,SDL_Rect{64,620,360,5},220);
    fill(r,SDL_Rect{64,646,48,32},s.playerPaused?220:115);
    fill(r,SDL_Rect{124,654,150,8},105);
    fill(r,SDL_Rect{1040,654,176,8},85);
}

void render(SDL_Renderer* r, const AppState& s) {
    SDL_SetRenderDrawColor(r,8,8,10,255); SDL_RenderClear(r);
    if(s.view==View::Browse) renderBrowse(r,s);
    else if(s.view==View::Details) renderDetails(r,s);
    else renderPlayer(r,s);
}
}
