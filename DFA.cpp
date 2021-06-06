#include "DFA.h"
#include <map>
#include <iostream>

using namespace std;

void DFA::next_closure(vector<Node*>& nodes, Node* node, set<Node*>& visited){
    for(pair<Node*,string> next: node->next){
        Node* next_node = next.first;
        string edge_val = next.second;
        if(edge_val == EPSON){
            if(visited.find(next_node) != visited.end()) continue;
            visited.insert(next_node);
            nodes.push_back(next_node);
            next_closure(nodes,next_node,visited);
        }
    }
}


void DFA::next_move(map<char,Entity*>& next_state, Node* node){
    set<Node*> visited;
    for(pair<Node*,string> next : node->next){
        Node* next_node = next.first;
        string edge_val = next.second;
        if(edge_val == EPSON) continue;
        inputs.insert(edge_val[0]);
        vector<Node*> next_nodes {};
        next_nodes.push_back(next_node);
        next_closure(next_nodes,next_node,visited); 
        if(state_toentity.find(next_nodes) == state_toentity.end()) state_toentity[next_nodes] = new Entity(set<Node*> (next_nodes.begin(),next_nodes.end())," ");
        if(next_state.find(edge_val[0]) == next_state.end()) next_state[edge_val[0]] = state_toentity[next_nodes];
        else{
            for(int i = 0; i < next_nodes.size(); i++){
				Node* n = next_nodes[i];
                next_state[edge_val[0]]->state.insert(n);
            }
        }
      }
}





/**
 * construct Deteminstice Finite Automate from NonDetermenistic finite Automate.
 * 
 */


void DFA::constructDFA(vector<Graph*>& graphes){
     
    vector<Node*> head;
    for(Graph* graph : graphes){
        set<Node*> visited;
        head.push_back(graph->head);
        next_closure(head,graph->head,visited);
    }
    int name = 0;
    table.push_back(new Entity(set<Node*>(head.begin(),head.end()),to_string(name++)));
    set<Entity*> used;
    used.insert(table.back());
    for(int i = 0; i < table.size(); i++){
        Entity* entity = table[i];
        for(Node* n : entity->state){
            next_move(entity->next_states,n);
        }
        for(auto j : entity->next_states){
            if(used.find(j.second) == used.end()){
                used.insert(j.second);
                j.second->name = (to_string(name++));
                table.push_back(j.second);
            }   
        }
    
    }
    
}



void DFA::print_table(){
    freopen("output.txt","w",stdout);
    for(char c : inputs)cout<< " "<< c << " ";
    cout << "\n";
    for(Entity* entity : table){
        cout << entity->name<<"|"<<is_accepted(entity)<<"|";
        for(auto i : entity->next_states){
            cout << "(" << i.first << ")";
            cout << i.second->name<<" ";
        }cout <<"\n";
        
    }
}
