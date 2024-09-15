#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "index.h"

void index_documents(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Erro ao abrir o arquivo de entrada");
        return;
    }

    HashTable *index = hash_create(1000);
    char buffer[1024];
    char *words[100];
    char doc_id[50];
    int word_count;

    while (fgets(buffer, sizeof(buffer), file))
    {
        parse_line(buffer, doc_id, words, &word_count);

        for (int i = 0; i < word_count; i++)
        {
            char *word = words[i];

            Node *node = hash_lookup(index, word);
            DocumentFreq *doc_freq;

            if (!node)
            {
                doc_freq = create_document_freq(doc_id);
                hash_insert(index, word, doc_freq);
            }
            else
            {
                doc_freq = (DocumentFreq *)node->data;
                update_document_freq(doc_freq, doc_id);
            }

            free(word);  // Liberar memória alocada para a palavra, se necessário
        }
    }

    print_index(index);
    fclose(file);
    hash_destroy(index, free);
}

DocumentFreq *create_document_freq(const char *doc_id)
{
    DocumentFreq *df = (DocumentFreq *)malloc(sizeof(DocumentFreq));
    if (!df)
    {
        perror("Erro ao alocar memória para DocumentFreq");
        exit(1);
    }
    strcpy(df->doc_id, doc_id);
    df->frequency = 1;
    return df;
}

void update_document_freq(DocumentFreq *df, const char *doc_id)
{
    if (strcmp(df->doc_id, doc_id) == 0)
    {
        df->frequency++;
    }
}

void print_index(HashTable *index)
{
    HashIterator *it = hash_iterator_create(index);
    Node *node;

    while ((node = hash_iterator_next(it)) != NULL)
    {
        DocumentFreq *df = (DocumentFreq *)node->data;
        printf("%s %s %d\n", (char *)node->key, df->doc_id, df->frequency);
    }

    hash_iterator_destroy(it);
}
