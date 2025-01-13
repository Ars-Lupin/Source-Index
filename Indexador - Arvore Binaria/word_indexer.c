#include "word_indexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct WordIndexer {
    BinaryTree *tree; // Árvore binária de palavras
};

WordIndexer *word_indexer_construct(CmpFn cmp_fn, 
                                    KeyDestroyFn key_destroy_fn, 
                                    ValDestroyFn val_destroy_fn) 
{
    WordIndexer *indexer = (WordIndexer *)malloc(sizeof(WordIndexer));

    if (indexer == NULL)
        exit(printf("Erro ao alocar memória para o indexador de palavras\n"));

    indexer->tree = binary_tree_construct(cmp_fn, key_destroy_fn, val_destroy_fn);

    return indexer;
}

void word_indexer_add_word(WordIndexer *indexer, char *word, char* doc_name) 
{
    BinaryTree *tree = indexer->tree;
    KeyValPair *kvp = binary_tree_get(tree, word);


    if (kvp == NULL) // Se a palavra não estiver na árvore
    {
        // Cria uma lista de DocumentFrequency
        List *doc_list = list_construct(cmp_doc_freq, doc_freq_destroy);
        DocumentFrequency *df = doc_freq_construct(doc_name);
        list_push_front(doc_list, df);
        binary_tree_add(tree, strdup(word), doc_list);
    } 
    else 
    {
        // Recupera a lista de DocumentFrequency
        List *doc_list = (List *)kvp->value;
        Node *n = list_get_head(doc_list);
        DocumentFrequency *df = NULL;

        while (n != NULL) // Procura o documento na lista
        {
            DocumentFrequency *current_df = (DocumentFrequency *)n->value;

            // Se o documento já existe na lista
            if (strcmp(doc_name, doc_freq_get_name(current_df)) == 0) 
            {
                df = current_df; // Atribui o DocumentFrequency atual
                break;
            }
            n = n->next;
        }

        if (df == NULL) // Se o documento não foi encontrado
        {
            // Cria um novo DocumentFrequency
            df = doc_freq_construct(doc_name);
            list_push_front(doc_list, df);
        } 
        else 
        {
            // Incrementa a frequência da palavra no documento
            doc_freq_increment(df);
        }
    }
}

void word_indexer_print_file(WordIndexer *indexer) 
{
    BinaryTree *tree = indexer->tree;
    Vector *v = binary_tree_inorder_traversal(tree);

    // Imprime qtd de palavras encontradas no documento
    printf("%d\n", vector_size(v));

    for (int i = 0; i < vector_size(v); i++)
    {
        KeyValPair *kvp = (KeyValPair *)vector_get(v, i);
        char *word = (char *)kvp->key;
        List *doc_list = (List *)kvp->value;

        printf("%s ", word);
        printf("%d ", list_size(doc_list));

        Node *n = list_get_head(doc_list);
        while (n != NULL) 
        {
            DocumentFrequency *df = (DocumentFrequency *)n->value;
            char* current_doc_name = doc_freq_get_name(df);

            printf("%s %d", current_doc_name, doc_freq_get_frequency(df));

            if (n->next != NULL)
                printf(" ");

            n = n->next;
        }

        if (i < vector_size(v) - 1)
            printf("\n");
    }

    for (int i = 0; i < vector_size(v); i++)
    {
        KeyValPair *kvp = (KeyValPair *)vector_get(v, i);
        key_val_pair_destroy(kvp);
    }

    vector_destroy(v);
}

BinaryTree* word_indexer_get_binary_tree(WordIndexer *indexer)
{
    return indexer->tree;
}

void word_indexer_destroy(WordIndexer *indexer)
{
    BinaryTree *tree = indexer->tree;
    Vector *v = binary_tree_inorder_traversal(tree);

    for (int i = 0; i < vector_size(v); i++)
    {
        KeyValPair *kvp = (KeyValPair *)vector_get(v, i);
        List *doc_list = (List *)kvp->value;
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
    binary_tree_destroy(tree);
    free(indexer);
}
