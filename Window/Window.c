#include<SDL2/SDL.h>
#include<stdbool.h>

#include"Object/Object.h"
#include"Color/Color.h"
#include"ObjectList/ObjectListNode.h"

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
    bool isClosed;
    bool isClicked;
    int currentSize;
    Object* createdObject;
    ObjectListNode* head;
    SDL_Window* sdlWindow;
    SDL_Renderer* renderer;
}Window;
Window* Window_create(int rank, int windowsHorizonally, int windowVertically){
    time_t t;
    srand((unsigned) time(&t));
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
    int midX = circle->x - self->lowerX;
    int midY = circle->y - self->lowerY;
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
void Window_process_external_object(Window* self, Object* object){
    ObjectListNode** current = &self->head;
    while(*current!=NULL){
        Object_calculate_influence(&(*current)->info, object);
        current = &(*current)->next;
    }
    int x = object->x;
    int y = object->y;
    int radius = object->radius;
    if(((x<=self->upperX && x+radius>=self->lowerX)||(x>=self->lowerX || x-radius<=self->upperX))&&((y<=self->upperY && y+radius>=self->lowerY)||(y>=self->lowerY || y-radius<=self->upperY))){
        if(x>=self->lowerX && x<=self->upperX && y>= self->lowerY && self<= self->upperY){
            List_add(&(self->head), *object);
        }else{
            Window_draw_circle(self, object);
        }
    }
}
void Window_draw_local_circles(Window* self){
    ObjectListNode** current = &self->head;
    while(*current!=NULL){
        Window_draw_circle(self, &(*current)->info);
        current = &(*current)->next;
    }
    if(self->createdObject!=NULL){
        Window_draw_circle(self, self->createdObject);
    }
}

void Window_process_clicks(Window* self){
    SDL_Event event;
    while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                self -> isClosed = true;
            }
            if(self->isClicked){
                Object* object = self->createdObject;
                if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT){
                    self->createdObject = NULL;
                    List_add(&self->head, *object);
                    free(object);
                }
                else{
                    object->mass+=1;
                    object->radius+=1;
                }
            }else{
                if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                    Color* color = Color_create(rand()%255, rand()%255, rand()%255, 255);
                    self->createdObject = Object_create(1, 1, event.button.x, event.button.y, ((rand()%20)-10)/5.0, ((rand()%20)-10)/5.0, color);
                }
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
void Window_move(Window* self){
    ObjectListNode** current = &self->head;
    while(*current!=NULL){
        Object_move(&(*current)->info);
        current =&(*current)->next;
    }
}
void Window_process_internal_influences(Window* self){
    ObjectListNode** current = &self->head;
    while(*current!=NULL){
        current =&(*current)->next;
        ObjectListNode** current1 = &self->head;
        while(*current1!=NULL){
            if(*current1==*current){
                continue;
            }
            Object_calculate_influence(&(*current)->info, &(*current1)->info);
            current1 =&(*current)->next;
        }
    }
}
