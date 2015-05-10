/*
 * Job.cpp
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#include "Job.h"
#include "Node.h"
#include "common.h"
using namespace std;

Job::Job()
{
}

Job::Job(INT64_ id, INT64_ runtime, int sClass, int tasks, int cpu, int mem) {
	// TODO Auto-generated constructor stub
   nJobID = id;
   nSchedClass = sClass;
   nTotalRunTime = runtime;
   nCurrRunTime = 0;
   nCores = cpu;
   int myCpu = (cpu)/tasks;
   int myMem = (mem)/tasks;
   for(int i=0; i<tasks ; i++){
	   Task *t= new Task();
	   t->setMem(myMem);
	   t->setCpu(myCpu);
	   t->setJob(this);
	   vTasks.push_back(t);
   }
}

vector<Task*> Job:: getTasks(){
 vector<Task*> tasks = vTasks;
 return tasks;
}

Job::~Job() {
	for(auto iter = vTasks.begin(); iter != vTasks.end();)
	{
		Task *pTask = *iter;
        iter++;
		delete pTask;

	}
    vTasks.clear();
    
}
INT64_ Job:: getJobID(){
	return nJobID;
}

int Job:: setNodeID(int n){
    nodeID = n;
    return SUCCESS;
    
}

int Job:: getNodeID(){
    return nodeID;
    
}

int Job::sClass(){
	return nSchedClass;
}

INT64_ Job::GetTotalRunTime()
{
    return nTotalRunTime;
}


INT64_ Job:: getCurrTime(){
	return nCurrRunTime;
}

int Job:: IncCurrTime(INT64_ timestamp){
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




