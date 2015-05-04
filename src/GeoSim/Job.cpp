/*
 * Job.cpp
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#include "Job.h"
#include "Node.h"
#include "common.h"



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

std::vector<Task*>& Job:: getTasks(){
 return vTasks;
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


int Job:: setRetireTime(){
	nRetireTime= nCurrRunTime + nTotalRunTime;
	return SUCCESS;
}
int Job:: setCurrTime(INT64_ arrival){
		nCurrRunTime = arrival;
		setRetireTime();
		return SUCCESS;
}

INT64_ Job:: getRetireTime(){
	return nRetireTime;
}

INT64_ Job:: getCurrTime(){
	return nCurrRunTime;
}

int Job:: INcCurrTime(INT64_ timestamp){
		nCurrRunTime += timestamp ;
		return SUCCESS;
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
void Task::setNode(Node* node){
	pNode = node ;
}

 Job* Task::getJob(){
	return pJob;
}
 map<int, int> Task::getPossibleNodes(){
	 return possibleNode;
 }
int Task::setPossibleNodes(map<int,int> pN){
 	  possibleNode = pN;
 	  return SUCCESS;
  }




