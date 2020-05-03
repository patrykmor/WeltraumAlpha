#include"Object.h"
typedef struct node
{
    struct node* prev;
    Object info;
    struct node* next;
}ObjectListNode;

void List_add(ObjectListNode** self, Object info);
void List_remove(ObjectListNode** self);
