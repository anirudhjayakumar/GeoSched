/*
 * common.h
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include <vector>
#include <sstream>

#include <mutex>
#include <thread>
#include  <condition_variable>



typedef long long int INT64_;
#include <iostream>

#define SUCCESS 0
#define FAIL 1
using namespace std;

std::vector<std::string> &Tokenize(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<int> &Tokenize(const std::string &s, char delim, std::vector<int> &elems);

class Barrier
{
private:
    std::mutex _mutex;
    std::condition_variable _cv;
    std::size_t _count;
    std::size_t _org_val;
    int  thread_count;
public:
    explicit Barrier(std::size_t count);
    void decrement();
    void Wait(int);
};










#endif /* COMMON_H_ */
