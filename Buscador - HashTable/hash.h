
#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

typedef struct HashTable HashTable;

typedef int (*HashFunction)(HashTable *, void *);
typedef int (*CmpFunction)(void *k1, void *k2);

typedef struct
{
    void *key;
    void *val;
} HashTableItem;

typedef struct HashTableIterator HashTableIterator;

#include "vector.h"

// constroi a hash
HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn);

// funcao para insercao/atualizacao de pares chave-valor em O(1).
// Se a chave ja existir, atualiza o valor e retorna o valor antigo para permitir desalocacao.
void *hash_table_set(HashTable *h, void *key, void *val);

// retorna o valor associado com a chave key ou NULL se ela nao existir em O(1).
void *hash_table_get(HashTable *h, void *key);

// remove o par chave-valor e retorna o valor ou NULL se nao existir tal chave em O(1).
void *hash_table_pop(HashTable *h, void *key);

// numero de buckets
int hash_table_size(HashTable *h);

// numero de elementos inseridos
int hash_table_num_elems(HashTable *h);

// libera o espaco alocado para a tabela hash
void hash_table_destroy(HashTable *h);

// cria um novo iterador para a tabela hash
HashTableIterator *hash_table_iterator(HashTable *h);

// retorna 1 se o iterador chegou ao fim da tabela hash ou 0 caso contrario
int hash_table_iterator_is_over(HashTableIterator *it);

// retorna o proximo par chave valor da tabela hash
HashTableItem *hash_table_iterator_next(HashTableIterator *it);

// desaloca o iterador da tabela hash
void hash_table_iterator_destroy(HashTableIterator *it);

Vector *hash_to_vector(HashTable *h);

#endif
