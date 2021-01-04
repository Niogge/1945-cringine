#ifndef AIV_LIST_H
#define AIV_LIST_H
#include "aiv-common.h"
struct _aiv_list_node{
    void* __data;
    struct _aiv_list_node* __next;
    struct _aiv_list_node* __previous;
};
typedef struct _aiv_list_node aiv_node;


struct _aiv_list{
    aiv_node* __head;
    aiv_node* __tail;
    uint __count;
};

typedef struct _aiv_list aiv_list;

aiv_list* aiv_list_new();
void aiv_list_destroy(aiv_list* list);
void aiv_list_add(aiv_list* list, void* elem);
boolean aiv_list_is_empty(aiv_list* list);
uint aiv_list_size(aiv_list* list);
void* aiv_list_item_at(aiv_list* list, uint index);
void aiv_list_add_item_at(aiv_list* list, void* elem, uint index);
void aiv_list_remove(aiv_list* list, void* elem);
void aiv_list_remove_at(aiv_list* list, const uint index);
boolean aiv_list_contains(aiv_list* list, void* elem);
int aiv_list_index_of(aiv_list* list, void* elem);
#endif