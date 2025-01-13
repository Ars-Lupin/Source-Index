
#ifndef _DEQUE_H_
#define _DEQUE_H_

#include "vector.h"

typedef void* data_type;
typedef struct Deque Deque;

Deque *deque_construct();
void deque_push_back(Deque *f, data_type item);
void deque_push_front(Deque *f, data_type item);
void* deque_pop_back(Deque *f);
void* deque_pop_front(Deque *f);
void deque_destroy(Deque *f);
int deque_size(Deque *f);
void* deque_get(Deque *f, int idx);
int deque_find(Deque* f, data_type val, int cmp(const void*, const void*));


#endif