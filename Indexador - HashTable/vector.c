#include <stdio.h>
#include <stdlib.h>
#include "vector.h"


struct Vector {
    data_type *data;
    int size;
    int allocated;
};

Vector *vector_construct(){

    Vector *vetor = (Vector*) calloc(1, sizeof(Vector));
    vetor->allocated = 1;

    if (!vetor){
        printf("ERRO: falha na alocacao do vector");
        exit(1);
    }

    vetor->data = (data_type*) calloc(1, sizeof(data_type));

    if (!vetor->data){
        printf("ERRO: falha na alocacao dos dados de vector");
        exit(1);
    }

    return vetor;
}

void vector_push_back(Vector *v, data_type val){

    if (v->size >= v->allocated){
        v->allocated *= 2;
        v->data = (data_type*) realloc(v->data, v->allocated * (sizeof(data_type)));
    }

    v->data[v->size] = val;
    v->size++;
}

int vector_size(Vector *v){

    if (v)
        return v->size;
    
    return 0;
}

data_type vector_get(Vector *v, int i){

    if (i >= 0 && i < vector_size(v))
        return v->data[i];

    printf("ERRO: indice invalido");
    exit(1);
}

void vector_set(Vector *v, int i, data_type val){

    if (i >= 0 && i < vector_size(v)){
        v->data[i] = val;
    } else {
        printf("ERRO: indice invalido");
        exit(1);
    }
}

int vector_find(Vector *v, data_type val, int cmp(const void*, const void*)){

    for (int i = 0; i <= vector_size(v) - 1; i++){
        if (cmp(v->data[i], val) == 1)
            return i;
    }

    return -1;
}

void vector_destroy(Vector *v){

    if (v){
        if (v->data){
            /*for (int i = 0; i < vector_size(v); i++){
                if (v->data[i]) destroy_data_type(v->data[i]);
            }*/
            free(v->data);
        }
        free(v);
    }
}

data_type vector_max(Vector *v, int cmp(const void*, const void*)){

    data_type max = v->data[0];

    for (int i = 1; i < vector_size(v); i++){

        if (cmp(max, v->data[i]) < 0)
            max = v->data[i];
    }

    return max;
}

data_type vector_min(Vector *v, int cmp(const void*, const void*)){

    data_type min = v->data[0];

    for (int i = 1; i < vector_size(v); i++){

        if (cmp(min, v->data[i]) > 0)
            min = v->data[i];
    }

    return min;
}

int vector_argmax(Vector *v, int cmp(const void*, const void*)){

    return vector_find(v, vector_max(v, cmp), cmp);
}

int vector_argmin(Vector *v, int cmp(const void*, const void*)){

    return vector_find(v, vector_min(v, cmp), cmp);
}

data_type vector_remove(Vector *v, int i){

    data_type elem_removido = v->data[i];

    for (; i < vector_size(v) - 1; i++){
        v->data[i] = v->data[i + 1];
    }

    v->size--;

    return elem_removido;
}

data_type vector_pop_front(Vector *v){

    data_type front_element = vector_remove(v, 0);

    return front_element;
}

data_type vector_pop_back(Vector *v){

    data_type back_element = vector_remove(v, v->size - 1);

    return back_element;
}

void vector_insert(Vector *v, int i, data_type val){

    if (vector_size(v) == v->allocated){
        v->allocated++;
        v->data = (data_type*) realloc(v->data, v->allocated * (sizeof(data_type)));
    }

    for (int j = v->size; j > i; j--){
        v->data[j] = v->data[j - 1];
    }

    v->data[i] = val;
    v->size++;
}

void vector_swap(Vector *v, int i, int j){

    data_type aux = v->data[i];
    v->data[i] = v->data[j];
    v->data[j] = aux;
}

void vector_sort(Vector *v, int (cmp_fn)(const void* prod1, const void* prod2)){

    int swapped;

    while (1){

        swapped = 0; // Nao houve trocas ainda

        for (int j = 0; j < vector_size(v) - 1; j++){

            if (cmp_fn(v->data[j], v->data[j + 1]) > 0){
                vector_swap(v, j, j + 1);
                swapped = 1;
            }
        }

        if (!swapped) // Se nao houve trocas, nao precisa ordenar mais
            break;
    }
}

 int vector_binary_search(Vector *v, data_type val){

    int idx_min = 0, idx_max = vector_size(v) - 1, idx_mid;

    while (idx_min <= idx_max){

        idx_mid = (idx_min + idx_max) / 2;

        if (v->data[idx_mid] == val){
            return idx_mid;
        } else if (val < v->data[idx_mid]){
            idx_max = idx_mid - 1;
        } else if (val > v->data[idx_mid]){
            idx_min = idx_mid + 1;
        }
    }

    return -1;
}

void vector_reverse(Vector *v){

    if (vector_size(v) == 1)
        return;

    int i = 0, j = vector_size(v) - 1;

    while(1){

        vector_swap(v, i, j);
        i++;

        if (i >= j)
            break;

        j--;
    }
}
