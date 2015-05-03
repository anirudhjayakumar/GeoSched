/*
 * Job.cpp
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#include "Job.h"
#include "Node.h"
#include "common.h"

std::vector<Task*> vTasks;
	INT64_ nTotalRunTime;
	INT64_ nCurrRunTime;
	INT64_ nJobID;
	int    nSchedClass;

Job::Job(INT64_ id, INT64_ runtime, int sClass, int tasks, double cpu, double mem) {
	// TODO Auto-generated constructor stub
   nJobID = id;
   nSchedClass = sClass;
   nTotalRunTime = runtime;
   nCurrRunTime = 0;
   double myCpu = cpu/tasks;
   double myMem = mem/tasks;
   for(int i=0; i<tasks ; i++){
	   Task *t= new Task;
	   t->setMem(myMem);
	   t->setCpu(myCpu);
	   t->setJob(this);
	   vTasks.push_back(t);
   }
}

Job::~Job() {
	// TODO Auto-generated destructor stub
}
INT64_ Job:: getJobID(){
	return nJobID;
}

int Job::sClass(){
	return nSchedClass;
}
Task::Task() {
	// TODO Auto-generated constructor stub

}

Task::~Task() {
	// TODO Auto-generated destructor stub
}

int Task::getNodeID(){
	return NodeID;
}
void Task::setNodeID(int node){
	 NodeID = node ;
}
int Task::getMem(){
	return mem;
}

void Task::setMem(int m){
	 mem = m ;
}
int Task::getCpu(){
	return cpu;
}
void Task::setCpu(int cp){
	 cpu = cp ;
}
Node* Task::getNode(){
	return pNode;
}
void Task::setJob(Job *job){
	 pJob = job ;
}
 Job* Task::getJob(){
	return pJob;
}




