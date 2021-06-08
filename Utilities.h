#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <set>
#include <string>

using namespace std;


const string EPSON = "\\L";


/**
 * add some space to some certain keywords.
 */

void addspace(string& s);


/**
 * this function expands ranged values. 
 * example 0-4 --> 0,1,2,3,4.
 */

vector<string> expand(vector<string> v);


/**
 * divide the given string s into parts on the divider char.
 */

vector<string> divide_on(string s, char divider);


/** replace all matched string with replace string.
 * 
 */

bool replace_all(string& s, string match, string replace);

/**
 * truncate given string.
 */
void truncate(string& s);

#endif