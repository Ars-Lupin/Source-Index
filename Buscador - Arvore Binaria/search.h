#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "binary_tree.h"
#include "document_frequency.h"
#include "document_relevance.h"
#include "list.h"
#include "vector.h"



#define MAX_QUERY_SIZE 1000

void load_index(BinaryTree *index_tree, const char *file_name);

void load_stopwords(Vector *stopwords, const char *file_name, int (*cmp)(const void*, const void*));

void process_query(BinaryTree *index_tree, Vector *stopwords, 
                    CmpFn cmp_fn,
                    KeyDestroyFn key_destroy_fn,
                    ValDestroyFn val_destroy_fn,
                    int (*cmp)(const void*, const void*));

void consult_processer(BinaryTree *index_tree, Vector *query_words, BinaryTree *doc_relevances);

void consult_destroy(BinaryTree *doc_relevances, Vector* query_words);

void display_top_documents(BinaryTree *doc_relevances);

#endif