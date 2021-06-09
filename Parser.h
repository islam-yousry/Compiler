#ifndef PARSER_H
#define PARSER_H

#include <bits/stdc++.h>
#include "Utilities.h"
#include "LexicalParser.h"

using namespace std;

class Parser{

private:

    queue<string> q;
    string start = "";
    map<string,map<string,string>> parsing_table;
    vector<vector<string>> leftmost_derivation_sententials;
    
    /**
     * add a new keyword.
     */


    void add_keywords(string s, vector<string>& keywords);


    /*
    * add a pucuation.
    */

    void add_puncuations(string s, vector<char>& puncuations);
     

    // phase two functions.

    /**
     * get next token from the lexical analyzer output.
     */
    string get_token();


    /**
     * get first terminals of the given non terminal.
     */
    set<string> get_first(map<string,vector<vector<string>>>& production_rules, string& non_terminal);

    /**
     * get follow terminals of the given non terminal.
    */

    set<string> get_follow(map<string,vector<vector<string>>>& production_rules, string& s);

    /**
     * implement elimination left recursion algorithm.
     */
    void eliminate_left_recursion(map<string,vector<vector<string>>>& production_rules);

    /**
     * get terminals of given production.
     */
    vector<string> get_terminals_of_production(string& production);

    /**
     * print the given stack.
     */
    void print_stack(stack<string> s);

public:

    void parse_input_grammer(map<string,vector<string>>& regularExpression, map<string,vector<string>>& regularDefinition, vector<char>& puncuations, vector<string>& keywords);
   
    void parse_input_program(vector<string>& keywords, vector<char>& puncuations, Entity* head);

    void parse_CFG();

    void generate_parsing_table(map<string,vector<vector<string>>>& production_rules);

    void print_parsing_table();

    /**
     * remove nested regualr definitions.
     */

    void expand_nested_regular_defenition(map<string,vector<string>>& regularDefinition);

    /**
     * replace regular definitions in regular expression with its values.
     */
    void evaluate_regular_expressions(map<string,vector<string>>& regularExpression, map<string,vector<string>>& regularDefinition);

   void generate_leftmost_derivation_sentential();

};

#endif