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
using namespace std;
#include <map>
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
    Job();
	Job(INT64_, INT64_, int,  int , double , double);
	virtual ~Job();
	INT64_ getJobID();
    int setNodeID(int n);
    int getNodeID();
	int sClass();
	INT64_ getCurrTime();
	int  IncCurrTime(INT64_ timestamp);
	std::vector<Task*> getTasks();
	INT64_ GetTotalRunTime();
    // add required method
	double   nCores;
private:
	std::vector<Task*> vTasks;
	INT64_ nTotalRunTime;
	INT64_ nCurrRunTime;
	INT64_ nJobID;
    int nodeID;
	int    nSchedClass;
	 //neeed for energy calc
};

#endif /* JOB_H_ */

