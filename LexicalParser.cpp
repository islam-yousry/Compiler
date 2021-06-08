#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include "MinimizedDFA.h"
#include "LexicalParser.h"

using namespace std;



LexicalParser::LexicalParser(Entity* head):entity(head),index(-1){}


string LexicalParser::next_token(string& path, vector<string>& keywords, vector<char>& puncuations){ 
    stack<Entity*> entities_path;
    Entity* cur = entity;
    string cur_token_val = "";
    while(++index < path.size()+1){
        if(index < path.size() && cur->next_states.find(path[index]) != cur->next_states.end()){
            cur = cur->next_states[path[index]];
            cur_token_val.push_back(path[index]);
            entities_path.push(cur);
        }else{
            if(cur_token_val == "" && find(puncuations.begin(),puncuations.end(),path[index]) != puncuations.end()){
                return string(1,path[index]);
                continue;
            }
            index--;
            while(entities_path.size() && !is_accepted(entities_path.top())){
                index--;
                entities_path.pop();
            }
            if(find(keywords.begin(),keywords.end(),cur_token_val) != keywords.end()){
                return cur_token_val;
            }
            else if(entities_path.size()) return find_token(entities_path.top());
            else { // error
                index++;
                return "";
            }
            // entities_path = stack<Entity*>();
            // cur = entity;
            // cur_token_val = "";
            // if(index == path.size()-1) break;
            // if(path[index+1] == ' ') index++;
        }
    }
    return "";
}   

bool LexicalParser::has_next(string& path){
    return (index < (int)path.size());
}

void LexicalParser::resetIndex(){
    index = -1;
}