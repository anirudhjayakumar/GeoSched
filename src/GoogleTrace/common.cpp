/*
 * common.cpp
 *
 *  Created on: Mar 25, 2015
 *      Author: Anirudh Jayakumar
 */

#include "common.h"

std::vector<std::string> &Utils::Tokenize(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

