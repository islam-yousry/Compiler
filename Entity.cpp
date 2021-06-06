#include <bits/stdc++.h>
#include "Entity.h"

using namespace std;


bool is_accepted(Entity* entity){
    set<Node*> state = entity->state;
    for(Node* node : state){
        if(node->accepted) return true;
    }
    return false;
}

string find_token(Entity* entity){
    string token = "";
    assert(entity != NULL);
    set<Node*> state = entity->state;

    for(Node* node : state){
        if(node->accepted) return node->token;
    }
    return token;
}