/*
 * Node.h
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#ifndef NODE_H_
#define NODE_H_
#include "Job.h"
#include <vector>

class Node {
public:
	Node();
	virtual ~Node();
	// deal with integers. will be faster
	// always use ceil the value
	int nTotalCPU;
	int nTotalMem;
	int nFreeCPU;
	int nFreeMem;
	int nNodeID;
	std::vector<Task*> vTasks;
	int getFreeCPU();
	int  getFreeMem();
	int  getNodeID();
	int increaseCPU(int cpu);
	int decreaseCPU(int cpu);
	int increaseMem(int mem);
    int decreaseMem(int mem);
    int getTotalCPU();
    int getTotalMem();


};

#endif /* NODE_H_ */
