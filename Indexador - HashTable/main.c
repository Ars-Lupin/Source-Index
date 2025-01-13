#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "list.h"
#include "document_frequency.h"
#include "vector.h"
#include "WordIndexer.h"

#define MAX_DIRECTORY 1000

int hash_str(HashTable *table, void *key)
{
    char *str = key;

    int table_size = hash_table_size(table);

    unsigned long hash_val = 0, base = 27183;
    unsigned long random_val = 31415;

    int size = strlen(str);

    for (int i = 0; i < size; i++)
    {
        hash_val = (hash_val * random_val + str[i]) % table_size;

        random_val = (random_val * base) % (table_size - 1);
    }

    return hash_val;
}

int cmp_str(void *a, void *b)
{
    return strcmp((char *)a, (char *)b);
}

int cmp_string(const void *a, const void *b)
{
    return cmp_str(((HashTableItem *)a)->key, ((HashTableItem *)b)->key);
}

#define MAX_DOC_NAME 100

int main() 
{

    char dir[MAX_DIRECTORY];
    scanf("%s", dir);
    FILE *file = fopen(dir, "r");

    if (file == NULL) 
        exit(printf("ERRO: nao foi possivel abrir o arquivo\n"));

    // Lê o número de documentos
    int qtd_docs;
    fscanf(file, "%d\n", &qtd_docs);

    // Tamanho da tabela hash
    int table_size = 919;

    // Criação do indexador de palavras
    WordIndexer *indexer = word_indexer_construct(table_size, hash_str, cmp_str);

    // Processa cada documento
    for (int i = 0; i < qtd_docs; i++) 
    {
        // Processa cada documento
        char doc_name[MAX_DOC_NAME];
        int qtd_words;

        fscanf(file, "%[^:]: %d: ", doc_name, &qtd_words);

        // Processa cada palavra do documento
        for (int k = 0; k < qtd_words; k++)
        {
            char word[MAX_DOC_NAME];
            fscanf(file, "%s", word);
            word_indexer_add_word(indexer, word, doc_name); // Adiciona a palavra ao indexador
        }

        fscanf(file, "\n");
    }

    // Exibe o indexador de palavras no formato esperado
    word_indexer_print_file(indexer);

    // Libera a memória alocada
    word_indexer_destroy(indexer);
    fclose(file);

    return 0;
}
