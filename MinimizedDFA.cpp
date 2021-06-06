#include <iostream>
#include <vector>
#include <algorithm>
#include "DFA.h"
#include "MinimizedDFA.h"

using namespace std;


bool MinimizedDFA::same_group(Entity* a, Entity* b){
    for(vector<Entity*> group : groups){
        if(find(group.begin(),group.end(),a) != group.end() && find(group.begin(),group.end(),b) != group.end()){
            return true;
        }
    }
    return false;
}


void MinimizedDFA::minimize(map<string,vector<string>>& regularExpression, vector<Entity*>& table, set<char>& inputs){
    map<string,int> expression_to_index;
    int idx = 1;
    for(auto it : regularExpression){
        expression_to_index[it.first] = idx++;
    }
    groups.resize(1+regularExpression.size());
    for(int i = 0; i < table.size(); i++){
        Entity* entity = table[i];
        if(is_accepted(entity)){
            groups[expression_to_index[find_token(entity)]].push_back(entity);
        }
        else{
            groups[0].push_back(entity);
        }
    }
    bool found = true;
    while(found){
        found = false;
        int size = groups.size();
        for(int group_id = 0; group_id < size; group_id++){
            vector<Entity*>& group = groups[group_id];
            if(group.size() > 1){
                set<Entity*> out_group;
                set<Entity*> in_group;
                in_group.insert(group[0]);
                map<char,Entity*> first_next_state = group[0]->next_states;
                for(int entity_id = 1; entity_id < group.size(); entity_id++){
                    Entity* cur_entity = group[entity_id];
                    bool b = false;
                    map<char,Entity*> cur_next_state = cur_entity->next_states;
                    for(char input : inputs){
                        if(((first_next_state.find(input) == first_next_state.end()) != (cur_next_state.find(input) == cur_next_state.end())) 
                        || ((first_next_state.find(input) != first_next_state.end()) && !same_group(first_next_state[input],cur_next_state[input]))){
                            b = true;
                            out_group.insert(cur_entity);
                            break;
                        }
                    }
                    if(!b) in_group.insert(cur_entity);
                }
                if(out_group.size()){
                    found = true;
                    groups[group_id] = {};
                    for(Entity* entity : in_group){
                        groups[group_id].push_back(entity);
                    }
                    groups.push_back({});
                    for(Entity* entity : out_group){
                        groups.back().push_back(entity);
                    }
                }
            }
        }   
    }
    
}

        
void MinimizedDFA::constructMinimizedDFA(vector<Entity*>& table){
    map<Entity*,Entity*> entity_togroup;
    int num = 0;
    for(vector<Entity*> group : groups){
        Entity* group_entity = new Entity({},to_string(num++)); 
        minimized_states.insert(group_entity);
        for(Entity* entity : group){
            if(table[0] == entity) head = group_entity;
            entity_togroup[entity] = group_entity;
        }
    }
    for(vector<Entity*> group : groups){
        for(Entity* entity : group){
            Entity* group_entity = entity_togroup[entity];
            group_entity->state.insert(entity->state.begin(),entity->state.end());
            for(auto& i : entity->next_states){
                char edge_val = i.first;
                Entity* next_entity = i.second;
                group_entity->next_states[edge_val] = entity_togroup[next_entity];
            }
        }
    }
    
}

void MinimizedDFA::print_minimzed_DFA(set<char>& inputs){
    freopen("minimized DFA output.txt","w",stdout);
    for(char c : inputs)cout<< " "<< c << " ";
    cout << "\n";
    for(Entity* entity : minimized_states){
        cout << entity->name<< " " << ((is_accepted(entity))?"acc":"not") << " | ";
        for(char c : inputs){
            if(entity->next_states.find(c) == entity->next_states.end()) cout << "  ";
            else cout << "("<<c<<")"<< entity->next_states[c]->name << " ";
        }
        cout << "\n";
    }
}
