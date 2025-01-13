#ifndef _WORD_INDEXER_H_
#define _WORD_INDEXER_H_


#include "hash.h"
#include "list.h"
#include "document_frequency.h"

typedef struct WordIndexer WordIndexer;


WordIndexer *word_indexer_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn);

void word_indexer_add_word(WordIndexer *indexer, char *word, char* doc_name);

void word_indexer_print_file(WordIndexer *indexer);

HashTable* word_indexer_get_table(WordIndexer *indexer);

void word_indexer_destroy(WordIndexer *indexer);

#endif