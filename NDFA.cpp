#include "NDFA.h"
#include <stack>
#include <iostream>

using namespace std;




Graph* NDFA::closure(Graph* u, string expression){
    Node* right = new Node(true,u->end->token,{});
    Node* left = new Node(false,"",{{u->head,EPSON}});
    if(expression == "*") left->next.push_back({right,EPSON});
    u->end->accepted = false;
    u->end->token="";
    u->end->next = {{u->head,EPSON},{right,EPSON}};
    Graph* graph = new Graph(left,right);
    return graph;
}



Graph* NDFA::dis(Graph* u, Graph* v){
    Node* left = new Node(false,"",{{u->head,EPSON},{v->head,EPSON}});
    Node* right = new Node(true,u->end->token,{});
    u->end->next = v->end->next = {{right,EPSON}};
    u->end->accepted = v->end->accepted = false;
    u->end->token = v->end->token = "";
    Graph* graph = new Graph(left,right);
    return graph;
}


Graph* NDFA::add(Graph* u, Graph* v){
    u->end->accepted = false;
    u->end->token="";
    u->end->next = v->head->next;
    v->head = u->end;
    Graph* graph = new Graph(u->head,v->end);
    return graph;
}



void NDFA::calculate(stack<Graph*>& g, stack<string>& operations){
    // assert(operations.size() > 0);
    // assert(g.size() > 0);
    
    string operation = operations.top();
    // cout << operation << " ";
    if(operation == "|"){
        Graph* v = g.top();
        g.pop();
        Graph* u = g.top();
        g.pop();    
        g.push(dis(u,v));
    }
    else if(operation == "and"){
        Graph* v = g.top();
        g.pop();
        Graph* u = g.top();
        g.pop();
        g.push(add(u,v));
    }
    else{
        Graph* u = g.top();
        g.pop();
        g.push(closure(u,operation));
    }
    operations.pop();
}



void NDFA::constructNDFA(map<string,vector<string>>& regularExpression, vector<Graph*>& graphes){
    for(auto i : regularExpression){
        stack<Graph*> expression_graphes;
        stack<string> operations;
        bool is_prev_expression = false;
        for(string expression : i.second){ 
            if(reserved.find(expression) != reserved.end()){
                if(expression == "(" && is_prev_expression){
                    is_prev_expression = false;
                    operations.push("and");
                }
                
                if(expression == ")"){
                    // assert(operations.size() > 0);
                    while(operations.top() != "("){
                        calculate(expression_graphes,operations);
                    }
                    // assert(operations.size() > 0);
                    operations.pop();
                }
                else if(expression == "*" || expression == "+"){
                    is_prev_expression = true;
                    operations.push(expression);
                    calculate(expression_graphes,operations);
                }
                else {
                    is_prev_expression = false;
                    operations.push(expression);
                }
            }
            else {
                if(is_prev_expression){
                    operations.push("and");
                }
                if(expression == "\\L") expression = EPSON;
                replace_all(expression,"\\","");
                Node* v = new Node(true,i.first,{});
                Node* u = new Node(false,"",{{v,expression}});
                expression_graphes.push(new Graph(u,v));
                is_prev_expression = true;
                if(!operations.empty() && operations.top() == "and"){
                    calculate(expression_graphes,operations);
                    // is_prev_expression = false;
                }
            }            
        }
        
        // cout << expression_graphes.size() <<" "<< operations.size() <<"\n";
        // cout << operations.top();
        while(expression_graphes.size() > 1){
            // cout << operations.top() << "\n";
            calculate(expression_graphes,operations);
        }   
        // cout << operations.size() << " " << expression_graphes.size() << "\n";
        // assert(expression_graphes.size() > 0);
        graphes.push_back(expression_graphes.top());
    }
}



void NDFA::print_path(Node* node){
    if(st.find(node) != st.end()) return;
    st.insert(node);
    for(pair<Node*,string> next : node->next){
        cout <<"|"<< next.first->accepted<<"|" <<"("<< next.second << ")";
        print_path(next.first);
        cout << "\n";
    }
}

void NDFA::print_graph(vector<Graph*>& graphes){
    for(Graph* graph : graphes){
        print_path(graph->head);
        cout << "\n-----------------------------------\n";
    }
}