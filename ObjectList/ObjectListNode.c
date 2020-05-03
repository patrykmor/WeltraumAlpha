#include<stdlib.h>
#include"Object.h"

typedef struct node
{
    struct node* prev;
    Object info;
    struct node* next;
}ObjectListNode;

void List_add(ObjectListNode** self, Object info){
    ObjectListNode* new_node;
    ObjectListNode* old_prev;
    new_node=(ObjectListNode*) malloc(sizeof(ObjectListNode));
    new_node->info=info;
    new_node->next= *self;
    if(*self!=NULL){
        (*self)->prev=new_node;
    }
    *self = new_node;
}
void List_remove(ObjectListNode** self){
    ObjectListNode* next_node;
    ObjectListNode* node=*self;
    next_node=node->next;
    if(next_node!=NULL){
        next_node->prev=node->prev;
    }
    free(*self);
    *self=next_node;
}
