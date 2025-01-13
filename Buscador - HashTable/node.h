
#ifndef _NODE_H_
#define _NODE_H_

typedef void* data_type;

typedef struct Node
{
    data_type value;
    struct Node *prev;
    struct Node *next;
} Node;

Node *node_construct(data_type value, Node *prev, Node *next);
void node_destroy(Node *n);
void node_print(Node *l, void (*print_fn)(data_type));

#endif