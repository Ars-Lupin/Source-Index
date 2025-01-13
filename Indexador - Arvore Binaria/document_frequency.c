#include "document_frequency.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A estrutura é um par de documento e a frequência da palavra no documento
struct DocumentFrequency {
    char doc_name[MAX_DOC_NAME];   // Ponteiro para o documento
    int frequency;   // Frequência da palavra no documento
};

DocumentFrequency *doc_freq_construct(char *doc_name)
{
    DocumentFrequency *df = (DocumentFrequency *)malloc(sizeof(DocumentFrequency));

    if (df == NULL) 
        exit(printf("ERRO: Falha ao alocar memória para DocumentFrequency\n"));

    strcpy(df->doc_name, doc_name);      // Copia o nome do documento
    df->frequency = 1;              // Inicialmente, a palavra aparece uma vez no documento

    return df;
}

char* doc_freq_get_name(DocumentFrequency *df)
{
    return df->doc_name;
}

int doc_freq_get_frequency(DocumentFrequency *df)
{
    return df->frequency;
}

void doc_freq_increment(DocumentFrequency *df)
{
    df->frequency++;
}

int cmp_doc_freq(DocumentFrequency* a, DocumentFrequency* b)
{
    return strcmp(a->doc_name, b->doc_name);
}

void doc_freq_destroy(DocumentFrequency *df)
{
    if (df != NULL) 
        free(df); // Libera a estrutura DocumentFrequency
}
