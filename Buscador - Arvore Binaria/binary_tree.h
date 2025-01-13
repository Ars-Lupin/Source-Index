
#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

#include "vector.h"

typedef int (*CmpFn)(void *, void *);
typedef void (*KeyDestroyFn)(void *);
typedef void (*ValDestroyFn)(void *);

typedef struct BinaryTree BinaryTree;

typedef struct
{
    void *key;
    void *value;
} KeyValPair;

typedef struct NodeBinaryTree
{
    void *key;
    void *value;
    struct NodeBinaryTree *left;
    struct NodeBinaryTree *right;
} NodeBinaryTree;

KeyValPair *key_val_pair_construct(void *key, void *val);
void key_val_pair_destroy(KeyValPair *kvp);

NodeBinaryTree *node_binary_tree_construct(void *key, void *value, NodeBinaryTree *left, NodeBinaryTree *right);
void node_binary_tree_destroy(NodeBinaryTree *node);

BinaryTree *binary_tree_construct(
    CmpFn cmp_fn, KeyDestroyFn key_destroy_fn,
    ValDestroyFn val_destroy_fn);
void binary_tree_add(BinaryTree *bt, void *key, void *value);
void binary_tree_add_recursive(BinaryTree *bt, void *key, void *value);
int binary_tree_empty(BinaryTree *bt);
void binary_tree_remove(BinaryTree *bt, void *key);
KeyValPair *binary_tree_min(BinaryTree *bt);
KeyValPair *binary_tree_max(BinaryTree *bt);
KeyValPair *binary_tree_pop_min(BinaryTree *bt);
KeyValPair *binary_tree_pop_max(BinaryTree *bt);
void *binary_tree_get(BinaryTree *bt, void *key);
void binary_tree_destroy(BinaryTree *bt);

// a funcao abaixo pode ser util para debug, mas nao eh obrigatoria.
void binary_tree_print(BinaryTree *bt);

Vector *binary_tree_inorder_traversal(BinaryTree *bt);
Vector *binary_tree_preorder_traversal(BinaryTree *bt);
Vector *binary_tree_postorder_traversal(BinaryTree *bt);
Vector *binary_tree_levelorder_traversal(BinaryTree *bt);

Vector *binary_tree_inorder_traversal_recursive(BinaryTree *bt);
Vector *binary_tree_preorder_traversal_recursive(BinaryTree *bt);
Vector *binary_tree_postorder_traversal_recursive(BinaryTree *bt);

// Funcoes extras que adicionei no programa
void *binary_tree_get_root(BinaryTree *bt);

#endif