#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "document_frequency.h"
#include "vector.h"
#include "binary_tree.h"
#include "search.h"

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

int cmp_str(void *a, void *b)
{
    return strcmp((char *)a, (char *)b);
}

int cmp_string(const void *a, const void *b)
{
    return cmp_str((char *)a, (char *)b);
}

void index_tree_destroy(BinaryTree* index_tree)
{
    Vector *v = binary_tree_inorder_traversal(index_tree);

    for (int i = 0; i < vector_size(v); i++)
    {
        KeyValPair *kvp = (KeyValPair *)vector_get(v, i);
        List *doc_list = (List *)kvp->value;

        // Destrói a lista de DocumentFrequency
        Node *n = list_get_head(doc_list);
        while (n != NULL)
        {
            DocumentFrequency *df = (DocumentFrequency *)n->value;
            doc_freq_destroy(df);
            n = n->next;
        }
        list_destroy(doc_list);
        key_val_pair_destroy(kvp);
    }
    vector_destroy(v);
    binary_tree_destroy(index_tree);
}

int main()
{

    // Criação da tabela hash
    BinaryTree *index_tree = binary_tree_construct(cmp_fn, key_destroy_fn, val_destroy_fn);

    // Carrega o índice do arquivo
    char index_file[MAX_DOC_NAME];
    scanf("%s", index_file);
    load_index(index_tree, index_file);

    // Leitura das stopwords
    Vector *stopwords = vector_construct();

    char stopwords_file[MAX_DOC_NAME];
    scanf("%s", stopwords_file);
    load_stopwords(stopwords, stopwords_file, cmp_string);

    // Leitura e processamento das palavras da consulta
    process_query(index_tree, stopwords, cmp_fn, key_destroy_fn, val_destroy_fn, cmp_string);

    // Destruição da arvore binaria de indices
    index_tree_destroy(index_tree);

    // Destruição das stopwords
    int size = vector_size(stopwords);
    for (int i = 0; i < size; i++)
        free(vector_get(stopwords, i));

    vector_destroy(stopwords);

    return 0;
}
