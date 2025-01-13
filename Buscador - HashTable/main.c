#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "search.h"

int hash_str(HashTable *h, void *data)
{
    char *str = (char *)data;

    long hash_val = 0;
    int base = 127;

    for (size_t i = 0; i < strlen(str); i++)
        hash_val = (base * hash_val + str[i]) % hash_table_size(h);

    return hash_val;
}

int cmp_str(void *a, void *b)
{
    return strcmp((char *)a, (char *)b);
}

int cmp_string(const void *a, const void *b)
{
    return cmp_str((char *)a, (char *)b);
}

int main()
{
    // Tamanho da tabela hash (mesmo usado no Indexador)
    int table_size = 2011;

    // Criação da tabela hash
    HashTable *index_table = hash_table_construct(table_size, hash_str, cmp_str);

    // Carrega o índice do arquivo
    char index_file[MAX_DOC_NAME];
    scanf("%s", index_file);
    load_index(index_table, index_file);

    // Leitura das stopwords
    Vector *stopwords = vector_construct();

    char stopwords_file[MAX_DOC_NAME];
    scanf("%s", stopwords_file);
    load_stopwords(stopwords, stopwords_file, cmp_string);

    // Leitura e processamento das palavras da consulta
    process_query(index_table, stopwords, cmp_string, hash_str, cmp_str);

    // Destruição da tabela hash de indices
    HashTableIterator *it = hash_table_iterator(index_table);

    while (!hash_table_iterator_is_over(it))
    {
        HashTableItem *item = hash_table_iterator_next(it);
        List *doc_list = item->val;
        char *word = item->key;

        // Destrói a lista de DocumentFrequency
        Node *n = list_get_head(doc_list);
        while (n != NULL)
        {
            DocumentFrequency *df = (DocumentFrequency *)n->value;
            doc_freq_destroy(df); // Certifique-se de que isso está correto
            n = n->next;
        }
        list_destroy(doc_list);
        free(word);
        free(item);
    }
    hash_table_destroy(index_table);
    hash_table_iterator_destroy(it);

    // Destruição das stopwords

    int size = vector_size(stopwords);
    for (int i = 0; i < size; i++)
        free(vector_get(stopwords, i));

    vector_destroy(stopwords);

    return 0;
}
