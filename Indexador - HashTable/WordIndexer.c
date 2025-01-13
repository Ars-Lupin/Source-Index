#include "WordIndexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Estrutura que armazena a palavra e a lista de frequências nos documentos
struct WordIndexer {
    HashTable *table; // Tabela de frequências das palavras nos documentos
};

WordIndexer *word_indexer_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn) 
{
    WordIndexer *indexer = (WordIndexer *)malloc(sizeof(WordIndexer));

    if (indexer == NULL)
        exit(printf("Erro ao alocar memória para o indexador de palavras\n"));

    indexer->table = hash_table_construct(table_size, hash_fn, cmp_fn);

    return indexer;
}

void word_indexer_add_word(WordIndexer *indexer, char *word, char* doc_name) 
{
    // Recupera a lista de DocumentFrequency da palavra
    List *df_list = (List *) hash_table_get(indexer->table, word); 

    // Se a palavra não foi encontrada na tabela, cria uma nova lista
    if (df_list == NULL) {
        df_list = list_construct();
        hash_table_set(indexer->table, strdup(word), df_list);
    }

    // Verifica se o documento já existe na lista de frequências
    Node *n = list_get_head(df_list);
    while (n) 
    {
        DocumentFrequency *current_df = (DocumentFrequency*) n->value;
        char* current_doc_name = doc_freq_get_name(current_df);

        if (!(strcmp(current_doc_name, doc_name))) {
            doc_freq_increment(current_df);
            return; // Saí da função se a frequência foi incrementada
        }
        n = n->next;
    }

    // Se o documento não foi encontrado, cria um novo DocumentFrequency
    DocumentFrequency *new_df = doc_freq_construct(doc_name); 
    list_push_front(df_list, new_df); // Certifique-se de que list_push_front não libere o novo_df
}


void word_indexer_print_file(WordIndexer *indexer) 
{
    HashTableIterator *it = hash_table_iterator(indexer->table);
    
    // Imprime qtd de palavras encontradas no documento
    printf("%d\n", hash_table_num_elems(indexer->table));

    while (!hash_table_iterator_is_over(it)) {
        HashTableItem *item = hash_table_iterator_next(it);
        
        // Verifica se o item não é nulo
        if (item == NULL || item->key == NULL || item->val == NULL) {
            continue; // Pula o item se for inválido
        }

        char *word = (char *) item->key;
        List *doc_list = (List *) item->val;
        
        printf("%s ", word);
        Node *n = list_get_head(doc_list);

        // Imprime a frequência da palavra em cada documento
        printf("%d ", list_size(doc_list));
        
        while (n != NULL) {
            DocumentFrequency *df = (DocumentFrequency *)n->value;
            char* current_doc_name = doc_freq_get_name(df);

            // Verifica se o documento é válido antes de imprimir
            if (current_doc_name != NULL) {
                printf("%s %d", current_doc_name, doc_freq_get_frequency(df));

                if (n->next != NULL)
                    printf(" ");
            }

            n = n->next;
        }
        
        if (!hash_table_iterator_is_over(it))
            printf("\n");
    }
    
    // Libera o iterador
    hash_table_iterator_destroy(it);
}

HashTable* word_indexer_get_table(WordIndexer *indexer) 
{
    return indexer->table;
}

void word_indexer_destroy(WordIndexer *indexer) 
{
    // Cria iterador para percorrer a tabela hash
    HashTableIterator *it = hash_table_iterator(indexer->table);
    
    while (!hash_table_iterator_is_over(it)) {
        HashTableItem *item = (HashTableItem*)hash_table_iterator_next(it);
        List *doc_list = item->val;
        char *word = item->key;

        // Destrói a lista de DocumentFrequency
        Node *n = list_get_head(doc_list);
        while (n != NULL) {
            DocumentFrequency *df = (DocumentFrequency *)n->value;
            doc_freq_destroy(df);
            n = n->next;
        }
        list_destroy(doc_list); // Libera a lista
        free(word); // Libera a palavra
        free(item); // Libera o item
    }
    
    hash_table_iterator_destroy(it);
    hash_table_destroy(indexer->table); // Libera a tabela hash
    free(indexer); // Libera o indexador
}
