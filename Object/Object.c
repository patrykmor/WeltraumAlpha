#include<stdlib.h>
#include<stdbool.h>

#include"Window.h"
#include"Color.h"

#define G 0.00000001

typedef struct object
{
    long mass;
    double x;
    double y;
    int radius;
    double xSpeed;
    double ySpeed;
    int xAcceleration;
    int yAcceleration;
    Color* color; 
} Object;
Object* Object_create(long mass, int radius, double x, double y, double xSpeed, double ySpeeed, Color* color){
    Object* ret = (Object*) malloc(sizeof(Object));
    ret->mass=mass;
    ret->x=x;
    ret->y=y;
    ret->radius=radius;
    ret->xSpeed=xSpeed;
    ret->ySpeed=ySpeeed;
    ret->xAcceleration=0;
    ret->yAcceleration=0;
    ret->color=color;
    return ret;
}
void Object_move(Object* self, Window* window){
    self->xSpeed+=self->xAcceleration;
    self->ySpeed+=self->yAcceleration;
    if(window->topBoundary && self->y + self->ySpeed < window->lowerY){
        self->ySpeed*=-1;
    }
    else if(window->bottomBoundary && self->y + self->ySpeed > window->upperY){
        self->ySpeed*=-1;
    }
    if(window->leftBoundary && self->x + self->xSpeed < window->lowerX){
        self->xSpeed*=-1;
    }
    else if(window -> rightBoundary && self->x + self->xSpeed > window->upperX){
        self->xSpeed*=-1;
    }
    self->x+=self->xSpeed;
    self->y+=self->ySpeed;
}
void Object_calculate_influence(Object* self, Object* object){
    //gravitationalForce
    double x = self->x - object->x;
    double y = self->y - object->y;
    double dist2 = x*x+y*y;
    double dist = sqrt(dist2);
    double a = G*object->mass/dist2;
    double ax = x*a/dist;
    double ay = y*a/dist;
    if(x>0){
        self->xAcceleration-=ax;
    }else{
        self->xAcceleration+=ax;
    }
    if(y>0){
        self->yAcceleration-=ay;
    }else{
        self->yAcceleration+=ay;
    }
    //collisions
    if(dist<=self->radius+object->radius){
        //TODO: Calculate collisions
    }
}

void Object_reset_acceleration(Object* self){
    self->xAcceleration=0;
    self->yAcceleration=0;
}
void Object_destroy(Object* self){
    free(self);
}
