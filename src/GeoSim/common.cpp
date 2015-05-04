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



Barrier::Barrier(std::size_t count)
{
	_count = _org_val = count;
}

void Barrier::Wait()
{
	std::unique_lock<std::mutex> lock{_mutex};
	if (--_count == 0) {
		//_count = _org_val;
		_cv.notify_all();
	} else {
		_cv.wait(lock, [this] { return _count == 0; });
	}
}


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
