#ifndef AIV_DICT_H
#define AIV_DICT_H
#include "aiv-common.h"
#include <string.h>



typedef struct _aiv_dict_node dict_node;
struct _aiv_dict_node{
    void* key;
    uint keylen;
    void* data;
    dict_node* next;
};
//it's better to use a linked list so we can have better insert and remove, easier than a vector
//we are basically keeping an array of smaller linked lists, so we get the flexibility of a linked list
//but we can easy access elements ALMOST directly with keys (if we have low collisions)
typedef struct{
    dict_node** __hashmap;
    int __capacity;
    uint (*__hasher)(void*, uint);
    uint __count;
} aiv_dict;

aiv_dict* aiv_dict_new();
aiv_dict* aiv_dict_new_capacity(int capacity);
aiv_dict* aiv_dict_new_hash(uint (*hashfunct)(void*, uint));
void aiv_dict_put(aiv_dict* dict, void* key, uint keylen, void* val);
void aiv_dict_destroy(aiv_dict* dict);
boolean aiv_dict_is_empty(aiv_dict* dict);
uint aiv_dict_size(aiv_dict* dict);
void* aiv_dict_get(aiv_dict* dict, void*key, uint keylen);
void aiv_dict_remove(aiv_dict* dict, void* data);
boolean aiv_dict_contains_key(aiv_dict* dict, void *key, uint keylen);
#endif