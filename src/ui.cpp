#include "netflixnx.hpp"
#include <algorithm>
#include <cmath>

namespace nx {
static constexpr int kCols=8, kVisibleCols=5, kRows=2;
static SDL_Rect navRect(int i){return {52+i*145,44,124,42};}
static SDL_Rect detailsButton(int i){return {62+i*190,286,170,48};}
static SDL_Rect keyRect(int i){return {70+(i%10)*112,205+(i/10)*72,96,54};}
static void fill(SDL_Renderer*r,SDL_Rect q,Uint8 c){SDL_SetRenderDrawColor(r,c,c,c,255);SDL_RenderFillRect(r,&q);}
static bool hit(int x,int y,const SDL_Rect&q){return x>=q.x&&x<q.x+q.w&&y>=q.y&&y<q.y+q.h;}

static SDL_Rect cardRect(const AppState&s,int row,int col,bool focused=false){
    const int w=focused?222:210,h=focused?126:118;
    int x=54+col*238-(int)s.scrollX[row], y=382+row*150;
    return {x-(w-210)/2,y-(h-118)/2,w,h};
}
static void syncScroll(AppState&s){
    for(int row=0;row<kRows;++row){
        int focus=(row==s.focusedRow)?s.focusedCard:0;
        s.targetScrollX[row]=(float)std::max(0,(focus-(kVisibleCols-1))*238);
    }
}
void update(AppState&s,float dt){
    for(int i=0;i<kRows;++i){
        float d=s.targetScrollX[i]-s.scrollX[i];
        s.scrollX[i]+=d*std::min(1.0f,dt*11.0f);
        if(std::fabs(d)<0.25f)s.scrollX[i]=s.targetScrollX[i];
    }
}
static void selectScreen(AppState&s,int delta){
    int v=static_cast<int>(s.screen); s.screen=static_cast<Screen>((v+delta+4)%4);
    s.focusedCard=s.focusedRow=0;s.view=View::Browse;syncScroll(s);
}
static void openFocused(AppState&s){
    if(s.screen==Screen::Search&&s.view==View::Browse){ if(s.searchLength<24)++s.searchLength; return; }
    if(s.screen==Screen::Settings&&s.view==View::Browse){s.profile=(s.profile+1)%4;return;}
    if(s.view==View::Browse){s.view=View::Details;s.detailsAction=0;}
    else if(s.view==View::Details&&s.detailsAction==0){s.view=View::Player;s.playerPaused=false;}
    else if(s.view==View::Player)s.playerPaused=!s.playerPaused;
}
static void back(AppState&s){
    if(s.screen==Screen::Search&&s.view==View::Browse&&s.searchLength>0){--s.searchLength;return;}
    if(s.view==View::Player)s.view=View::Details;else if(s.view==View::Details)s.view=View::Browse;
}
void handleInput(AppState&s,PadState&pad,SDL_Renderer*){
    u64 down=padGetButtonsDown(&pad); if(down)s.lastInputTick=SDL_GetTicks();
    if(down&HidNpadButton_Plus){s.running=false;return;} if(down&HidNpadButton_B)back(s);

    if(s.view==View::Browse&&s.screen==Screen::Search){
        if(down&(HidNpadButton_Right|HidNpadButton_StickLRight))s.searchKey=std::min(29,s.searchKey+1);
        if(down&(HidNpadButton_Left|HidNpadButton_StickLLeft))s.searchKey=std::max(0,s.searchKey-1);
        if(down&(HidNpadButton_Down|HidNpadButton_StickLDown))s.searchKey=std::min(29,s.searchKey+10);
        if(down&(HidNpadButton_Up|HidNpadButton_StickLUp))s.searchKey=std::max(0,s.searchKey-10);
    } else if(s.view==View::Browse){
        if(down&(HidNpadButton_Right|HidNpadButton_StickLRight))s.focusedCard=std::min(kCols-1,s.focusedCard+1);
        if(down&(HidNpadButton_Left|HidNpadButton_StickLLeft))s.focusedCard=std::max(0,s.focusedCard-1);
        if(down&(HidNpadButton_Down|HidNpadButton_StickLDown))s.focusedRow=std::min(kRows-1,s.focusedRow+1);
        if(down&(HidNpadButton_Up|HidNpadButton_StickLUp))s.focusedRow=std::max(0,s.focusedRow-1);
        syncScroll(s);
    } else if(s.view==View::Details){
        if(down&(HidNpadButton_Right|HidNpadButton_StickLRight))s.detailsAction=std::min(1,s.detailsAction+1);
        if(down&(HidNpadButton_Left|HidNpadButton_StickLLeft))s.detailsAction=std::max(0,s.detailsAction-1);
    }
    if(down&HidNpadButton_R)selectScreen(s,+1); if(down&HidNpadButton_L)selectScreen(s,-1);
    if(down&HidNpadButton_A)openFocused(s);

    HidTouchScreenState t{};hidGetTouchScreenStates(&t,1);bool td=t.count>0;
    if(td&&!s.touchWasDown){
        int x=t.touches[0].x,y=t.touches[0].y;s.lastInputTick=SDL_GetTicks();
        if(s.view==View::Browse){
            for(int i=0;i<4;++i)if(hit(x,y,navRect(i))){s.screen=static_cast<Screen>(i);s.focusedCard=s.focusedRow=0;syncScroll(s);}
            if(s.screen==Screen::Search){for(int i=0;i<30;++i)if(hit(x,y,keyRect(i))){s.searchKey=i;openFocused(s);}}
            else for(int row=0;row<kRows;++row)for(int col=0;col<kCols;++col){SDL_Rect c=cardRect(s,row,col);if(hit(x,y,c)){if(s.focusedRow==row&&s.focusedCard==col)openFocused(s);else{s.focusedRow=row;s.focusedCard=col;syncScroll(s);}}}
        }else if(s.view==View::Details){for(int i=0;i<2;++i){SDL_Rect b=detailsButton(i);if(hit(x,y,b)){s.detailsAction=i;if(i==0)openFocused(s);}}}
        else openFocused(s);
    }s.touchWasDown=td;
}
static void nav(SDL_Renderer*r,const AppState&s){
    fill(r,{0,0,1280,105},10);for(int i=0;i<4;++i){SDL_Rect n=navRect(i);fill(r,n,i==(int)s.screen?54:24);if(i==(int)s.screen){SDL_SetRenderDrawColor(r,229,9,20,255);SDL_Rect l{n.x,n.y+n.h+7,n.w,4};SDL_RenderFillRect(r,&l);}}
    SDL_SetRenderDrawColor(r,229,9,20,255);SDL_Rect m{1185,34,30,50};SDL_RenderFillRect(r,&m);
}
static void browse(SDL_Renderer*r,const AppState&s){
    nav(r,s);for(int y=105;y<355;y+=10){Uint8 c=(Uint8)(31-(y-105)/18);SDL_SetRenderDrawColor(r,c,c,c+3,255);SDL_Rect b{0,y,1280,10};SDL_RenderFillRect(r,&b);}
    fill(r,{54,145,440,22},205);fill(r,{54,181,330,13},100);fill(r,{54,205,390,13},80);fill(r,{54,248,154,52},220);fill(r,{222,248,154,52},68);
    for(int row=0;row<kRows;++row){fill(r,{54,352+row*150,170,12},125);for(int col=0;col<kCols;++col){bool f=row==s.focusedRow&&col==s.focusedCard;SDL_Rect c=cardRect(s,row,col,f);if(c.x>1280||c.x+c.w<0)continue;if(f){fill(r,{c.x-4,c.y-4,c.w+8,c.h+8},245);}SDL_SetRenderDrawColor(r,(Uint8)(43+row*12+col*5),(Uint8)(45+col*4),(Uint8)(52+row*10),255);SDL_RenderFillRect(r,&c);fill(r,{c.x+12,c.y+c.h-31,c.w-50,7},155);fill(r,{c.x+12,c.y+c.h-17,c.w-88,5},92);}}
}
static void search(SDL_Renderer*r,const AppState&s){
    nav(r,s);fill(r,{70,130,1100,54},28);fill(r,{88,151,std::min(900,24+s.searchLength*32),12},150);
    for(int i=0;i<30;++i){SDL_Rect k=keyRect(i);if(i==s.searchKey)fill(r,{k.x-4,k.y-4,k.w+8,k.h+8},235);fill(r,k,50+(i%3)*7);fill(r,{k.x+35,k.y+20,26,12},150);}
    fill(r,{70,450,240,14},110);for(int i=0;i<4;++i)fill(r,{70+i*285,485,260,146},45+i*8);
}
static void details(SDL_Renderer*r,const AppState&s){
    fill(r,{0,0,1280,430},31);fill(r,{0,0,650,430},14);fill(r,{62,82,420,34},215);fill(r,{62,135,300,12},115);fill(r,{62,160,470,10},88);fill(r,{62,180,430,10},78);
    for(int i=0;i<2;++i){SDL_Rect b=detailsButton(i);if(i==s.detailsAction)fill(r,{b.x-4,b.y-4,b.w+8,b.h+8},245);fill(r,b,i==0?210:64);}fill(r,{62,475,200,15},125);for(int i=0;i<4;++i)fill(r,{62+i*290,510,260,145},48+i*7);
}
static void player(SDL_Renderer*r,const AppState&s){fill(r,{48,42,360,16},115);fill(r,{64,620,1152,5},95);fill(r,{64,620,360,5},220);fill(r,{64,646,48,32},s.playerPaused?220:115);fill(r,{124,654,150,8},105);fill(r,{1040,654,176,8},85);}
void render(SDL_Renderer*r,const AppState&s){SDL_SetRenderDrawColor(r,8,8,10,255);SDL_RenderClear(r);if(s.view==View::Player)player(r,s);else if(s.view==View::Details)details(r,s);else if(s.screen==Screen::Search)search(r,s);else browse(r,s);}
}
