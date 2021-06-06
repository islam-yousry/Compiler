#ifndef NDFA_H
#define NDFA_H

#include "Graph.h"
#include <vector>
#include <set>
#include "Utilities.h"
#include <stack>
#include <map>


using namespace std;


class NDFA{
private:

    set<string> reserved{"|","+","*","(",")"};
    /**
     * calculate the result of closure of a graph.
     */


    Graph* closure(Graph* u, string expression);



    /**
     * calculate the result of disjunction of two graphes.
     */

    Graph* dis(Graph* u, Graph* v);

    /**
     * add two graphes
     */

    Graph* add(Graph* u, Graph* v);


    /**
     * calculate an operation to a graph.
     */

    void calculate(stack<Graph*>& g, stack<string>& operations);

    set<Node*> st;

public:

    /**
     * construct NonDeterministic automata from regulare expressions
     * 
     */

    void constructNDFA(map<string,vector<string>>& regularExpression, vector<Graph*>& graphes);

   

    void print_path(Node* node);


    void print_graph(vector<Graph*>& graphes);



};
#endif