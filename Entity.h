#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <map>
#include <set>
#include "Node.h"

using namespace std;


struct Entity{
    string name;
    set<Node*> state;
    map<char,Entity*> next_states;
    Entity(set<Node*> state, string name): state(state), name(name){}
};


/**
 *  return true if the entity is accpted
 */

bool is_accepted(Entity* entity);



string find_token(Entity* entity);

#endif