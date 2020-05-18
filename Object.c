#include<stdlib.h>
#include<stdbool.h>
#include<stdio.h>
#include<math.h>
#include"Color.h"
typedef struct object
{
    long mass;
    double x;
    double y;
    int radius;
    double xSpeed;
    double ySpeed;
    double xAcceleration;
    double yAcceleration;
    Color color; 
} Object;

#include"ObjectListNode.h"
#include"Window.h"


#define G 0.05


Object* Object_create(long mass, int radius, double x, double y, double xSpeed, double ySpeeed, Color color){
    Object* ret = (Object*) malloc(sizeof(Object));
    ret->mass=mass;
    ret->x=x;
    ret->y=y;
    ret->radius=radius;
    ret->xSpeed=xSpeed;
    ret->ySpeed=ySpeeed;
    ret->xAcceleration=0.0;
    ret->yAcceleration=0.0;
    ret->color=color;
    return ret;
}
void Object_move(Object* self, Window* window){
    //printf("Changing speed: ax=%f, ay=%f\n", self->xAcceleration, self->yAcceleration);
    self->xSpeed+=self->xAcceleration;
    self->ySpeed+=self->yAcceleration;
    double newY = self->y + self->ySpeed;
    int lowerY = window->lowerY+self->radius;
    int upperY = window->upperY-self->radius;
    int lowerX = window->lowerX+self->radius;
    int upperX = window->upperX-self->radius;
    /*if(window->topBoundary && newY  < lowerY){
        self->y=2*lowerY - newY;
        self->ySpeed*=-1;
    }
    else if(window->bottomBoundary && newY > upperY){
        self->y=2*upperY - newY;
        self->ySpeed*=-1;
    }else{*/
        self->y=newY;
    //}
    double newX = self->x + self->xSpeed;
    /*if(window->leftBoundary && newX < lowerX){
        self->x=2*lowerX - newX;
        self->xSpeed*=-1;
    }
    else if(window -> rightBoundary && newX > upperX){
        self->x=2*upperX - newX;
        self->xSpeed*=-1;
    }else{*/
        self->x=newX;
    //}
    //printf("x: %f, y: %f\n", self->x, self->y);
}
void Object_calculate_influence(Object* self, Object* object){
    //gravitationalForce
    double x = self->x - object->x;
    double y = self->y - object->y;
    double dist2 = x*x+y*y;
    double dist = sqrt(dist2);
    if(dist > self->radius + object->radius){
        double a = G*object->mass/(dist*dist);
        double ax = x*a/dist;
        double ay = y*a/dist;
        //printf("ax: %f, ay: %f\n", ax, ay);
        self->xAcceleration-=ax;
        self->yAcceleration-=ay;
        //printf("new ax: %f, ay: %f\n", self->xAcceleration, self->yAcceleration);
    }

    
    //collisions
    /*if(dist<=self->radius+object->radius){
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
        /*if(dist<self->radius+object->radius){
            self->x+=dist-(self->radius+object->radius);
            self->y+=dist-(self->radius+object->radius);
        }
    }*/
}

void Object_reset_acceleration(Object* self){
    //printf("Reset\n");
    self->xAcceleration=0;
    self->yAcceleration=0;
}
void Object_destroy(Object* self){
    free(self);
}
