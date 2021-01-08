#ifndef AIV_VECTOR_H
#define AIV_VECTOR_H
#include <stdlib.h>
#include "aiv-common.h"

struct _aiv_vector{
    void** __items;
    uint __count;
    uint __capacity;
};

typedef struct _aiv_vector aiv_vector;

aiv_vector* aiv_vector_new();
aiv_vector* aiv_vector_new_with_cap(uint capacity);
void aiv_vector_destroy(aiv_vector* vector);
void aiv_vector_add(aiv_vector* vector, void* item);
void* aiv_vector_at(aiv_vector* vector, uint index);
aiv_vector* aiv_vector_resize(aiv_vector* vector, uint capacity);
void aiv_vector_foreach(aiv_vector* vector, void (*action)(void* elem));
void aiv_vector_foreach_dt(aiv_vector* vector, void (*action)(void* elem,float f), float f );
boolean aiv_vector_is_empty(aiv_vector* vector);
void aiv_vector_set(aiv_vector* vector, const uint index, void* elem);
void aiv_vector_add_all(aiv_vector* vector, aiv_vector* other);
void* aiv_vector_remove_at(aiv_vector* vector, const uint index);
uint aiv_vector_size(aiv_vector* vector);
void aiv_vector_insert_at(aiv_vector* vector, const uint index, void* elem);
void* aiv_vector_find_one(aiv_vector* vector, boolean(*func)(void*));
aiv_vector* aiv_vector_find_all(aiv_vector* vector, boolean(*func)(void*));
void aiv_vector_bubble(aiv_vector* vector, int (*comparator)(void*,void*));
void aiv_vector_quick(aiv_vector* vector, int (*comparator)(void*,void*));
void* aiv_vector_pop(aiv_vector* vector);
#endif // AIV_VECTOR_H