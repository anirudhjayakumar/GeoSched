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
#include <mutex>
#include <thread>

typedef long long int INT64_;
#include <iostream>

#define SUCCESS 0
#define FAIL 1
using namespace std;

std::vector<std::string> &Tokenize(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<int> &Tokenize(const std::string &s, char delim, std::vector<int> &elems);
/*
class Barrier
{
private:
    static Barrier* barrier;
    std::mutex _mutex;
    std::condition_variable _cv;
    std::size_t _count;
public:
    explicit Barrier(std::size_t count) : _count{count} { }
    virtual ~Barrier() ;
    void Wait();
    static Barrier* getBarrier(size_t count);
};

Barrier* Barrier::barrier=0;
Barrier::~Barrier(){

}
 Barrier* Barrier::getBarrier(size_t count) {
                     if(!barrier) {
                              barrier = new Barrier(count);
                              cout << "getBarrier(): First instance\n";
                              return barrier;
                      }
                      else {
                              cout << "getBarrier(): previous instance\n";
                              return barrier;
                      }
    }

    void Barrier:: Wait()
    {
        std::unique_lock<std::mutex> lock{_mutex};
        if (--_count == 0) {
            _cv.notify_all();
        } else {
            _cv.wait(lock, [this] { return _count == 0; });
        }
    }


*/



#endif /* COMMON_H_ */
