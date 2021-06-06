#ifndef GRAPH_H
#define GRAPH_H

#include "Node.h"

struct Graph{
    Node* head;
    Node* end;
    Graph(Node* head, Node* end):head(head),end(end){}
};

#endif