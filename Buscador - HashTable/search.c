#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"

void load_index(HashTable *index_table, const char *file_name) 
{
    FILE * file = fopen(file_name, "r");

    if (!file) exit(printf("ERRO: nao foi possivel abrir o arquivo %s\n", file_name));

    // Lẽ o número total de palavras no índice
    int total_words;
    fscanf(file, "%d\n", &total_words);

    // Para cada palavra no índice
    for (int i = 0; i < total_words; i++) 
    {
        char word[MAX_WORD_SIZE];
        int num_docs;

        // Lẽ a palavra e o número de documentos que ela aparece
        fscanf(file, "%s %d", word, &num_docs);

        // Cria uma nova lista para armazenar os documentos-frequência
        List *doc_freq_list = list_construct();

        // Lẽ cada documento e sua frequência
        for (int j = 0; j < num_docs; j++) 
        {
            char doc_name[MAX_DOC_NAME];
            int freq;

            fscanf(file, " %s %d", doc_name, &freq);

            // Cria um novo DocumentFrequency e o adiciona à lista
            DocumentFrequency *doc_freq = doc_freq_construct(doc_name);
            doc_freq_set_frequency(doc_freq, freq);
            list_push_front(doc_freq_list, doc_freq);
        }

        // Adiciona a palavra e sua lista de frequências na tabela hash
        // strdup é usada para duplicar as strings de palavra 
        // para garantir que a memória seja gerenciada corretamente
        hash_table_set(index_table, strdup(word), doc_freq_list);
    }

    fclose(file);
}

void load_stopwords(Vector *stopwords, const char *file_name, int (*cmp)(const void*, const void*))
{
    FILE *file = fopen(file_name, "r");

    if (!file) exit(printf("ERRO: nao foi possivel abrir o arquivo %s\n", file_name));

    // Lẽ o número total de stopwords
    int total_stopwords;
    fscanf(file, "%d\n", &total_stopwords);

    // Para cada stopword no arquivo
    for (int i = 0; i < total_stopwords; i++) 
    {
        char word[MAX_WORD_SIZE];
        fscanf(file, "%s", word);

        // Adiciona a stopword ao vetor
        vector_push_back(stopwords, strdup(word));
    }

    fclose(file);
}

void process_query(HashTable *index_table, Vector *stopwords, int (*cmp)(const void*, const void*), 
                    int (*hash_str)(HashTable*, void*), 
                    int (*cmp_str)(void*, void*))
{
    // Lê a consulta do usuário
    char query[MAX_QUERY_SIZE];
    Vector *query_words = vector_construct();

    // Leitura da query (consulta do usuário)
    scanf(" %[^\n]", query);

    // Separar as palavras da consulta
    char *word = strtok(query, " ");
    while (word != NULL) 
    {
        // Se não for uma stopword, adiciona a palavra ao vetor de palavras da consulta
        if (vector_binary_search(stopwords, word, cmp) == -1)
            vector_push_back(query_words, strdup(word));

        word = strtok(NULL, " ");
    }

    // Cria uma tabela hash para armazenar a relevância de cada documento
    HashTable *doc_relevances = hash_table_construct(2011, hash_str, cmp_str);

    // Processa a consulta
    relevance_table_processer(doc_relevances, query_words, index_table);

    // Ordena e exibe os documentos mais relevantes
    display_top_documents(doc_relevances);

    // Destruição do vetor de palavras da consulta
    int query_words_size = vector_size(query_words);
    for (int i = 0; i < query_words_size; i++) 
        free(vector_get(query_words, i));

    vector_destroy(query_words);

    // Destruição da tabela de relevância
    relevance_table_destroy(doc_relevances);
}

void relevance_table_processer(HashTable* doc_relevances, Vector* query_words, HashTable* index_table)
{
    // Para cada palavra da consulta
    for (int i = 0; i < vector_size(query_words); i++) 
    {
        char *word = vector_get(query_words, i);

        // Busca a palavra na tabela hash
        List *doc_freq_list = hash_table_get(index_table, word);

        // Verifica se a palavra foi encontrada e se a lista é válida
        if (doc_freq_list != NULL) 
        {
            // Para cada DocumentFrequency da palavra
            Node *n = list_get_head(doc_freq_list);
            while (n != NULL) 
            {
                DocumentFrequency *doc_freq = (DocumentFrequency *)n->value;
                char *doc_name = doc_freq_get_name(doc_freq);
                int freq = doc_freq_get_frequency(doc_freq);

                // Se o documento já está na tabela de relevância, incrementa a relevância
                int *relevance = hash_table_get(doc_relevances, doc_name);
                if (relevance != NULL) 
                    (*relevance) += freq;
                else 
                {
                    // Se o documento não está na tabela de relevância, adiciona-o
                    relevance = (int *)malloc(sizeof(int));
                    *relevance = freq;
                    hash_table_set(doc_relevances, strdup(doc_name), relevance);
                }

                n = n->next;
            }
        }
    }
}

void relevance_table_destroy(HashTable *doc_relevances) 
{
    HashTableIterator *it = hash_table_iterator(doc_relevances);
    while (!hash_table_iterator_is_over(it)) 
    {
        HashTableItem *item = hash_table_iterator_next(it);
        free(item->key);
        free(item->val);
        free(item);
    }

    hash_table_iterator_destroy(it);
    hash_table_destroy(doc_relevances);
}

void display_top_documents(HashTable *doc_relevances) 
{
    Vector *documents = vector_construct();

    // Converter o hash de relevância para um vetor de documentos
    HashTableIterator *it = hash_table_iterator(doc_relevances);
    while (!hash_table_iterator_is_over(it)) 
    {
        HashTableItem *item = hash_table_iterator_next(it);
        char *doc_name = (char*)item->key;
        int *relevance = (int*)item->val;
        vector_push_back(documents, doc_rel_construct(doc_name, *relevance));
    }
    hash_table_iterator_destroy(it);

    // Ordenar os documentos por relevância e em ordem alfabética se houver empate
    vector_sort(documents, cmp_doc_rel);

    int size = vector_size(documents);
    if (size > 10) size = 10; // Apenas exibe os 10 documentos mais relevantes

    for (int i = 0; i < size; i++) 
    {
        DocumentRelevance *doc = vector_get(documents, i);
        printf("%s %d\n", doc_rel_get_name(doc), doc_rel_get_relevance(doc));
    }

    // Limpar memória
    for (int i = 0; i < size; i++) 
    {
        DocumentRelevance *doc = vector_get(documents, i);
        doc_rel_destroy(doc);
    }
    
    vector_destroy(documents);
}
