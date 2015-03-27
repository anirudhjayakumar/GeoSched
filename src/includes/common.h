#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include <vector>
#include <sstream>
typedef long long int INT64_;


#define SUCCESS 0
#define FAIL 1

namespace Utils
{

	std::vector<std::string> &Tokenize(const std::string &s, char delim, std::vector<std::string> &elems);

};



#endif /* COMMON_H_ */


