#ifndef PARSER_H
#define PARSER_H

#include <bits/stdc++.h>
#include "Utilities.h"
#include "LexicalParser.h"

using namespace std;

class Parser{

private:

    /**
     * add a new keyword.
     */


    void add_keywords(string s, vector<string>& keywords);


    /*
    * add a pucuation.
    */

    void add_puncuations(string s, vector<char>& puncuations);
public:

    void parseGrammer(map<string,vector<string>>& regularExpression, map<string,vector<string>>& regularDefinition, vector<char>& puncuations, vector<string>& keywords);
   
    void parseProgram(vector<string>& keywords, vector<char>& puncuations, Entity* head);


    /**
     * remove nested regualr definitions.
     */

    void expand_nested_regular_defenition(map<string,vector<string>>& regularDefinition);

    /**
     * replace regular definitions in regular expression with its values.
     */
    void evaluate_regular_expressions(map<string,vector<string>>& regularExpression, map<string,vector<string>>& regularDefinition);

};

#endif