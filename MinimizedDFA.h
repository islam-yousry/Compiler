#ifndef MINIMIZED_DFA_H
#define MINIMIZED_DFA_H

#include <vector>
#include "Entity.h"
using namespace std;


class MinimizedDFA{
private:
    vector<vector<Entity*>> groups;

    Entity* head;
    set<Entity*> minimized_states;

public:

    /**
     * return true if the two entities belong to the same group.
     */

    bool same_group(Entity* a, Entity* b);



    /**
     * preform minimze operation to the DFA.
     */

    void minimize(map<string,vector<string>>& regularExpression, vector<Entity*>& table, set<char>& inputs);

            
    /*
    construct table state for Minimzer DFA
    */

    void construct_minimized_DFA(vector<Entity*>& table);



    void print_minimzed_DFA(set<char>& inputs);

    Entity* get_head();

};
#endif