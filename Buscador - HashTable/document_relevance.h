#ifndef _DOCUMENT_RELEVANCE_H_
#define _DOCUMENT_RELEVANCE_H_


#include "document_frequency.h"

typedef struct DocumentRelevance DocumentRelevance;


DocumentRelevance *doc_rel_construct(char *doc_name, int relevance);

int cmp_doc_rel(const void *a, const void *b);

int doc_rel_get_relevance(DocumentRelevance *dr);
char *doc_rel_get_name(DocumentRelevance *dr);

void doc_rel_destroy(DocumentRelevance *dr);

#endif