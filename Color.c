#include<stdlib.h>
typedef struct color{
    int red;
    int green;
    int blue;
    int alpha;
}Color;

Color* Color_create(int red, int green, int blue, int alpha){
    Color* ret = (Color*) malloc (sizeof(Color));
    ret->red = red;
    ret->green = green;
    ret->blue = blue;
    ret->alpha = alpha;
    return ret;
}
void Color_destroy(Color* self){
    free(self);
}