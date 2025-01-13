#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary_tree.h"
#include "deque.h"

struct BinaryTree
{
    NodeBinaryTree *root;
    CmpFn cmp_fn;
    KeyDestroyFn key_destroy_fn;
    ValDestroyFn val_destroy_fn;
};

KeyValPair *key_val_pair_construct(void *key, void *val)
{
    KeyValPair *kvp = malloc(sizeof(KeyValPair));

    if (!kvp) exit(printf("ERRO: falha ao alocar memoria para o par chave-valor\n"));

    kvp->key = key;
    kvp->value = val;

    return kvp;
}

void key_val_pair_destroy(KeyValPair *kvp)
{
    if (kvp)
    {
        //if (kvp->key) free(kvp->key);
        //if (kvp->value) free(kvp->value);
        free(kvp);
    }
        
    return;
}

NodeBinaryTree *node_binary_tree_construct(void *key, void *value, NodeBinaryTree *left, NodeBinaryTree *right)
{
    NodeBinaryTree *node = malloc(sizeof(NodeBinaryTree));

    if (!node) exit(printf("ERRO: falha ao alocar memoria para o no\n"));

    node->key = key;
    node->value = value;
    node->left = left;
    node->right = right;

    return node;
}

void node_binary_tree_destroy(NodeBinaryTree *node) 
{
    if (node) // Se o nó não for nulo
    {
        // Destruir os filhos do nó atual
        node_binary_tree_destroy(node->left);
        node_binary_tree_destroy(node->right);

        // Destruir o nó atual
        if (node->key) free(node->key);
        //if (node->value) free(node->value); // Valor deve ser liberado pelo usuário

        free(node);
    }

    return;
}

BinaryTree *binary_tree_construct(
    CmpFn cmp_fn, KeyDestroyFn key_destroy_fn,
    ValDestroyFn val_destroy_fn)
{
    BinaryTree *bt = malloc(sizeof(BinaryTree));

    if (!bt) exit(printf("ERRO: falha ao alocar memoria para a arvore binaria\n"));

    bt->root = NULL;
    bt->cmp_fn = cmp_fn;
    bt->key_destroy_fn = key_destroy_fn;
    bt->val_destroy_fn = val_destroy_fn;

    return bt;
}

void binary_tree_add(BinaryTree *bt, void *key, void *value) 
{
    // Se o valor já estiver na árvore, não adiciona
    if (binary_tree_get(bt, key))
    {
        bt->key_destroy_fn(key);
        bt->val_destroy_fn(value);
        return;
    }

    NodeBinaryTree* new_node = node_binary_tree_construct(key, value, NULL, NULL);

    if (!bt->root) { // Se a árvore estiver vazia (raiz é nula)
        bt->root = new_node;
        return;
    }

    NodeBinaryTree *current = bt->root;
    while(1) {
        if (bt->cmp_fn(key, current->key) <= 0) {
            if (!current->left) // Se o nó atual não tiver filho à esquerda
            { 
                current->left = new_node; // Adiciona o novo nó à esquerda
                break; // Sai do loop
            }
            current = current->left; // Atualiza o nó atual para o filho à esquerda
        } 
        else // Se a chave for maior ou igual ao nó atual
        { 
            if (!current->right) // Se o nó atual não tiver filho à direita
            { 
                current->right = new_node; // Adiciona o novo nó à direita
                break; // Sai do loop
            }
            current = current->right; // Atualiza o nó atual para o filho à direita
        }
    }
}

NodeBinaryTree* binary_tree_add_recursive_aux(NodeBinaryTree *node, void *key, void *value, BinaryTree *bt)
{
    if (!node) // Se o nó for nulo
        return node_binary_tree_construct(key, value, NULL, NULL);

    if (bt->cmp_fn(key, node->key) <= 0) // Se a chave for menor ou igual à chave do nó atual
        node->left = binary_tree_add_recursive_aux(node->left, key, value, bt);
    else // Se a chave for maior que a chave do nó atual
        node->right = binary_tree_add_recursive_aux(node->right, key, value, bt);

    return node;
}

void binary_tree_add_recursive(BinaryTree *bt, void *key, void *value)
{
    // Se o valor já estiver na árvore, não adiciona
    if (binary_tree_get(bt, key))
    {
        bt->key_destroy_fn(key);
        bt->val_destroy_fn(value);
        return;
    }

    bt->root = binary_tree_add_recursive_aux(bt->root, key, value, bt);

    return;
}

int binary_tree_empty(BinaryTree *bt)
{
    return bt->root == NULL;
}

