#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// Função hash simples para strings (djb2)
unsigned long hash_function(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

// Criação da tabela hash
HashTable* hash_create(size_t size) {
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->buckets = (Node **)calloc(size, sizeof(Node *));
    table->size = size;
    return table;
}

// Destruição da tabela hash e liberação da memória
void hash_destroy(HashTable *table, void (*free_data)(void *)) {
    for (size_t i = 0; i < table->size; i++) {
        Node *node = table->buckets[i];
        while (node) {
            Node *next = node->next;
            free(node->key);
            if (free_data) {
                free_data(node->data);
            }
            free(node);
            node = next;
        }
    }
    free(table->buckets);
    free(table);
}

// Inserção de uma nova chave e dado na tabela hash
void hash_insert(HashTable *table, const char *key, void *data) {
    unsigned long hash = hash_function(key) % table->size;
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->key = strdup(key);
    new_node->data = data;
    new_node->next = table->buckets[hash];
    table->buckets[hash] = new_node;
}

// Busca de uma chave na tabela hash
Node* hash_lookup(HashTable *table, const char *key) {
    unsigned long hash = hash_function(key) % table->size;
    Node *node = table->buckets[hash];
    while (node) {
        if (strcmp((char *)node->key, key) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

// Criação de um iterador para a tabela hash
HashIterator* hash_iterator_create(HashTable *table) {
    HashIterator *it = (HashIterator *)malloc(sizeof(HashIterator));
    it->table = table;
    it->current_bucket = 0;
    it->current_node = NULL;
    return it;
}

// Função para obter o próximo elemento no iterador
Node* hash_iterator_next(HashIterator *it) {
    if (it->current_node) {
        it->current_node = it->current_node->next;
        if (it->current_node) {
            return it->current_node;
        }
    }

    while (it->current_bucket < it->table->size) {
        if (it->table->buckets[it->current_bucket]) {
            it->current_node = it->table->buckets[it->current_bucket];
            it->current_bucket++;
            return it->current_node;
        }
        it->current_bucket++;
    }

    return NULL;
}

// Destruição do iterador da tabela hash
void hash_iterator_destroy(HashIterator *it) {
    free(it);
}

void parse_line(char *buffer, char *doc_id, char **words, int *word_count)
{
    const char *delimiters = " \t\n";
    char *token;
    int count = 0;

    *word_count = 0;

    token = strtok(buffer, delimiters);
    while (token != NULL)
    {
        words[count] = strdup(token);
        count++;

        token = strtok(NULL, delimiters);
    }

    *word_count = count;
}