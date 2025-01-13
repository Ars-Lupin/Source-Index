#ifndef _WORD_INDEXER_H_
#define _WORD_INDEXER_H_

#include "binary_tree.h"
#include "list.h"
#include "deque.h"
#include "document_frequency.h"

typedef struct WordIndexer WordIndexer;

WordIndexer *word_indexer_construct(CmpFn cmp_fn, 
                                    KeyDestroyFn key_destroy_fn, 
                                    ValDestroyFn val_destroy_fn);
                        
void word_indexer_add_word(WordIndexer *indexer, char *word, char* doc_name);

void word_indexer_print_file(WordIndexer *indexer);

BinaryTree* word_indexer_get_binary_tree(WordIndexer *indexer);

void word_indexer_destroy(WordIndexer *indexer);

#endif