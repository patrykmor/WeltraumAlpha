#include"Color.h"
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
Object* Object_create(long mass, int radius, double x, double y, double xSpeed, double ySpeeed, Color* color);
void Object_move(Object* self);
void Object_reset_acceleration(Object* self);
void Object_calculate_influence(Object* self, Object* object);
void Object_destroy(Object* self);
