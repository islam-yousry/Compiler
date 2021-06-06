#include <vector>
#include <set>
#include <string>
#include "Utilities.h"

using namespace std;

void addspace(string& s){
    string regex = "-|+*(){}:=!><";
    string tmp = "   ";
    for(char c : regex){
        tmp[1] = c;
        replace_all(s,string(1,c),tmp);  
    }
    replace_all(s,"\\ ","\\");
}




vector<string> expand(vector<string> v){
    vector<string> res;
    res.push_back("(");
    for(string s : v){
        if(s.find('-') != -1){
            res.push_back("(");
            for(char c = s[0]; c <= s[2]; c++){
                res.push_back(string(1,c));
                if(c != s[2]) res.push_back("|");
            }
            res.push_back(")");
        }
        else res.push_back(s);
    }
    res.push_back(")");
    // cout << "-------------------------------\n";
    // for(string i : res) cout << i << " ";
    // cout << "--------------------------------\n";
    return res;
}


vector<string> divide_on(string s, char divider){
    vector<string> divided;
    divided.push_back("");
    for(char c : s){
        if(c == divider){
            if(divided.back().size()) divided.push_back("");
        }
        else {
            if(c == '=' && divided.back().size()) divided.back().pop_back(); 
            divided.back().push_back(c);
        }
    }
    if(divided.back().empty()) divided.pop_back();
    return divided;
}



bool replace_all(string& s, string match, string replace){
    string res = "";
    int n = s.size();
    bool done = false;
    for(int i = 0; i < n; i++){
        if(s.substr(i,match.size()) == match){
            done = true;
            for(char r : replace){
                res.push_back(r);
            }
            i+= match.size()-1;
        }
        else res.push_back(s[i]);
    }
    s = res;
    return done;
}
