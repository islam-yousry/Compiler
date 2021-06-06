#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include "MinimizedDFA.h"
#include "LexicalParser.h"

using namespace std;




void LexicalParser::trace(string path, vector<string>& keywords, vector<char>& puncuations, Entity* head){ 
    stack<Entity*> entities_path;
    Entity* cur = head;
    string cur_token_val = "";
    for(int i = 0; i < path.size()+1; i++){
        if(i < path.size() && cur->next_states.find(path[i]) != cur->next_states.end()){
            cur = cur->next_states[path[i]];
            cur_token_val.push_back(path[i]);
            entities_path.push(cur);
        }else{
            if(cur_token_val == "" && find(puncuations.begin(),puncuations.end(),path[i]) != puncuations.end()){
                cout << path[i] << "\n";
                continue;
            }
            i--;
            while(entities_path.size() && !is_accepted(entities_path.top())){
                i--;
                entities_path.pop();
            }
            if(find(keywords.begin(),keywords.end(),cur_token_val) != keywords.end()){
                cout << cur_token_val << "\n";
            }
            else if(entities_path.size()) cout << find_token(entities_path.top()) << "\n";
            entities_path = stack<Entity*>();
            cur = head;
            cur_token_val = "";
            if(i == path.size()-1) break;
            if(path[i+1] == ' ') i++;
        }
    }
}   