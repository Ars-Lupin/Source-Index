#ifndef SEARCH_H
#define SEARCH_H

#include "hash.h"

// Estruturas para armazenar a relevância dos documentos e a frequência das palavras
typedef struct DocumentFreqSearch DocumentFreqSearch;

typedef struct DocumentRelevance DocumentRelevance;

// Funções para busca e manipulação de documentos
void search_documents(const char *index_file, const char *stopwords_file);
int is_stopword(const char *word, char stopwords[100][50]);
void load_stopwords(const char *filename, char stopwords[100][50]);
void sort_documents_by_relevance(DocumentRelevance *docs, int doc_count);
void display_top_documents(DocumentRelevance *docs, int doc_count);

#endif // SEARCH_H
