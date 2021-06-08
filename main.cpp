#include <bits/stdc++.h>
#include "Parser.h"
#include "NDFA.h"
#include "MinimizedDFA.h"
#include "DFA.h"
#include "LexicalParser.h"


using namespace std;



int main(){

    
    map<string,vector<string>> regularExpression, regularDefinition;
    vector<string> keywords;
    vector<char> puncuations;
    vector<Graph*> graphes;
    
    Parser parser = Parser();
    parser.parse_CFG();
    parser.parse_input_grammer(regularExpression,regularDefinition,puncuations,keywords);
    // construct_parsing_table(regularExpression);
    parser.expand_nested_regular_defenition(regularDefinition);
    parser.evaluate_regular_expressions(regularExpression,regularDefinition);

    NDFA ndfa = NDFA();
    ndfa.constructNDFA(regularExpression,graphes);
    
    // print_graph(graphes);

    DFA dfa = DFA();
    dfa.construct_DFA(graphes);
//    print_table();
    
    MinimizedDFA minimizedDFA = MinimizedDFA();
    minimizedDFA.minimize(regularExpression,dfa.get_table(),dfa.get_inputs());
    minimizedDFA.construct_minimized_DFA(dfa.get_table());
    minimizedDFA.print_minimzed_DFA(dfa.get_inputs());

    parser.parse_input_program(keywords,puncuations,minimizedDFA.get_head());
    parser.generate_leftmost_derivation_sentential();
    return 0;
}