void binary_tree_remove(BinaryTree *bt, void *key)
{
    NodeBinaryTree *current = bt->root;
    NodeBinaryTree *parent = NULL;
    NodeBinaryTree *removed_node = NULL; // Nó a ser removido (deve ser liberado)

    // Encontrar o nó a ser removido
    while (current)
    {
        if (bt->cmp_fn(key, current->key) == 0) // Se a chave for igual à chave do nó atual
        {
            removed_node = current;
            break;
        }
        else if (bt->cmp_fn(key, current->key) < 0) // Se a chave for menor que a chave do nó atual
        {
            parent = current;
            current = current->left;
        }
        else // Se a chave for maior que a chave do nó atual
        {
            parent = current;
            current = current->right;
        }
    }

    if (!removed_node) return; // Se o nó a ser removido não foi encontrado

    // Caso 1: nó a ser removido não tem filhos
    if (!removed_node->left && !removed_node->right)
    {
        if (parent) // Se o nó a ser removido não for a raiz
        {
            if (parent->left == removed_node) parent->left = NULL;
            else parent->right = NULL;
        }
        else // Se o nó a ser removido for a raiz
            bt->root = NULL;
    }

    // Caso 2: nó a ser removido tem um filho
    else if (!removed_node->left || !removed_node->right)
    {
        NodeBinaryTree *child = removed_node->left ? removed_node->left : removed_node->right;

        if (parent) // Se o nó a ser removido não for a raiz
        {
            if (parent->left == removed_node) parent->left = child;
            else parent->right = child;
        }
        else // Se o nó a ser removido for a raiz
            bt->root = child;
    }

    // Caso 3: nó a ser removido tem dois filhos
    else
    {
        NodeBinaryTree *successor = removed_node->right;
        NodeBinaryTree *successor_parent = removed_node;

        // Encontrar o nó sucessor (menor nó à direita do nó a ser removido)
        while (successor->left)
        {
            successor_parent = successor;
            successor = successor->left;
        }

        // Atualizar os ponteiros dos nós
        if (successor_parent != removed_node)
        {
            successor_parent->left = successor->right;
            successor->right = removed_node->right;
        }

        successor->left = removed_node->left;

        if (parent) // Se o nó a ser removido não for a raiz
        {
            if (parent->left == removed_node) parent->left = successor;
            else parent->right = successor;
        }
        else // Se o nó a ser removido for a raiz
            bt->root = successor;
    }

    // Liberar o nó removido
    if (removed_node->key) bt->key_destroy_fn(removed_node->key);
    if (removed_node->value) bt->val_destroy_fn(removed_node->value);
    free(removed_node);

    return;
}

KeyValPair *binary_tree_min(BinaryTree *bt)
{
    NodeBinaryTree *current = bt->root;

    if (binary_tree_empty(bt)) return NULL; // Se a árvore estiver vazia

    while (current->left) // Enquanto houver filho à esquerda
        current = current->left;

    return key_val_pair_construct(current->key, current->value);
}

KeyValPair *binary_tree_max(BinaryTree *bt)
{
    NodeBinaryTree *current = bt->root;

    if (binary_tree_empty(bt)) return NULL; // Se a árvore estiver vazia

    while (current->right) // Enquanto houver filho à direita
        current = current->right;

    return key_val_pair_construct(current->key, current->value);
}

KeyValPair *binary_tree_pop_min(BinaryTree *bt)
{
    NodeBinaryTree *current = bt->root;

    if (binary_tree_empty(bt)) return NULL; // Se a árvore estiver vazia

    // Encontrar o nó com a chave mínima
    while (current->left)
        current = current->left;

    // Copio a chave e o valor para retornar
    void* key_copy = malloc(sizeof(int));
    if (key_copy == NULL) return NULL; // Verificar falha de alocação
    memcpy(key_copy, current->key, sizeof(int)); // Copiar o conteúdo da chave

    void* value_copy = malloc(sizeof(int));
    if (value_copy == NULL) return NULL; // Verificar falha de alocação
    memcpy(value_copy, current->value, sizeof(int)); // Copiar o conteúdo do valor

    // Remove o nó com a chave mínima
    binary_tree_remove(bt, current->key);

    // Retorna a chave e o valor
    return key_val_pair_construct(key_copy, value_copy);
}

KeyValPair *binary_tree_pop_max(BinaryTree *bt)
{
    NodeBinaryTree *current = bt->root;

    if (binary_tree_empty(bt)) return NULL; // Se a árvore estiver vazia

    // Encontra o nó com a chave máxima
    while (current->right)
        current = current->right;

    // Copio a chave e o valor para retornar
    void* key_copy = malloc(sizeof(int));
    if (key_copy == NULL) return NULL; // Verificar falha de alocação
    memcpy(key_copy, current->key, sizeof(int)); // Copiar o conteúdo da chave

    void* value_copy = malloc(sizeof(int));
    if (value_copy == NULL) return NULL; // Verificar falha de alocação
    memcpy(value_copy, current->value, sizeof(int)); // Copiar o conteúdo do valor

    // Remove o nó com a chave máxima
    binary_tree_remove(bt, current->key);

    // Retorna a chave e o valor
    return key_val_pair_construct(key_copy, value_copy);
}

void *binary_tree_get(BinaryTree *bt, void *key)
{
    NodeBinaryTree *current = bt->root;

    while (current)
    {
        if (bt->cmp_fn(key, current->key) == 0) // Se a chave for igual à chave do nó atual
            return current->value;
        else if (bt->cmp_fn(key, current->key) < 0) // Se a chave for menor que a chave do nó atual
            current = current->left;
        else // Se a chave for maior que a chave do nó atual
            current = current->right;
    }

    return NULL;
}

