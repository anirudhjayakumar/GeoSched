/*
 * Job.h
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#ifndef JOB_H_
#define JOB_H_
#include "common.h"
#include <vector>
class Job;
class Node;
class Task {
public:
	Task();
	virtual ~Task();
	//add required methods
private:
	int NodeID;
	int mem;
	int cpu;
	Node *pNode;
	Job *pJob;
};

class Job {
public:
	Job();
	virtual ~Job();
	// add required method
private:
	std::vector<Task*> vTasks;
	INT64_ nTotalRunTime;
	INT64_ nCurrRunTime;
	INT64_ nJobID;
	int    nSchedClass;
};

#endif /* JOB_H_ */

