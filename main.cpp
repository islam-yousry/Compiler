#include <bits/stdc++.h>
#include "Parser.h"
#include "NDFA.h"
#include "MinimizedDFA.h"
#include "DFA.h"
#include "LexicalParser.h"


using namespace std;

map<string,vector<string>> regularExpression, regularDefinition;
vector<string> keywords;
vector<char> puncuations;

vector<Graph*> graphes;

int main(){
    Parser parser = Parser();
    parser.parseGrammer(regularExpression,regularDefinition,puncuations,keywords);
    parser.expand_nested_regular_defenition(regularDefinition);
    parser.evaluate_regular_expressions(regularExpression,regularDefinition);

    NDFA ndfa = NDFA();
    ndfa.constructNDFA(regularExpression,graphes);
    
    // print_graph(graphes);

    DFA dfa = DFA();
    dfa.constructDFA(graphes);
//    print_table();
    
    MinimizedDFA minimizedDFA = MinimizedDFA();
    minimizedDFA.minimize(regularExpression,dfa.table,dfa.inputs);
    minimizedDFA.constructMinimizedDFA(dfa.table);
    minimizedDFA.print_minimzed_DFA(dfa.inputs);

    parser.parseProgram(keywords,puncuations,minimizedDFA.head);

    return 0;
}