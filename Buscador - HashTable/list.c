#include "list.h"
#include <stdlib.h>
#include <stdio.h>

struct List
{
    Node *head;
    Node *last;
    int size;
};

struct ListIterator
{
    Node *current;
};

List *list_construct()
{

    List* f = (List*) malloc(sizeof(List));

    if (!f) exit(printf("ERRO: falha ao alocar list\n"));

    f->head = NULL;
    f->last = NULL;
    f->size = 0;

    return f;
}

int list_size(List *l)
{
    if (l) return l->size;
    
    return 0;
}

void list_push_front(List *l, data_type data)
{
    Node* new_node = node_construct(data, NULL, l->head);

    if (l) 
    {
        if (l->head == NULL)
            l->head = l->last = new_node;
        else 
        {
            l->head->prev = new_node;
            l->head = new_node;
        }
        l->size++;
    }
}

void list_push_back(List *l, data_type data)
{
    Node* new_node = node_construct(data, l->last, NULL);

    if (l)
    {
        if (l->head == NULL)
            l->head = l->last = new_node;
        else
        {
            l->last->next = new_node;
            l->last = new_node;
        }
        l->size++;
    }
}

void list_print(List *l, void (*print_fn)(data_type))
{
    Node* current = l->head;  // Ponteiro auxiliar para percorrer a lista

    printf("[");
    for (int i = 0; i < list_size(l); i++)
    {
        print_fn(current->value);
        current = current->next;  // Avança para o próximo nó
        if (i < list_size(l) - 1) printf(", ");
    }
    printf("]");
} 

void list_print_reverse(List *l, void (*print_fn)(data_type))
{
    Node* current = l->last;  // Ponteiro auxiliar para percorrer a lista

    printf("[");
    while(current != NULL)
    {
        print_fn(current->value);
        if (current->prev != NULL) printf(", ");
        current = current->prev;  // Avança para o nó anterior
    }
    printf("]");
}

data_type list_get(List *l, int i)
{

    if (i < 0 || i >= list_size(l)) 
        exit(printf("ERRO: índice fora dos limites\n"));

    Node* node = l->head;
    for (int j = 0; j < i; j++)
        node = node->next;

    return node->value;
}

data_type list_pop_front(List *l)
{

    if (l->head == NULL) exit(printf("ERRO: lista nula\n"));

    Node* node_to_pop = l->head;
    l->head = l->head->next;
    data_type value = node_to_pop->value;
    node_destroy(node_to_pop);
    l->size--;

    if (l->head != NULL)
        l->head->prev = NULL;
    else
        l->last = l->head;

    return value;
}

data_type list_pop_back(List *l)
{
    if (l->head == NULL) exit(printf("ERRO: lista nula\n"));

    Node* node_to_pop = l->last;
    l->last = l->last->prev;
    data_type value = node_to_pop->value;
    node_destroy(node_to_pop);
    l->size--;

    if (l->last != NULL)
        l->last->next = NULL;
    else
        l->head = l->last;

    return value;
}

Node* list_search_prev(List *l, int index){

    Node* curr = l->head;
    Node* prev = NULL;

    for (int count = 0; count < index; count++)
    {
        prev = curr;
        curr = curr->next;
    }

    return prev;
}

data_type list_pop_index(List *l, int index)
{

    if (index < 0 || index >= list_size(l)) 
    {
        printf("INVALID INDEX\n");
        //return NULL;
    }

    Node* to_remove = l->head;
    Node* prev = list_search_prev(l, index);

    if (prev == NULL)
    {
        to_remove = l->head;
        l->head = l->head->next;
    }
    else
    {
        to_remove = prev->next;
        prev->next = to_remove->next;
    }

    data_type value = to_remove->value;
    node_destroy(to_remove);
    l->size--;

    return value;
}

List *list_reverse(List *l)
{
    List *rev_l = list_construct();
    Node* curr = l->head;

    while (curr != NULL)
    {
        list_push_front(rev_l, curr->value);
        curr = curr->next;
    }

    return rev_l;
}

void list_clear(List *l)
{

    int n_items = list_size(l);

    for (int i = 0; i < n_items; i++)
        list_pop_front(l);

    l->size = 0;
}

void list_remove(List *l, data_type val)
{

    Node* curr = l->head;
    Node* prev = NULL;
    Node* new_n = NULL;

    while (curr != NULL)
    {
        if (curr->value == val)
        {
            if (prev == NULL)
                l->head = new_n = curr->next;
            else
                prev->next = new_n = curr->next;

            node_destroy(curr);
            curr = new_n;
            l->size--;
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }
}

void list_cat(List *l, List *m){
    
    Node *curr = m->head;

    while (curr != NULL)
    {
        list_push_front(l, curr->value);   
        curr = curr->next;
    }
}

void list_destroy(List *l)
{

    if (l)
    {
        while (l->head != NULL)
        {
        Node* node_to_destroy = l->head;
        l->head = l->head->next;
        node_destroy(node_to_destroy);
        }

        free(l);
    }
}

void list_sort(List *l)
{
    Node *curr = l->head;
    Node *next = NULL;
    data_type temp;
    int swaped = 1;

    while (swaped)
    {
        swaped = 0;
        curr = l->head;
        next = curr->next;

        while (next != NULL)
        {
            if (curr->value > next->value)
            {
                temp = curr->value;
                curr->value = next->value;
                next->value = temp;
                swaped = 1;
            }

            curr = next;
            next = next->next;
        }
    }
}

ListIterator *list_front_iterator(List *l)
{
    ListIterator *it = (ListIterator*) malloc(sizeof(ListIterator));

    if (!it) exit(printf("ERRO: falha ao alocar ListIterator\n"));

    it->current = l->head;

    return it;
}

ListIterator *list_back_iterator(List *l)
{
    ListIterator *it = (ListIterator*) malloc(sizeof(ListIterator));

    if (!it) exit(printf("ERRO: falha ao alocar ListIterator\n"));

    it->current = l->last;

    return it;
}

data_type *list_iterator_next(ListIterator *it)
{
    if (it->current == NULL) return NULL;

    data_type *value = &(it->current->value);
    it->current = it->current->next;

    return value;
}

data_type *list_iterator_previous(ListIterator *it)
{
    if (it->current == NULL) return NULL;

    data_type *value = &(it->current->value);
    it->current = it->current->prev;

    return value;
}

bool list_iterator_is_over(ListIterator *it)
{
    return it->current == NULL;
}

void list_iterator_destroy(ListIterator *it)
{
    free(it);
}

Node *list_get_head(List *l)
{
    return l->head;
}

void list_set_head(List *l, Node *n)
{
    l->head = n;
}

Node *list_get_last(List *l)
{
    return l->last;
}

void list_set_last(List *l, Node *n)
{
    l->last = n;
}
