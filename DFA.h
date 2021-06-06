#ifndef DFA_H
#define DFA_H

#include "Graph.h"
#include "Entity.h"
#include "Utilities.h"

using namespace std;

class DFA{

public:
    vector<Entity*> table;
    set<char> inputs;
    map<vector<Node*>,Entity*> state_toentity;



    /**
     * get next states which is reachable from node by EPSON move.
     */

    void next_closure(vector<Node*>& nodes, Node* node, set<Node*>& visited);


    /**
     * get the next state of the node for all possible inputs.
     */

    void next_move(map<char,Entity*>& next_state, Node* node);



    /**
     * construct Deteminstice Finite Automate from NonDetermenistic finite Automate.
     * 
     */


    void constructDFA(vector<Graph*>& graphes);


    void print_table();

};

#endif