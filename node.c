
#include <stdlib.h>
#include "node.h"

Node* node_create(void *data, void *key) {
    Node *node = (Node*)malloc(sizeof(Node));
    if (!node) return NULL;
    node->data = data;
    node->key = key;
    node->next = NULL;
    return node;
}

void node_destroy(Node *node, void (*destroy_data)(void *)) {
    if (destroy_data) {
        destroy_data(node->data);
    }
    free(node);
}

Node* node_insert(Node *head, void *data , void *key) {
    Node *new_node = node_create(data , key);
    new_node->next = head;
    return new_node;
}
