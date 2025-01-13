#include "deque.h"
#include <stdlib.h>

struct Deque{

    data_type* arr;
    int start;
    int end;
    int size;
    int allocated;
};

Deque *deque_construct(){

    Deque* deque = (Deque*) calloc(1, sizeof(Deque));
    deque->arr = (data_type*) calloc(1, sizeof(data_type));

    deque->allocated = 1;

    return deque;
}

void deque_push_back(Deque *f, data_type item){

    int idx_source;
    int idx_dest;

    if (f->size >= f->allocated){
        f->allocated *= 2;

        data_type* new_arr = (data_type*) malloc(f->allocated * sizeof(data_type));

        for (int i = 0; i < f->size; i++){
            idx_source = (f->start + i) % f->size;
            idx_dest = i;

            new_arr[idx_dest] = f->arr[idx_source];
        }

        f->start = 0;
        f->end = f->size;
        free(f->arr);
        f->arr = new_arr;
    }

    f->arr[f->end] = item;
    f->end = (f->end + 1) % f->allocated;
    f->size++;
}

void deque_push_front(Deque *f, data_type item){

    int idx_source;
    int idx_dest;

    if (f->size >= f->allocated){
        f->allocated *= 2;

        data_type* new_arr = (data_type*) malloc(f->allocated * sizeof(data_type));

        for (int i = 0; i < f->size; i++){
            idx_source = (f->start + i) % f->size;
            idx_dest = i;

            new_arr[idx_dest] = f->arr[idx_source];
        }

        f->start = 0;
        f->end = f->size;
        free(f->arr);
        f->arr = new_arr;
    }

    if (f->start - 1 < 0)
        f->start = f->allocated;
    
    f->arr[f->start - 1] = item;
    f->start = (f->start - 1) % f->allocated;
    f->size++;
}

data_type deque_pop_back(Deque *f){

    if (f->end - 1 < 0)
        f->end = f->allocated;

    data_type popped = f->arr[f->end - 1];
    f->end = (f->end - 1) % f->allocated;
    f->size--;
    return popped;
}

data_type deque_pop_front(Deque *f){

    data_type popped = f->arr[f->start];
    f->start = (f->start + 1) % f->allocated;
    f->size--;
    return popped;
}

void deque_destroy(Deque *f){

    if (f){
        /*
        int idx;

        for (int i = 0; i < f->size; i++){
            idx = (f->start + i) % f->allocated;
            free(f->arr[idx]);
        }
        */

        free(f->arr);
        free(f);
    }
}

int deque_size(Deque *f){

    return f->size;
}

void* deque_get(Deque *f, int idx){

    if (idx < 0 || idx >= f->size)
        return NULL;

    return f->arr[(f->start + idx) % f->allocated];
}

int deque_find(Deque* f, data_type val, int cmp(const void*, const void*)){

    for (int i = 0; i < f->size; i++){
        if (cmp(val, f->arr[(f->start + i) % f->allocated]) == 1)
            return i;
    }

    return -1;
}
