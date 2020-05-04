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
    double newY = self->y + self->ySpeed;
    if(window->topBoundary && newY  < window->lowerY){
        self->y=2*window->lowerY - newY;
        self->ySpeed*=-1;
    }
    else if(window->bottomBoundary && newY > window->upperY){
        self->y=2*window->upperY - newY;
        self->ySpeed*=-1;
    }else{
        self->y=newY;
    }
    double newX = self->x + self->xSpeed;
    if(window->leftBoundary && newX < window->lowerX){
        self->x=2*window->lowerX - newX;
        self->xSpeed*=-1;
    }
    else if(window -> rightBoundary && newX > window->upperX){
        self->x=2*window->upperX - newX;
        self->xSpeed*=-1;
    }else{
        self->x+=newX;
    }
    
    
}
void Object_calculate_influence(Object* self, Object* object){
    //gravitationalForce
    double x = self->x - object->x;
    double y = self->y - object->y;
    double dist2 = x*x+y*y;
    double dist = sqrt(dist2);
    if(dist!=0){
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
    }

    
    //collisions
    if(dist<=self->radius+object->radius){
        double v1 = sqrt(self->xSpeed*self->xSpeed + self->ySpeed * self->ySpeed);
        double v2 = sqrt(object->xSpeed*object->xSpeed + object->ySpeed * object->ySpeed);
        double sinPhi = 1;
        double cosPhi = 0;
        if(dist!=0){
            sinPhi = y/dist;
            cosPhi = x/dist;
        }
        double sin01 = 1; 
        double cos01 = 0;
        if(v1!=0){
            sin01 = self->ySpeed/v1; 
            cos01 = self->xSpeed/v1;
        }
        double sin02 = 1; 
        double cos02 = 0;
        if(v2!=0){
            sin02 = object->ySpeed/v2; 
            cos02 = object->xSpeed/v2;
        }
        double t1 = (cos01*cosPhi + sin01*sinPhi)*v1*(self->mass - object->mass);
        double t2 = (cos02*cosPhi + sin02*sinPhi)*2*(object->mass)*v2;
        double t3 = (t1+t2)/(self->mass + object->mass);
        double t4 = (sin01*cosPhi - sinPhi*cos01)*v1;
        
        self->xSpeed = t3*cosPhi + t4*sinPhi;
        self->ySpeed = t3*sinPhi + t4*cosPhi;
    }
}

void Object_reset_acceleration(Object* self){
    self->xAcceleration=0;
    self->yAcceleration=0;
}
void Object_destroy(Object* self){
    free(self);
}
