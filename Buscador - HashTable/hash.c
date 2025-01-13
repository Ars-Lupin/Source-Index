#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "list.h"

struct HashTable
{
    List **buckets;
    int table_size;
    int num_elems;
    HashFunction hash_fn;
    CmpFunction cmp_fn;
};

struct HashTableIterator
{
    HashTable *h;
    int bucket_idx;
    Node *node;
    int current_elem;
};

HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn)
{
    HashTable *h = (HashTable*) malloc(sizeof(HashTable));

    if (!h) exit(printf("ERRO: falha ao alocar hash table\n"));

    h->buckets = (List**) malloc(sizeof(List*) * table_size);

    if (!h->buckets) exit(printf("ERRO: falha ao alocar hash table\n"));

    for (int i = 0; i < table_size; i++)
        h->buckets[i] = list_construct();

    h->table_size = table_size;
    h->num_elems = 0;
    h->hash_fn = hash_fn;
    h->cmp_fn = cmp_fn;

    return h;
}

void *hash_table_set(HashTable *h, void *key, void *val)
{
    int hash = h->hash_fn(h, key);
    List *l = h->buckets[hash];

    Node *n = list_get_head(l);

    // Se a chave ja existir, atualiza o valor e retorna o valor antigo

    while (n)
    {
        HashTableItem *item = n->value;
        if (h->cmp_fn(item->key, key) == 0)
        {
            void *old_val = item->val;
            item->val = val;
            return old_val;
        }
        n = n->next;
    }

    // Se a chave nao existir, insere um novo item na lista

    HashTableItem *item = (HashTableItem*) malloc(sizeof(HashTableItem));
    item->key = key;
    item->val = val;

    list_push_front(l, item);

    h->num_elems++;

    return NULL;
}

void *hash_table_get(HashTable *h, void *key)
{
    int hash = h->hash_fn(h, key);
    List *l = h->buckets[hash];

    Node *n = list_get_head(l);

    while (n)
    {
        HashTableItem *item = n->value;
        if (h->cmp_fn(item->key, key) == 0)
            return item->val;
        n = n->next;
    }

    return NULL;
}

void *hash_table_pop(HashTable *h, void *key)
{
    int hash = h->hash_fn(h, key);
    List *l = h->buckets[hash];

    Node *n = list_get_head(l);
    Node *prev = NULL;

    while (n)
    {
        HashTableItem *item = n->value;
        if (h->cmp_fn(item->key, key) == 0)
        {
            void *val = item->val;
            if (prev)
                prev->next = n->next;
            else
                list_set_head(l, n->next);

            if (n == list_get_head(l))
                list_set_last(l, prev);

            free(item->key);
            free(item);
            free(n);

            h->num_elems--;

            return val;
        }

        prev = n;
        n = n->next;
    }

    return NULL;
}

int hash_table_size(HashTable *h)
{
    return h->table_size;
}

int hash_table_num_elems(HashTable *h)
{
    return h->num_elems;
}

void hash_table_destroy(HashTable *h)
{
    int size = h->table_size;
    for (int i = 0; i < size; i++)
    {
        List* l = h->buckets[i];
        list_destroy(l);
    }

    free(h->buckets);
    free(h);
}

HashTableIterator *hash_table_iterator(HashTable *h)
{
    HashTableIterator *it = (HashTableIterator*) malloc(sizeof(HashTableIterator));

    if (!it) exit(printf("ERRO: falha ao alocar HashTableIterator\n"));

    it->h = h;
    it->bucket_idx = 0;
    it->node = list_get_head(h->buckets[0]);
    it->current_elem = 0;

    return it;
}

int hash_table_iterator_is_over(HashTableIterator *it)
{
    return it->current_elem >= it->h->num_elems;
}

HashTableItem *hash_table_iterator_next(HashTableIterator *it)
{
    while (it->node == NULL)
    {
        it->bucket_idx++;
        if (it->bucket_idx >= it->h->table_size)
            return NULL;
        it->node = list_get_head(it->h->buckets[it->bucket_idx]);
    }

    HashTableItem *item = it->node->value;
    it->node = it->node->next;
    it->current_elem++;

    return item;
}

void hash_table_iterator_destroy(HashTableIterator *it)
{
    free(it);
}

Vector *hash_to_vector(HashTable *h)
{
    Vector *v = vector_construct(h->num_elems);

    HashTableIterator *it = hash_table_iterator(h);

    while (!hash_table_iterator_is_over(it))
    {
        HashTableItem *item = hash_table_iterator_next(it);
        vector_push_back(v, item);
    }

    hash_table_iterator_destroy(it);

    return v;
}
