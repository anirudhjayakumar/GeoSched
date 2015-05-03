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
	int getNodeID();
	void setNodeID(int id);
	int getMem();
	void setMem(int m);
	int getCpu();
	void setCpu(int cp);
	Node* getNode();
	void setNode(Node* node);
	Job* getJob();
	void setJob(Job* job);


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
	Job(INT64_, INT64_, int,  int , double , double);
	virtual ~Job();
	INT64_ getJobID();
	int sClass();
	// add required method
private:
	std::vector<Task*> vTasks;
	INT64_ nTotalRunTime;
	INT64_ nCurrRunTime;
	INT64_ nJobID;
	int    nSchedClass;
};

#endif /* JOB_H_ */

