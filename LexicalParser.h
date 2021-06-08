#ifndef LEXICAL_PARSER_H
#define LEXICAL_PARSER_H

#include <string>
#include <vector>
#include "Entity.h"


using namespace std;



class LexicalParser{
private:
    int index;
    Entity* entity;
public:
    LexicalParser(Entity* head);
    string next_token(string& path, vector<string>& keywords, vector<char>& puncuations);

    bool has_next(string& path);

    void resetIndex();
};

#endif