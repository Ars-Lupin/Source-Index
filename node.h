
#ifndef NODE_H
#define NODE_H

typedef struct Node {
    void *data;
    void *key;
    struct Node *next;
} Node;

Node* node_create(void *data, void *key);
void node_destroy(Node *node, void (*destroy_data)(void *));
Node* node_insert(Node *head, void *data, void *key);

#endif
