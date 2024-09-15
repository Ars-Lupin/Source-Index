#ifndef INDEX_H
#define INDEX_H

#include "hash.h"

typedef struct {
    char doc_id[50];
    int frequency;
} DocumentFreq;

void index_documents(const char *input_file);

DocumentFreq* create_document_freq(const char *doc_id);

void update_document_freq(DocumentFreq *df, const char *doc_id);

void print_index(HashTable *index);


#endif
