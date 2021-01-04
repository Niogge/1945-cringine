#include "aiv-list.h"
#include <stdlib.h>

aiv_list* aiv_list_new(){
    aiv_list* list = (aiv_list*) malloc(sizeof(aiv_list));
    list->__head = NULL;
    list->__tail = NULL;
    list->__count =0;
    return list;
}

void aiv_list_destroy(aiv_list* list){
    aiv_node* each = list->__head;
    while(each){
        aiv_node* next = each->__next;
        free(each);
        each = next;
    }
    free(list);
}


void aiv_list_add(aiv_list* list, void* elem){
    aiv_node* node = (aiv_node*) malloc(sizeof(aiv_node));
    node->__data = elem;
    node->__next = NULL;
    node->__previous = NULL;
    
    if(list->__head == NULL){
        list->__head = node;
        list->__tail = node;
    }
    else{
        node->__previous = list->__tail;
        list->__tail->__next = node;
        list->__tail = node;
    }
    list->__count++;

}

boolean aiv_list_is_empty(aiv_list* list){
    if(list->__head == NULL){
        return true;
    }
    return false;
}

uint aiv_list_size(aiv_list* list){
    return list->__count;
}

void* aiv_list_item_at(aiv_list* list, uint index){
    if(index >= list->__count){
        return NULL;
    }

    aiv_node* elem = list->__head;
    for (uint i = 0; i < index; i++)
    {
        elem = elem->__next;
    }

    return elem->__data;
    
}
void aiv_list_add_item_at(aiv_list* list, void* elem, uint index){
   

    aiv_node* node = (aiv_node*) malloc(sizeof(aiv_node));
    node->__data = elem;
    node->__next = NULL;
    node->__previous = NULL;

    if(index >= list->__count){
        aiv_list_add(list,elem);
    }
    else if(index == 0){
        node->__next = list->__head;
        list->__head = node;
        list->__count++;
    }
    else{
        aiv_node* elem = list->__head;
        for (uint i = 0; i < index-1; i++)
        {
            elem = elem->__next;
        }
        elem->__next->__previous = node;
        elem->__next = node;
        list->__count++;

    }
}
void aiv_list_remove(aiv_list*list, void* elem){

    if( list->__head->__data == elem){
        aiv_list_remove_at(list,0);
        return;
    }
    if( list->__tail->__data == elem){
        aiv_list_remove_at(list, list->__count-1);
        return;
    }
    aiv_node* node_elem = list->__head->__next;
    while(node_elem != NULL){

        if(node_elem->__data == elem){
            node_elem->__previous->__next = node_elem->__next;
            node_elem->__next->__previous = node_elem->__previous;
            free(node_elem);
            list->__count --;
            return;
        }
        node_elem = node_elem->__next;
    }
    
}


void aiv_list_remove_at(aiv_list*list, const uint index){
    if(index >= list->__count) return;

    if(index == 0){
        aiv_node* old_head = list->__head;
        list->__head = list->__head->__next;
        list->__head->__previous = NULL;

        free(old_head);
        list->__count --;
        return;
    }else if( index == list->__count-1){
        aiv_node* old_tail = list->__tail;
        list->__tail = list->__tail->__previous;
        list->__tail->__next =NULL;

        free(old_tail);
        list->__count --;
        return;
    }
    else{
        aiv_node* elem = list->__head;
        for (int i = 0; i < index; i++)
        {
            elem = elem->__next;
        }
        elem->__previous->__next = elem->__next;
        elem->__next->__previous = elem->__previous;
        free(elem);
        list->__count--;
        return;
    }

}

boolean aiv_list_contains(aiv_list* list, void* elem){
    aiv_node* item = list->__head;
    while(item != NULL){
        if(item->__data == elem){
            return true;
        }
        item = item->__next;
    }
    return false;
}

int aiv_list_index_of(aiv_list* list, void* elem){
    aiv_node* item = list->__head;
    int index =0;
    while(item != NULL){
        if(item->__data == elem){
            return index;
        }
        item = item->__next;
        index++;
    }
    return -1;
}