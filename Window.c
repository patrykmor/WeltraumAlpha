#include<SDL2/SDL.h>
#include<stdbool.h>
#include<stdio.h>
typedef struct window Window;

#include"Color.h"

#include"Object.h"
#include"ObjectListNode.h"

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
    ret->isClicked = false;
    ret -> isClosed = false;
    return ret;
}
void Window_draw_circle(Window* self, Object* circle){
   // printf("Drawing...\n");
    int midX = circle->x - self->lowerX;
    int midY = circle->y - self->lowerY;
    int x = circle->radius -1;
    int y = 0;
    int tx = 1;
    int ty = 1;
    int diameter = 2 * circle->radius;
    int error = tx - diameter;
    Color color = circle->color;
    SDL_SetRenderDrawColor(self->renderer, color.red, color.green, color.blue, color.alpha);
    //printf("Drawing %d, %d, %d, %d\n", color.red, color.green, color.blue, color.alpha);
    while(x>=y){
        int po=0;
        po+=SDL_RenderDrawLine(self->renderer, midX, midY, midX + x, midY + y); 
        po+=SDL_RenderDrawLine(self->renderer, midX, midY, midX + x, midY - y);
        po+=SDL_RenderDrawLine(self->renderer, midX, midY, midX - x, midY + y);
        po+=SDL_RenderDrawLine(self->renderer, midX, midY, midX - x, midY - y);
        po+=SDL_RenderDrawLine(self->renderer, midX, midY, midX + y, midY + x);
        po+=SDL_RenderDrawLine(self->renderer, midX, midY, midX + y, midY - x);
        po+=SDL_RenderDrawLine(self->renderer, midX, midY, midX - y, midY + x);
        po+=SDL_RenderDrawLine(self->renderer, midX, midY, midX - y, midY - x);
        if(po!=0){
            //printf("ERRRRRRRRRRRRRRRRRROOOOOOOOOOOOOOOOOOOOOR");
        }
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
   //printf("End\n");
}
void Window_process_external_object(Window* self, Object* object){
    //printf("External objects...\n");
    ObjectListNode** current = &self->head;
    while(*current!=NULL){
        Object_calculate_influence(&(*current)->info, object);
        current = &(*current)->next;
    }
    int x = object->x;
    int y = object->y;
    int radius = object->radius;
    bool isX = ((x <= self->upperX) && (x+radius>=self->lowerX))||((x>=self->lowerX) && (x-radius<=self->upperX));
    bool isY = ((y<=self->upperY) && (y+radius>=self->lowerY))||((y>=self->lowerY) && (y-radius<=self->upperY));
    if(x>=self->lowerX && x<self->upperX +1.0 && y >= self->lowerY && y < self->upperY + 1.0){
        List_add(&(self->head), *object);
        printf("GOTCHA!\n");
    }else if(isX&&isY){
        //printf("Responsibility...\n");
        Window_draw_circle(self, object);
        //printf("Drawing %d, %d, %d, %d\n", object->color.red, object->color.green, object->color.blue, object->color.alpha);
    }
    //printf("End\n");
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
                    //printf("Adding to list...");
                    List_add(&self->head, *object);
                    free(object);
                    self->isClicked=false;
                }
                else{
                    object->radius+=1;
                    object->mass= object->radius * object->radius;
                }
            }else{
                if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT){
                    self->isClicked = true;
                    Color* color = Color_create(rand()%255, rand()%255, rand()%255, 255);
                    self->createdObject = Object_create(1, 1, event.button.x+self->lowerX, event.button.y+self->lowerY, ((rand()%20)-10)/4000.0, ((rand()%20)-10)/4000.0, *color);
                    //self->createdObject = Object_create(1, 1, event.button.x+self->lowerX, event.button.y+self->lowerY, 0.001, 0.001, *color);
                    Color_destroy(color);
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
        Object_move(&(*current)->info, self);
        current =&(*current)->next;
    }
}
void Window_process_internal_influences(Window* self){
    //printf("Internal objects...\n");
    ObjectListNode** current = &self->head;
    while(*current!=NULL){
        ObjectListNode** current1 = &self->head;
        while(*current1!=NULL){
            if(*current1==*current){
                current1 =&(*current1)->next;
                continue;
            }
            //printf("Influence..\n");
            Object_calculate_influence(&(*current)->info, &(*current1)->info);
            current1 =&(*current1)->next;
        }
        current =&(*current)->next;
    }
    //printf("End\n");
}
void Window_reset_local_objects(Window* self){
    //printf("Reseting...\n");
    ObjectListNode** current = &self->head;
    while(*current!=NULL){
        Object_reset_acceleration(&(*current)->info);
        current =&(*current)->next;
    }
    //printf("End\n");
}
