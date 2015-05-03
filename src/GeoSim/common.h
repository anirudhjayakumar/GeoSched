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
#include <cstdlib>

typedef long long int INT64_;


#define SUCCESS 0
#define FAIL 1


std::vector<std::string> &Tokenize(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<int> &Tokenize(const std::string &s, char delim, std::vector<int> &elems);





#endif /* COMMON_H_ */
