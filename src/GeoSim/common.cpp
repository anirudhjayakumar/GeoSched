/*
 * common.cpp
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#include "common.h"
//#include<stdlib.h>
#include <thread>
using namespace std;


vector<string> &Tokenize(const string &s, char delim, vector<string> &elems)
{
	stringstream ss(s);

	string item;
	while (getline(ss, item, delim)) {
	   elems.push_back(item);
	}
	return elems;
}

vector<int> &Tokenize(const string &s, char delim, vector<int> &elems)
{
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
	   elems.push_back(stoi(item));
	}
	return elems;
}
