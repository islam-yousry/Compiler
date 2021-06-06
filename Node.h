#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

using namespace std;
struct Node{
    bool accepted;
    string token;
    vector<pair<Node*,string>> next;
    Node(bool accepted,string token, vector<pair<Node*,string>> next):accepted(accepted), token(token),next(next){}
};

#endif