#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "document_relevance.h"

struct DocumentRelevance
{
    char doc_name[MAX_DOC_NAME];
    int relevance;
};

DocumentRelevance *doc_rel_construct(char *doc_name, int relevance)
{
    DocumentRelevance *dr = (DocumentRelevance *)malloc(sizeof(DocumentRelevance));

    if (dr == NULL)
        exit(printf("ERRO: Falha ao alocar memória para DocumentRelevance\n"));

    strcpy(dr->doc_name, doc_name);
    dr->relevance = relevance;

    return dr;
}

int cmp_doc_rel(const void *a, const void *b)
{
    DocumentRelevance *doc_a = (DocumentRelevance *)a;
    DocumentRelevance *doc_b = (DocumentRelevance *)b;

    // Comparação pela relevância (ordem decrescente)
    if (doc_a->relevance > doc_b->relevance) {
        return -1;  // Retorna negativo para colocar doc_a antes de doc_b
    } else if (doc_a->relevance < doc_b->relevance) {
        return 1;   // Retorna positivo para colocar doc_b antes de doc_a
    } else {
        // Se a relevância for igual, compara pelo nome do documento (ordem alfabética)
        return strcmp(doc_a->doc_name, doc_b->doc_name);
    }
}

int doc_rel_get_relevance(DocumentRelevance *dr)
{
    return dr->relevance;
}

char *doc_rel_get_name(DocumentRelevance *dr)
{
    return dr->doc_name;
}

void doc_rel_destroy(DocumentRelevance *dr)
{
    if (dr != NULL)
        free(dr);
}
