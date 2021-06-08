#include "Utilities.h"
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


void Parser::parse_input_grammer(map<string,vector<string>>& regularExpression, map<string,vector<string>>& regularDefinition, vector<char>& puncuations, vector<string>& keywords){
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

  

string Parser::get_token(){
    if(q.empty()) return "$";
    string token = q.front();
    q.pop();
    return token;
}

void Parser::parse_input_program(vector<string>& keywords, vector<char>& puncuations, Entity* head){
    ifstream inputProgram( "inputProgram.txt");
    freopen("Lexical Analyzer Output.txt","w",stdout);
    LexicalParser lexicalParser = LexicalParser(head);
    for( string line; getline(inputProgram, line); ){
        while(lexicalParser.has_next(line)){
            string token = lexicalParser.next_token(line,keywords,puncuations);
            if(token.size()){
                truncate(token);
                cout << token << "\n";
                q.push(token);
            }
        }
        lexicalParser.resetIndex();
    }
}


// left recursive
// left factoring
// generate parse table by first and follow functions


set<string> get_first(map<string,vector<vector<string>>>& production_rules, string& non_terminal){
    set<string> first;
    if(production_rules.find(non_terminal) == production_rules.end()) {
        first.insert(non_terminal);
        return first;
    }
    vector<vector<string>> terminals = production_rules[non_terminal];
    for(vector<string> terminal : terminals){
        if(production_rules.find(terminal[0]) != production_rules.end()){
            for(int i = 0; i < terminal.size(); i++){
                if(terminal[i] == non_terminal) {
                    // cout << terminal[i] << " " << non_terminal << "\n";
                    break;
                }
                set<string> f = get_first(production_rules,terminal[i]);
                first.insert(f.begin(),f.end());
                if(f.find(EPSON) != f.end()){
                    for(string s : f) cout << s << "\n";
                    cout << "\n";
                    if(i == terminal.size()-1) first.insert(EPSON);
                }else break;
            }
        }else first.insert(terminal[0]);
    }
    return first;
}

set<string> get_follow(map<string,vector<vector<string>>>& production_rules, string& s){
    set<string> follow;
    for(auto it : production_rules){
        string non_terminal = it.first;
        if(non_terminal == s) continue;
        vector<vector<string>> terminals = it.second;
        for(vector<string> terminal : terminals){
            if(find(terminal.begin(),terminal.end(),s) != terminal.end()){
                int index = find(terminal.begin(),terminal.end(),s)-terminal.begin();
                if(index == terminal.size()-1){
                    set<string> f = get_follow(production_rules,non_terminal);
                    follow.insert(f.begin(),f.end());
                }
                else if(production_rules.find(terminal[index+1]) == production_rules.end()){ // terminal.
                    follow.insert(terminal[index+1]);
                }else{ // nonterminal
                    set<string> f = get_first(production_rules,terminal[index+1]);
                    if(f.find(EPSON) != f.end()){
                        f.erase(EPSON);
                        follow.insert(f.begin(),f.end());
                        if(index+2 < terminal.size()){
                            if(production_rules.find(terminal[index+2]) == production_rules.end()){
                                follow.insert(terminal[index+2]);
                            }
                            else {
                                f = get_first(production_rules,terminal[index+2]);
                                follow.insert(f.begin(),f.end());
                            }
                        }
                    }
                }
            }  
        }
    }
    return follow;
}


void eliminate_left_recursion(map<string,vector<vector<string>>>& production_rules){
    for(auto& it : production_rules){
        string non_terminal = it.first;
        vector<vector<string>>& terminals = it.second;
        // find not left recursive terminal.
        vector<string> not_left_recursive;
        for(vector<string> terminal : terminals){
            set<string> first = get_first(production_rules,terminal[0]);
            assert(terminal.size() > 0);
            if(terminal[0] != non_terminal && first.find(non_terminal) == first.end()){ 
                not_left_recursive = terminal;
                break;
            }
        }
        assert(not_left_recursive.size() > 0);
        // find left recursive terminal and eleminate it.
        for(int i = 0; i < terminals.size(); i++){
            vector<string>& terminal = terminals[i];
            assert(terminal.size() > 0);
            set<string> first = get_first(production_rules,terminal[0]);
            if(terminal[0] == non_terminal || first.find(non_terminal) != first.end()){ // left recursion found.
                vector<string> alpha = vector<string>(terminal.begin()+1,terminal.end());
                vector<string> beta = not_left_recursive;
                assert(alpha.size() > 0);
                assert(beta.size() > 0);
                string non_terminal_dash = "\'"+non_terminal+"`"+"\'";
                beta.push_back(non_terminal_dash);
                terminal = beta;
                alpha.push_back(non_terminal_dash);
                production_rules[non_terminal_dash] = {alpha,{EPSON}};
                not_left_recursive = terminal;
            }
        }
    }
}


void Parser::generate_parsing_table(map<string,vector<vector<string>>>& production_rules){
    for(auto it : production_rules){
        string non_terminal = it.first;
        vector<vector<string>> terminals = it.second;
        for(vector<string> terminal : terminals){
            string terminal_string = "";
            for(string s : terminal){
                replace_all(s,"\'","");
                terminal_string += "\'"+s+"\'";
            }
            assert(terminal.size() > 0);
            set<string> first = get_first(production_rules,terminal[0]);
            for(string s : first){
                if(s == EPSON){
                    if(terminal.size() > 1 && production_rules.find(terminal[1]) == production_rules.end()){
                        replace_all(terminal[1],"\'", " ");
                        truncate(terminal[1]);
                        parsing_table[non_terminal][s] = non_terminal+"::="+terminal[1];
                    }else if(terminal.size() > 1){
                        set<string> f = get_first(production_rules,terminal[1]);
                        for(string st : f){
                            replace_all(st,"\'", " ");
                            truncate(st);
                            parsing_table[non_terminal][s] = non_terminal+"::="+st;
                        }
                    }
                }else{
                    replace_all(s,"\'", " ");
                    truncate(s);
                    parsing_table[non_terminal][s] = non_terminal+"::="+terminal_string;
                }
            }
            assert(terminal.size() > 0);
            if(terminal[0] == EPSON){
                set<string> follow = get_follow(production_rules,non_terminal);
                for(string s : follow){
                    replace_all(s,"\'", " ");
                    truncate(s);
                    parsing_table[non_terminal][s] = non_terminal+"::="+EPSON;
                }
            }
        }
    }
}

void Parser::print_parsing_table(){
    freopen("parsing table.txt","w",stdout);
    for(auto it : parsing_table){
        string non_terminal = it.first;
        cout << non_terminal << " |" << "\n";
        for(auto itt : it.second){
            string input = itt.first;
            string production = itt.second;
            cout << "\t\t" << "("<<input <<")" << production << "\n"; 
        }
        cout << "\n\n";
    }
}




// EXPRESSION::=SIMPLE_EXPRESSION'relop'SIMPLE_EXPRESSION
vector<string> get_terminals_of_production(string& production){
    assert(production.size() > 0);
    string terminals_part = production.substr(find(production.begin(),production.end(),'=')-production.begin()+1);
    replace_all(terminals_part,"\'"," ");
    replace_all(terminals_part,"  "," ");
    truncate(terminals_part);
    // cout << terminals_part << "\n";
    vector<string> terminals = divide_on(terminals_part,' ');
    return terminals;
}


void print_stack(stack<string> s){
    while(s.size()){
        if(s.top() != "$") cout << s.top() << " ";
        s.pop();
    }
    cout << "\n";
}

void Parser::generate_leftmost_derivation_sentential(){
    stack<string> s;
    s.push("$");
    s.push(start);
    string token = get_token();
    print_stack(s);
    int i = 0;
    while(token != "$" && i++ < 100){
        cout << s.top() << " " << token << "\n";
        if(parsing_table.find(s.top()) != parsing_table.end() && parsing_table[s.top()].find(token) != parsing_table[s.top()].end()){ // top of stack is nonterminal.
            string non_terminal = s.top();
            s.pop();
            vector<string> terminals = get_terminals_of_production(parsing_table[non_terminal][token]);
            while(terminals.size()){
                s.push(terminals.back());
                terminals.pop_back();
            }
                 
        }
        else if(s.top() == token){
            s.pop();
            token = get_token();
        }
        else { // error.
        }
        // print_stack(s);
    }
    // for(string s: leftmost_derivation_sententials) cout << s <<"\n";
}

// struct t{
//     vector<string>
//     vector<t> v;

// };

// void left_factoring(map<string,vector<vector<string>>>& production_rules){

// }

void Parser::parse_CFG(){
    ifstream inputCFG( "CFG Input.txt");
    map<string,vector<vector<string>>> production_rules;
    string prev_non_terminal = "";
    for(string line; getline(inputCFG,line); ){
        string non_terminal = prev_non_terminal;
        replace_all(line,"::"," :: ");
        replace_all(line,"="," = ");
        truncate(line);
        if(line[0] == '#' || line.find("::") != -1){
            if(line[0] == '#') line[0] = ' ';
            truncate(line);
            assert(line.size() > 0);
            non_terminal = line.substr(line[0] == '#'?1:0,find(line.begin(),line.end(),' ')-line.begin());
            replace_all(non_terminal," ","");
            if(start == "") start = non_terminal;
            production_rules[non_terminal] = {};
        }
        assert(line.size() > 0);
        string non_terminal_value = line.substr((non_terminal==prev_non_terminal)?0:find(line.begin(),line.end(),'=')-line.begin()+1);
        truncate(non_terminal_value);
        vector<string> terminals = divide_on(non_terminal_value,'|');
        for(string& terminal : terminals) {
            truncate(terminal);
            vector<string> v = divide_on(terminal,' ');
            production_rules[non_terminal].push_back(v);
        }
        prev_non_terminal = non_terminal;
    }
    eliminate_left_recursion(production_rules);
    
    generate_parsing_table(production_rules);
    // print_parsing_table();
    string r = "STATEMENT_LIST`";
    set<string> f  = get_first(production_rules,r);
    for(string s : f) cout << s << " ";
    

    // for(auto it : production_rules){
    //     cout << it.first << "\n";
    //     for(vector<string> v : it.second){
    //         for(string s : v) cout << s << " ";
    //         cout << "\n";
    //     }
    //     cout << "\n";
    // }

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