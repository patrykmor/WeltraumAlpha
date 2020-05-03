#include<SDL2/SDL.h>
#include<stdbool.h>

#include"Object.h"
#include"Color.h"

#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 480

typedef struct window
{
    int lowerX; 
    int upperX;
    int lowerY;
    int upperY;
    bool bottomBoundary;
    bool topBoundary;
    bool leftBoundary;
    bool rightBoundary;
    SDL_Window* sdlWindow;
    SDL_Renderer* renderer;
}Window;
Window* Window_create(int rank, int windowsHorizonally, int windowVertically){
    Window* ret  = (Window*) malloc(sizeof(Window));
    SDL_Init(SDL_INIT_VIDEO);
    int posX = rank%windowsHorizonally;
    int posY = rank/windowsHorizonally;
    char title[20];
    sprintf(title, "%d, %d", posX, posY);
    ret->sdlWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    ret->renderer = SDL_CreateRenderer(ret->sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    ret->lowerX = posX*WINDOW_WIDTH;
    ret->upperX = ret->lowerX + WINDOW_WIDTH -1;
    ret->lowerY = posY * WINDOW_HEIGHT;
    ret->upperY = ret->lowerY + WINDOW_HEIGHT -1;
    ret->topBoundary = ( posY == 0 );
    ret->bottomBoundary = ( posY == windowVertically -1 );
    ret->leftBoundary = ( posX == 0 );
    ret->rightBoundary = ( posX == windowsHorizonally -1 );
    return ret;
}
void Window_draw_circle(Window* self, Object* circle){
    int midX = circle->x;
    int midY = circle->y;
    int x = circle->radius -1;
    int y = 0;
    int tx = 1;
    int ty = 1;
    int diameter = 2 * circle->radius;
    int error = tx - diameter;
    Color color = *(circle->color);
    SDL_SetRenderDrawColor(self->renderer, color.red, color.green, color.blue, color.alpha);
    while(x>=y){
        SDL_RenderDrawLine(self->renderer, midX, midY, midX + x, midY + y);
        SDL_RenderDrawLine(self->renderer, midX, midY, midX + x, midY - y);
        SDL_RenderDrawLine(self->renderer, midX, midY, midX - x, midY + y);
        SDL_RenderDrawLine(self->renderer, midX, midY, midX - x, midY - y);
        SDL_RenderDrawLine(self->renderer, midX, midY, midX + y, midY + x);
        SDL_RenderDrawLine(self->renderer, midX, midY, midX + y, midY - x);
        SDL_RenderDrawLine(self->renderer, midX, midY, midX - y, midY + x);
        SDL_RenderDrawLine(self->renderer, midX, midY, midX - y, midY - x);
        if(error<=0){
            y++;
            error+=ty;
            ty+=2;
        }
        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}
void Window_clear(Window* self){
    SDL_SetRenderDrawColor(self->renderer, 0, 0, 0, 255);
    SDL_RenderClear(self->renderer);
}
void Window_present(Window* self){
    SDL_RenderPresent(self->renderer);
}