#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "document_frequency.h"
#include "vector.h"
#include "word_indexer.h"

#define MAX_DIRECTORY 1000


int cmp_fn(void *word1, void *word2) {
    // Cast para converter os ponteiros genéricos para ponteiros para string
    const char *w1 = (const char *)word1;
    const char *w2 = (const char *)word2;
    
    // Utiliza a função strcmp para comparar as strings
    return strcmp(w1, w2);
}

void key_destroy_fn(void *key)
{
    free(key);
}

void val_destroy_fn(void *val)
{
    free(val);
}

int *new_int(int value)
{
    int *ptr = malloc(sizeof(int));
    *ptr = value;
    return ptr;
}

int main() 
{

    char dir[MAX_DIRECTORY];
    scanf("%s", dir);
    FILE *file = fopen(dir, "r");

    if (file == NULL) exit(printf("ERRO: nao foi possivel abrir o arquivo\n"));

    // Lê o número de documentos
    int qtd_docs;
    fscanf(file, "%d\n", &qtd_docs);

    // Criação do indexador de palavras
    WordIndexer *indexer = word_indexer_construct(cmp_fn, key_destroy_fn, val_destroy_fn);

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
