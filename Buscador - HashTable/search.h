#ifndef _SEARCH_H_
#define _SEARCH_H_


#include "hash.h"
#include "document_frequency.h"
#include "document_relevance.h"
#include "list.h"
#include "vector.h"


#define MAX_QUERY_SIZE 1000


void load_index(HashTable *index_table, const char *file_name);

void load_stopwords(Vector *stopwords, const char *file_name, int (*cmp)(const void*, const void*));

void process_query(HashTable *index_table, Vector *stopwords, 
                    int (*cmp)(const void*, const void*), 
                    int (*hash_str)(HashTable*, void*), 
                    int (*cmp_str)(void*, void*));

void relevance_table_processer(HashTable* doc_relevances, Vector* query_words, HashTable* index_table);

void relevance_table_destroy(HashTable *doc_relevances);

void display_top_documents(HashTable *doc_relevances);

#endif