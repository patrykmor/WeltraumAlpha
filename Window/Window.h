#include<SDL2/SDL.h>
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
Window* Window_create(int rank, int windowsHorizonally, int windowVertically);
void Window_draw_circle(Object* circle);
void Window_clear(Window* self);
void Window_present(Window* self);
//TODO: process clicks
