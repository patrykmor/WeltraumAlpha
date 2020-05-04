#include<SDL2/SDL.h>
#include"ObjectList/ObjectListNode.h"
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
    bool isClosed;
    ObjectListNode* head;
    SDL_Window* sdlWindow;
    SDL_Renderer* renderer;
}Window;
Window* Window_create(int rank, int windowsHorizonally, int windowVertically);
void Window_draw_circle(Window* self, Object* circle);
void Window_draw_local_circles(Window* self);
void Window_process_external_object(Window* self, Object* object);
void Window_clear(Window* self);
void Window_present(Window* self);
void Window_process_clicks(bool* isClosed, bool* isClicked, int* x, int* y);
void Window_move(Window* self);
void Window_process_internal_influences(Window* self);

