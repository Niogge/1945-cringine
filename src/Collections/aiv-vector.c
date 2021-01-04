#include "aiv-vector.h"

aiv_vector* aiv_vector_new(){
    return aiv_vector_new_with_cap(10);
}
aiv_vector* aiv_vector_new_with_cap(uint capacity){
    aiv_vector* vector = (aiv_vector*)malloc(sizeof(aiv_vector));
    if(vector == NULL) return NULL;
    vector->__count = 0;
    vector->__capacity = capacity;
    vector->__items = malloc(vector->__capacity * sizeof(void*));
    if(vector->__items == NULL) return NULL;
    return vector;
}
void aiv_vector_destroy(aiv_vector* vector){
    vector->__count = 0;
    free(vector->__items);
    free(vector);
}

void aiv_vector_add(aiv_vector* vector, void* item){

    if(vector->__count == vector->__capacity){
        aiv_vector_resize(vector, vector->__capacity * 2);
    }
    int curr_index = vector->__count;
    vector->__items[curr_index] = item;

    vector->__count++;
}
void* aiv_vector_at(aiv_vector* vector, uint index){
    if(index >= vector->__count){
        return NULL;
    }
    return vector->__items[index];
}

aiv_vector*  aiv_vector_resize(aiv_vector* vector, uint capacity){
    void* ptr = realloc(vector->__items, capacity * sizeof(void*));
    if(ptr == NULL) return NULL;
    vector->__items = ptr;
    vector->__capacity = capacity;
    return vector;
}

void aiv_vector_foreach(aiv_vector* vector, void (*action)(void* elem))
{
    for (unsigned int i = 0; i < vector->__count; i++)
    {
        action(vector->__items[i]);
    }
    
}
void aiv_vector_foreach_dt(aiv_vector* vector, void (*action)(void* elem, float f), float f)
{
    for (unsigned int i = 0; i < vector->__count; i++)
    {
        action(vector->__items[i], f);
    }
    
}
boolean aiv_vector_is_empty(aiv_vector* vector){
    if(vector->__count == 0){
        return true;
    }
    return false;
}

void aiv_vector_set(aiv_vector* vector, const uint index, void* elem){
    if(index >= vector->__count) return;
    vector->__items[index] = elem;
}

void aiv_vector_add_all(aiv_vector* vector, aiv_vector* other){

    int n_of_elements = vector->__count + other->__count;
    if((n_of_elements)>= vector->__capacity){
        vector = aiv_vector_resize(vector,(n_of_elements)*2 );
    }

    for (int i = 0; i < other->__count; i++)
    {
        vector->__items[i+vector->__count] = other->__items[i];
        vector->__count++;
    }
    
}

void* aiv_vector_remove_at(aiv_vector* vector, const uint index){
    if(index >= vector->__count) return NULL;
    void * removedItem = vector->__items[index];
    for (int i = index; i < vector->__count-1; i++)
    {
        vector->__items[i] = vector->__items[i+1];
    }
    vector->__count--;
    if(vector->__count < vector->__capacity/2){
        vector = aiv_vector_resize(vector, vector->__capacity/2);
    }
    return removedItem;
    
}

uint aiv_vector_size(aiv_vector* vector){
    return vector->__count;
}

void aiv_vector_insert_at(aiv_vector* vector, const uint index, void* elem){
    if(index >= vector->__count) return;
    vector->__count++;
    if(vector->__count>vector->__capacity){
        vector = aiv_vector_resize(vector, vector->__capacity*2);
    }
    void* old;
    void* new = elem;
    for (int i = index; i < vector->__count; i++)
    {
        old = vector->__items[i];
        vector->__items[i] = new;
        new = old;
    }
    
}

void* aiv_vector_find_one(aiv_vector* vector, boolean(*func)(void*)){
    for (int i = 0; i < vector->__count; i++)
    {
        if(func(vector->__items[i])){
            return vector->__items[i];
        }
    }
    return NULL;   
}

aiv_vector* aiv_vector_find_all(aiv_vector* vector, boolean(*func)(void*))
{
    aiv_vector* vec = aiv_vector_new();
    for (int i = 0; i < vector->__count; i++)
    {
        if(func(vector->__items[i])){
            aiv_vector_add(vec, vector->__items[i]);
        }
    }
    if(vec->__count >0){
        return vec;
    }
    return NULL;   
}

void aiv_vector_bubble(aiv_vector* vector, int (*comparator)(void*,void*)){
    boolean swap = true;
    while(swap){ // worst case -> O(N)
        swap = false;
        for (int i = 0; i < vector->__count-1; i++) //size -1 * 5 =>  N => O(N)
        {
            if(comparator(vector->__items[i], vector->__items[i+1]) > 0){

                void* temp = vector->__items[i];
                vector->__items[i] = vector->__items[i+1];
                vector->__items[i+1] = temp;
                swap = true;                
            }
        }
        
    }
}// O(N)*O(N) --> O(N^2)
int __aiv_vector_quick_partition(aiv_vector* vector,int (*comparator)(void*,void*),uint start_index,uint end_index){

    int pivot_index = start_index; 
    int left_index = start_index;
    int right_index = end_index;

    void* item  = NULL;
    void* pivot = NULL;
    while( left_index<right_index){

        boolean itemIsGTE = true;
        while(itemIsGTE && pivot_index<right_index){

            item = aiv_vector_at(vector, right_index);
            pivot = aiv_vector_at(vector, pivot_index);
            itemIsGTE = comparator(item, pivot) != -1;
            if(itemIsGTE){
                right_index--;
            }
        }
        if(pivot_index != right_index){
            vector->__items[pivot_index] = item;
            vector->__items[right_index] = pivot;
            pivot_index = right_index;
        }

        boolean itemIsLTE = true;
        while(itemIsLTE && pivot_index > left_index){
            item = aiv_vector_at(vector,left_index);
            pivot = aiv_vector_at(vector, pivot_index);
            itemIsLTE = comparator(item, pivot) != 1;
            if(itemIsLTE){
                left_index++;
            }
        }
        if(pivot_index != left_index){
            vector->__items[pivot_index] = item;
            vector->__items[left_index] = pivot;
            pivot_index = left_index;
        }
    }
    return pivot_index;
}
void __aiv_vector_quick_recurs(aiv_vector* vector,int (*comparator)(void*,void*),uint start_index,uint end_index){
    if(start_index >= end_index) 
        return;  // se si incrociano ritorno 
    int pivot_index = __aiv_vector_quick_partition(vector, comparator, start_index, end_index); //nonostante gli indici so positivi, 
                                                                                                //accettiamo anche indici negativi per non bloccare la funzione dopo
    __aiv_vector_quick_recurs(vector, comparator, start_index, pivot_index-1);
    __aiv_vector_quick_recurs(vector, comparator, pivot_index +1 , end_index);

}
void aiv_vector_quick(aiv_vector* vector, int (*comparator)(void*,void*)){
    __aiv_vector_quick_recurs(vector, comparator, 0, vector->__count -1);
}


/*
int algoritmo(a, b){
    int sum = a+b;  //1
    return sum;     //1  
} //si arrotonda --> O(1) [tempo costante]

void printCount(int num){
    int num2 = 2 * num;             //1  
    for (int i = 0; i < num2; i++) // 2* num 
    {
        int v = i+1;  //1
        print(v);     //1
    } // 1 + 2*num *2  ---> 2*num*2 ---> 4num ---> num  ---> O(N)
    
}*/