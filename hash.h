#ifndef HASH_H
#define HASH_H

#include <stddef.h>

// Definição de um nó para a tabela hash
typedef struct Node {
    void *key;            // Chave da hash
    void *data;           // Dado associado à chave
    struct Node *next;    // Ponteiro para o próximo nó (colisão)
} Node;

// Definição da tabela hash
typedef struct {
    Node **buckets;       // Array de listas (bucket de nós)
    size_t size;          // Tamanho da tabela
} HashTable;

// Definição do iterador da hash
typedef struct {
    HashTable *table;     // Tabela hash
    size_t current_bucket;// Bucket atual
    Node *current_node;   // Nó atual no bucket
} HashIterator;

// Funções para manipulação da tabela hash
HashTable* hash_create(size_t size);
void hash_destroy(HashTable *table, void (*free_data)(void *));
void hash_insert(HashTable *table, const char *key, void *data);
Node* hash_lookup(HashTable *table, const char *key);

// Iterador da tabela hash
HashIterator* hash_iterator_create(HashTable *table);
Node* hash_iterator_next(HashIterator *it);
void hash_iterator_destroy(HashIterator *it);
void parse_line(char *line, char *doc_id, char **words, int *word_count);
#endif
