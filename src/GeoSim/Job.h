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
    map<int, int> getPossibleNodes();
    int setPossibleNodes(map<int,int> pN);

	//add required methods
private:
	int NodeID;
	int mem;
	int cpu;
	Node *pNode;
	Job *pJob;
	map<int,int> possibleNode;
};

class Job {
public:
	Job(INT64_, INT64_, int,  int , double , double);
	virtual ~Job();
	INT64_ getJobID();
	int sClass();
	int setRetireTime();
	int setCurrTime(INT64_ arrival);
	INT64_ getRetireTime();
	INT64_ getCurrTime();
	int  INcCurrTime(INT64_ timestamp);
	std::vector<Task*>& getTasks();
	// add required method
private:
	std::vector<Task*> vTasks;
	INT64_ nTotalRunTime;
	INT64_ nRetireTime;
	INT64_ nCurrRunTime;
	INT64_ nJobID;
	int    nSchedClass;
};

#endif /* JOB_H_ */

