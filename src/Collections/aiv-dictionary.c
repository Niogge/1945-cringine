#include "aiv-dictionary.h"
#include <stdlib.h>
#include "sprite.h"

uint djb33x_hash(void* key, uint keylen){
    char* k = (char * )key;
    int hash = 5381;//seed 
    char* key_as_num = (char*)key; //key in byte array
    for (int i = 0; i < keylen; i++)
    {
        hash = ((hash << 5 ) + hash) ^ key_as_num[i]; //lshift = moltiplication
    }
    return hash;
}

aiv_dict* aiv_dict_new(){
    return aiv_dict_new_capacity(10);
}
aiv_dict* aiv_dict_new_capacity(int capacity){
    aiv_dict* dict = (aiv_dict*)malloc(sizeof(aiv_dict));
    dict->__capacity = capacity;
    dict->__hasher = djb33x_hash;
    dict->__hashmap = (dict_node**)calloc(sizeof(dict_node*),dict->__capacity);

    dict->__count = 0;
    return dict;
}
aiv_dict* aiv_dict_new_hash(uint (*hashfunct)(void*, uint)){
    aiv_dict* dict = (aiv_dict*)malloc(sizeof(aiv_dict));
    dict->__capacity = 10;
    dict->__hasher = hashfunct;
    dict->__hashmap = (dict_node**)calloc(sizeof(void*),dict->__capacity);
    dict->__count = 0;

    return dict;
}
void aiv_dict_destroy(aiv_dict* dict){
    for (int i = 0; i < dict->__capacity; i++)
    {
        dict_node* curr = dict->__hashmap[i];
        while (curr != NULL)
        {
            dict_node* next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(dict->__hashmap);
    free(dict);
    
}


void _aiv_dict_resize(aiv_dict* dict, int new_cap){
    aiv_dict* newDict = aiv_dict_new_capacity(new_cap);
    for (int i = 0; i < dict->__capacity; i++)
    {
        dict_node* curr = dict->__hashmap[i];
        while(curr!=NULL){
            aiv_dict_put(newDict,curr->key,curr->keylen,curr->data);
            curr = curr->next;
        } 
    }
    //dict = newDict;
 
    for (int i = 0; i < dict->__capacity; i++)
    {
        dict_node* curr = dict->__hashmap[i];
        while (curr != NULL)
        {
            dict_node* next = curr->next;
            free(curr);
            curr = next;
        }
    }
    free(dict->__hashmap);
    dict->__capacity = newDict->__capacity;
    dict->__count = newDict->__count;
    dict->__hasher = newDict->__hasher;
    dict->__hashmap = newDict->__hashmap;
    free(newDict);
}
void aiv_dict_put(aiv_dict* dict, void* key, uint keylen,  void* val){
    uint hash = dict->__hasher(key,keylen);
    uint hash_index= hash%dict->__capacity;
    if(dict->__hashmap[hash_index]==NULL){//hash(key) not present
        dict_node * node = malloc(sizeof(dict_node));
        node->key = malloc(sizeof(keylen));
        memcpy_s(node->key, keylen, key, keylen);
        //We need this memcpy because few lines ahead we will do a memcmp.
        //But why? if we just stored the pointer to the key this would need us access
        //the key based on it's addess, which is stuff we cannot control.
        //doing this memcpy we ensure that if we pass an identical object it gets 
        //recognized, no matter where it's stored.
        node->keylen = keylen;
        node->data = val;
        node->next= NULL;
        dict->__hashmap[hash_index] = node;
        Texture* d =(Texture*) dict->__hashmap[hash_index]->data;
        puts("");
    }
    else{ //hash(key) exists
        dict_node * currnode = dict->__hashmap[hash_index];
        dict_node * lastnode;
        while(currnode != NULL){
            if(keylen==currnode->keylen){
                if(memcmp(key,currnode->key,keylen)==0){
                    //same key, substitute
                    currnode->data = val;
                    return;
                }
            }
            lastnode = currnode;
            currnode = currnode->next;
        }
        //no key found 
        dict_node * node = malloc(sizeof(dict_node));
        node->key = malloc(sizeof(keylen));
        memcpy_s(node->key, keylen, key, keylen);
        node->keylen = keylen;
        node->data = val;
        node->next= NULL;
        lastnode->next = node;
    }
    dict->__count ++;
    if(dict->__count == dict->__capacity){
        _aiv_dict_resize(dict, dict->__capacity*2);
    }
}

boolean aiv_dict_is_empty(aiv_dict* dict){
    return dict->__count==0;
}

uint aiv_dict_size(aiv_dict* dict){
    return dict->__count;
}

void* aiv_dict_get(aiv_dict* dict, void*key, uint keylen){
    uint hash = dict->__hasher(key,keylen);
    uint hash_index= hash%dict->__capacity;
    dict_node* curr = dict->__hashmap[hash_index];
    while(curr != NULL){
        if(keylen == curr->keylen){
            Texture * d = (Texture *) curr->data;
            char * ck = (char *) curr->key;
            char *k = (char*)key;
            int m = memcmp(curr->key,key,keylen);
            if(m== 0){
                
                return curr->data;
            }
        }
        curr= curr->next;
    }
    return NULL;
}

void aiv_dict_remove(aiv_dict* dict, void* data){
    for (int i = 0; i < dict->__capacity; i++)
    {
        dict_node* curr = dict->__hashmap[i];
        dict_node* prev = NULL;
        while(curr!=NULL){
            if(data == curr->data){
                if(prev == NULL)//first node
                {
                    dict->__hashmap[i] = curr->next;
                    
                }
                else{
                    prev->next= curr->next;
                    
                }
                curr->data = NULL;
                free(curr);
                dict->__count--;
                return;
            }
            prev = curr;
            curr= curr->next;
        }
    }
    
}


boolean aiv_dict_contains_key(aiv_dict* dict, void *key, uint keylen){
    for (int i = 0; i < dict->__capacity; i++)
    {
         dict_node* curr = dict->__hashmap[i];
        while(curr != NULL){
            if((curr->keylen == keylen) && (memcmp(curr->key,key,keylen)==0)){
                return true;
            }
            curr = curr->next;
        }
    }
    return false;
}