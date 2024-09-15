#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"
#include "hash.h"

struct DocumentFreqSearch
{
    char doc_id[50];
    int frequency;
};

struct DocumentRelevance
{
    char doc_id[50];
    int relevance;
};

void search_documents(const char *index_file, const char *stopwords_file)
{
    FILE *file = fopen(index_file, "r");
    if (!file)
    {
        perror("Erro ao abrir o arquivo de índice");
        return;
    }

    char stopwords[100][50];
    load_stopwords(stopwords_file, stopwords);

    HashTable *index = hash_create(1000);
    char buffer[1024];
    char *words[100];
    char doc_id[50];
    int word_count;

    DocumentRelevance doc_relevance[100] = {0};
    int doc_count = 0;

    while (fgets(buffer, sizeof(buffer), file))
    {
        parse_line(buffer, doc_id, words, &word_count);

        for (int i = 0; i < word_count; i++)
        {
            if (is_stopword(words[i], stopwords))
                continue;

            Node *node = hash_lookup(index, words[i]);
            if (node)
            {
                DocumentFreqSearch *df = (DocumentFreqSearch *)node->data;

                int found = 0;
                for (int j = 0; j < doc_count; j++)
                {
                    if (strcmp(doc_relevance[j].doc_id, df->doc_id) == 0)
                    {
                        doc_relevance[j].relevance += df->frequency;
                        found = 1;
                        break;
                    }
                }

                if (!found)
                {
                    strcpy(doc_relevance[doc_count].doc_id, df->doc_id);
                    doc_relevance[doc_count].relevance = df->frequency;
                    doc_count++;
                }
            }
        }
    }

    sort_documents_by_relevance(doc_relevance, doc_count);
    display_top_documents(doc_relevance, doc_count);

    fclose(file);
    hash_destroy(index, free);
}

int is_stopword(const char *word, char stopwords[100][50])
{
    for (int i = 0; i < 100; i++)
    {
        if (strcmp(stopwords[i], word) == 0)
            return 1;
    }
    return 0;
}

void load_stopwords(const char *filename, char stopwords[100][50])
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Erro ao abrir o arquivo de stop-words");
        return;
    }

    int i = 0;
    char buffer[50];
    while (fgets(buffer, sizeof(buffer), file) && i < 100)
    {
        strtok(buffer, "\n"); // Remove o caractere de nova linha
        strcpy(stopwords[i], buffer);
        i++;
    }

    fclose(file);
}

void sort_documents_by_relevance(DocumentRelevance *docs, int doc_count)
{
    for (int i = 0; i < doc_count - 1; i++)
    {
        for (int j = i + 1; j < doc_count; j++)
        {
            if (docs[i].relevance < docs[j].relevance ||
                (docs[i].relevance == docs[j].relevance && strcmp(docs[i].doc_id, docs[j].doc_id) > 0))
            {
                DocumentRelevance temp = docs[i];
                docs[i] = docs[j];
                docs[j] = temp;
            }
        }
    }
}

void display_top_documents(DocumentRelevance *docs, int doc_count)
{
    printf("Top 10 documentos mais relevantes:\n");
    for (int i = 0; i < doc_count && i < 10; i++)
    {
        printf("%s: %d\n", docs[i].doc_id, docs[i].relevance);
    }
}
