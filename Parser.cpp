
#include "Parser.h"

using namespace std;


void Parser::add_keywords(string s, vector<string>& keywords){
    addspace(s);
    vector<string> keyword = divide_on(s.substr(s.find('{')+1, (s.find('}')-1-s.find('{'))),' ');
    keywords.insert(keywords.end(),keyword.begin(),keyword.end());
}


void Parser::add_puncuations(string s, vector<char>& puncuations){
    addspace(s);
    for(char c : s){
        if(c == '[' || c == '\\' || c == ' ' || c == ']') continue;
        puncuations.push_back(c);
    }
}


void Parser::parseGrammer(map<string,vector<string>>& regularExpression, map<string,vector<string>>& regularDefinition, vector<char>& puncuations, vector<string>& keywords){
    ifstream inputGrammer( "inputGrammer.txt");

    for( string line; getline( inputGrammer, line ); ){
        if(find(line.begin(),line.end(),'[') != line.end()){
            add_puncuations(line,puncuations);
        }
        else if(find(line.begin(),line.end(),'{') != line.end()){
            add_keywords(line,keywords);
        }
        else if(find(line.begin(),line.end(),':') != line.end()){
            addspace(line);
            regularExpression[line.substr(0,line.find(':'))] = divide_on(line.substr(line.find(':')+1),' ');
        }
        else {
            replace_all(line," ","");
            replace_all(line,"|"," | ");
            regularDefinition[line.substr(0,line.find('='))] = expand(divide_on(line.substr(line.find('=')+1),' '));
        }
    }
}

void Parser::parseProgram(vector<string>& keywords, vector<char>& puncuations, Entity* head){
    ifstream inputProgram( "inputProgram.txt");
    LexicalParser LexicalParser;
    freopen("Lexical Analyzer Output.txt","w",stdout);
    for( string line; getline(inputProgram, line); ){
        LexicalParser.trace(line,keywords,puncuations,head);
    }
}


void Parser::expand_nested_regular_defenition(map<string,vector<string>>& regularDefinition){
    for(auto& i : regularDefinition){
        for(int j = 0; j < i.second.size(); j++){
            string definition = i.second[j];
            for(auto it : regularDefinition){
                if(replace_all(definition,it.first," "+it.first+" ")){
                    i.second[j] = definition;
                    vector<string> divided = divide_on(definition,' ');
                    i.second.insert(find(i.second.begin(),i.second.end(),definition),divided.begin(),divided.end());
                    i.second.erase(find(i.second.begin(),i.second.end(),definition));
                    definition = i.second[j];
                }
            }
            if(regularDefinition.find(definition) != regularDefinition.end()){
                i.second.insert(find(i.second.begin(),i.second.end(),definition),regularDefinition[definition].begin(),regularDefinition[definition].end());
                i.second.erase(find(i.second.begin(),i.second.end(),definition));
            }
        }
    }
}


void Parser::evaluate_regular_expressions(map<string,vector<string>>& regularExpression, map<string,vector<string>>& regularDefinition){
    for(auto& i: regularExpression){
        for(int j = 0; j < i.second.size(); j++){
            string expression = i.second[j];
            if(regularDefinition.find(expression) != regularDefinition.end()){
                i.second.insert(find(i.second.begin(),i.second.end(),expression),regularDefinition[expression].begin(),regularDefinition[expression].end());        
                i.second.erase(find(i.second.begin(),i.second.end(),expression));
            }
        }
    }
}