void binary_tree_destroy(BinaryTree *bt) 
{
    if (bt) 
    {
        // Destruir todos os nós da árvore
        node_binary_tree_destroy(bt->root);

        // Liberar a estrutura da árvore
        free(bt);
    }

    return;
}

void binary_tree_print_recursive(NodeBinaryTree *node)
{

    if (node)
    {   
        printf("(");
        printf("%s, ", (char*)node->key);
        binary_tree_print_recursive(node->left);
        printf(", ");
        binary_tree_print_recursive(node->right);
        printf(")");
    } 
    else 
    {
        printf("NULL");
    }

    return;
}

void binary_tree_print(BinaryTree *bt)
{
    binary_tree_print_recursive(bt->root);
    return;
}

Vector *binary_tree_inorder_traversal(BinaryTree *bt)
{
    Deque *stack = deque_construct();
    Vector *v = vector_construct();
    NodeBinaryTree* current_node = bt->root;

    while (1)
    {
        while (current_node)
        {
            deque_push_back(stack, current_node);
            current_node = current_node->left;
        }

        if (deque_size(stack) == 0) break;
        else
        {
            current_node = deque_pop_back(stack);
            vector_push_back(v, key_val_pair_construct(current_node->key, current_node->value));
            current_node = current_node->right;
        }
    }

    deque_destroy(stack);
    return v;
}

Vector *binary_tree_preorder_traversal(BinaryTree *bt)
{
    Deque *stack = deque_construct();
    Vector *v = vector_construct();
    deque_push_back(stack, bt->root);

    while(deque_size(stack) > 0)
    {
        NodeBinaryTree *node = deque_pop_back(stack);
        vector_push_back(v, key_val_pair_construct(node->key, node->value));
        if (node->right) deque_push_back(stack, node->right);
        if (node->left) deque_push_back(stack, node->left);
    }

    deque_destroy(stack);
    return v;
}

Vector *binary_tree_postorder_traversal(BinaryTree *bt)
{
    Deque* q1 = deque_construct();
    Deque* q2 = deque_construct();
    Vector* v = vector_construct();
    deque_push_back(q1, bt->root);

    while (deque_size(q1) > 0)
    {
        NodeBinaryTree* node = deque_pop_back(q1);

        if (node->left) deque_push_back(q1, node->left);
        if (node->right) deque_push_back(q1, node->right);
        deque_push_back(q2, node);
    }

    for (int i = deque_size(q2) - 1; i >= 0; i--)
    {
        NodeBinaryTree* node = deque_get(q2, i);
        vector_push_back(v, key_val_pair_construct(node->key, node->value));
    }

    deque_destroy(q1);
    deque_destroy(q2);
    return v;
}

Vector *binary_tree_levelorder_traversal(BinaryTree *bt)
{
    Deque* queue = deque_construct();
    Vector* v = vector_construct();

    deque_push_front(queue, bt->root);

    while (deque_size(queue) > 0)
    {
        NodeBinaryTree* node = deque_pop_back(queue);
        
        if (node != NULL)
        {
            vector_push_back(v, key_val_pair_construct(node->key, node->value));
            deque_push_front(queue, node->left);
            deque_push_front(queue, node->right);
        }
    }

    deque_destroy(queue);
    return v;
}

Vector *binary_tree_inorder_traversal_recursive_aux(NodeBinaryTree *node, Vector *v)
{
    if (node)
    {
        binary_tree_inorder_traversal_recursive_aux(node->left, v);
        vector_push_back(v, key_val_pair_construct(node->key, node->value));
        binary_tree_inorder_traversal_recursive_aux(node->right, v);
    }

    return v;
}

Vector *binary_tree_inorder_traversal_recursive(BinaryTree *bt)
{
    Vector *v = vector_construct();
    binary_tree_inorder_traversal_recursive_aux(bt->root, v);
    return v;
}

Vector *binary_tree_preorder_traversal_recursive_aux(NodeBinaryTree *node, Vector *v)
{
    if (node)
    {
        vector_push_back(v, key_val_pair_construct(node->key, node->value));
        binary_tree_preorder_traversal_recursive_aux(node->left, v);
        binary_tree_preorder_traversal_recursive_aux(node->right, v);
    }

    return v;
}

Vector *binary_tree_preorder_traversal_recursive(BinaryTree *bt)
{
    Vector *v = vector_construct();
    binary_tree_preorder_traversal_recursive_aux(bt->root, v);
    return v;
}

Vector *binary_tree_postorder_traversal_recursive_aux(NodeBinaryTree *node, Vector *v)
{
    if (node)
    {
        binary_tree_postorder_traversal_recursive_aux(node->left, v);
        binary_tree_postorder_traversal_recursive_aux(node->right, v);
        vector_push_back(v, key_val_pair_construct(node->key, node->value));
    }

    return v;
}

Vector *binary_tree_postorder_traversal_recursive(BinaryTree *bt)
{
    Vector *v = vector_construct();
    binary_tree_postorder_traversal_recursive_aux(bt->root, v);
    return v;
}

void *binary_tree_get_root(BinaryTree *bt)
{
    return bt->root;
}
