#ifndef DFA_H
#define DFA_H

#include "Graph.h"
#include "Entity.h"
#include "Utilities.h"

using namespace std;

class DFA{

private:
    map<vector<Node*>,Entity*> state_toentity;
    vector<Entity*> table;
    set<char> inputs;
public:



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


    void construct_DFA(vector<Graph*>& graphes);


    void print_table();


    vector<Entity*>& get_table();

    set<char>& get_inputs();

};

#endif