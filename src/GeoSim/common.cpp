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
    thread_count = 0;
	_count = _org_val = count;
}

void Barrier::Wait(int local_count)
{
    
	std::unique_lock<std::mutex> lock{_mutex};
	if (--_count == 0) {
		//cout << "barrier:" << _count << endl; 
		_count = _org_val;
        thread_count = local_count;
		_cv.notify_all();
	} else {
		//cout << "barrier:" << _count << endl; 
		_cv.wait(lock, [this,local_count] { return thread_count == local_count; });
	}
}

void Barrier:: decrement(){
    std::unique_lock<std::mutex> lock{_mutex};
    --_org_val;
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

