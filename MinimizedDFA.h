#ifndef MINIMIZED_DFA_H
#define MINIMIZED_DFA_H

#include <vector>
#include "Entity.h"
using namespace std;


class MinimizedDFA{

public:
    vector<vector<Entity*>> groups;

    Entity* head;
    set<Entity*> minimized_states;


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

    void constructMinimizedDFA(vector<Entity*>& table);



    void print_minimzed_DFA(set<char>& inputs);



};
#endif