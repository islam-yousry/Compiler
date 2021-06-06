#ifndef LEXICAL_PARSER_H
#define LEXICAL_PARSER_H

#include <string>
#include <vector>
#include "Entity.h"


using namespace std;



class LexicalParser{

public:
    void trace(string path, vector<string>& keywords, vector<char>& puncuations, Entity* head);

};

#endif