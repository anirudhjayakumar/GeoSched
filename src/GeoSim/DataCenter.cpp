/*

 * DataCenter.cpp
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#include "DataCenter.h"
#include "DataCenterProxy.h"
#include "common.h"
#include "Node.h"
#include <algorithm>
#include "Job.h"
#include <fstream>
#include <iostream>
#include "ConfigAccessor.h"
#include <stdio.h>

using namespace std;

#define TIMEINC 				5000000   // 5 secs in us
#define RESOURCE_SYNC_TIME		900000000 // 900 secs in us

const string machineConfig("/Users/harshitdokania/Desktop/cs525/geosched/datacenters/machine_config.csv");

DataCenter::DataCenter(int id, const std::string &workloadPath, Barrier *pBarrier, string n, int gmtDiff) {
	// TODO Auto-generated constructor stub
	nDCid = id;
	m_sWorkloadTrace = workloadPath;
	m_pBarrier = pBarrier;
    name = n;
    m_GMT = gmtDiff;
    
    string s= "Started creating " + name + " DataCenter at " +getLocalTime() + "\n";
    L.print(s);

}

DataCenter::~DataCenter() {
	// TODO Auto-generated destructor stub
}

int DataCenter::Initialize(DataCenterProxy * dataCenterProxies,
		ConfigAccessor *pAccessor) {

	m_pAccessor = pAccessor;
	set_dataCenterProxies(dataCenterProxies);
	string sLine;
	int nodeCount;
	int cpuCount;
	int memAvail;
	ifstream fileStream(machineConfig);
	int nodeID = 0;
   

	NodeMap mapNodes;
	while (!fileStream.eof()) {
		vector<int> vTokens;
		fileStream >> sLine;
		Tokenize(sLine, ',', vTokens);
		nodeCount = vTokens[0];
		cpuCount = vTokens[1];
		memAvail = vTokens[2];

		Node* node = new Node[nodeCount];
		for (int i = 0; i < nodeCount; i++) {
			node[i].nNodeID = nodeID;
			node[i].nTotalCPU = cpuCount;
			node[i].nTotalMem = memAvail;
			node[i].nFreeCPU = cpuCount;
			node[i].nFreeMem = memAvail;
			mapNodes[nodeID] = &node[i];
			nodeID++;
		}
	}
	m_mapDCtoResource[nDCid] = mapNodes;
    string s= "Completed creating "+ name +" DataCenter at "+ getLocalTime()+"\n";
    L.print(s);
    
    

	

	m_workLoad.Initialize(m_sWorkloadTrace, name, m_GMT);

	return SUCCESS;
}

void DataCenter::AddJobsToWaitingList(Job *pJob) {
   
    m_waitMutex.lock();
	m_vWaitingJobs.push_back(pJob);
	m_waitMutex.unlock();
	//cout << "Added to wait list\n";

}

void DataCenter::PrintUtilization() {
	return;
}

list<Job*> DataCenter::GetWaitingJobs() {
	m_waitMutex.lock();
	list<Job*> ret = m_vWaitingJobs;
	m_waitMutex.unlock();
	return ret;

}

string  DataCenter:: getLocalTime(){
    return localtime(m_GMT);
}

Barrier* DataCenter:: getBarrier(){
    return m_pBarrier;
}




NodeMap DataCenter::GetResourceData() {
   // string s = name + " Getting resource data at " +getLocalTime()+"\n";
    //L.print(s);
	m_resourceMutex.lock();
	NodeMap nodeMap = m_mapDCtoResource[nDCid];
	m_resourceMutex.unlock();
	return nodeMap;
}

GoogleTrace* DataCenter::getWorkLoad() {
	return &m_workLoad;
}

void DataCenter::StartSimulation() {

	string s =name + " Entered Simulation at "+getLocalTime()+"\n";
     L.print(s);
	INT64_ arrivalTime = TIMEINC;
    int cycle_count = 1;
	// loop till the time there are jobs left in the trace
	// or till the time all jobs complete running.
	while (!m_workLoad.FileEnd() || m_vRunningJobs.size()) {
		/* Take load for this interval */

        
        vector<TraceItem*> currLoad = m_workLoad.GetNextSet(arrivalTime, name, m_GMT);
		if (currLoad.size()) {
            s= "Job Arrival at "+ name+ " size: "+to_string(currLoad.size()) +"time: "+ getLocalTime()+"\n";
           L.print(s);
			for (auto it = currLoad.begin(); it != currLoad.end(); it++) {
				//
				Job* pJob = (*it)->createJob();

				/* high sensitive jobs put in my own queue */
				if (pJob->sClass() == 2 || pJob->sClass() == 3) {
                    cout<<"High sensitive jobs at "<<name<< "time: "<<getLocalTime()<<endl;
					
					//	cout<<"job = "<<pJob->sClass()<<endl;
					//cout << "DC " << nDCid << ": JobID: " <<  pJob->getJobID() << " send to local queue due to high sensitivity" << endl;
					AddJobsToWaitingList(pJob); // schedule during schedule phase
				} else {
                    cout<<name<< "Checking resource available at time: "<<getLocalTime()<<endl;
					auto vecDCs = GetDCSchedulable(pJob);
					
					//cout << "DC " << nDCid << ": JobID: " <<  pJob->getJobID() << " Schedulable DCs";
					//for (auto iter = vecDCs.begin(); iter != vecDCs.end(); ++iter)
					//	cout << *iter << " ";
				//	cout << endl;
					MetaSchedJobToDC(vecDCs, pJob);
				}

			}
		}
		else 
		{
			// some code so that barier is not reentered before other exits
			int b,c,d=0;
        	for(int a = 0; a < 3456789 ; a++)
			{
				b = a + 4 + d;
				c = b + (rand()%5);
				d = b + c;
		}
		}

		// inc running job time and remove jobs that are done
		ProgressRunningJobs();

		// look at the jobs in the waiting queue and add them to running queue based on
		// availability of resource on local dc.
		ScheduleJobsFromWaitingList();

		arrivalTime += TIMEINC;
		if (arrivalTime % RESOURCE_SYNC_TIME == 0) {
			UpdateResourceData();
		}

		cout << "DC waits " << nDCid << endl;
		m_pBarrier->Wait(cycle_count);
		cout << "DC resumes " << nDCid << endl;
		// barrier: all threads stop here before proceeding
        cycle_count++;
	}
    
    DecreaseBarrier();
    cout << "DC exits " << nDCid << endl;
	return;
}

void DataCenter::  decrementBarrier(){
    m_pBarrier->decrement();
}


void DataCenter::MetaSchedJobToDC(std::vector<int> vecDCs, Job* pJob)
{
	// will be replaced with advanced algo. for now choose random dc id and send job
	int dc_id = rand() % vecDCs.size();
    cout<<name<<" Scheduling jobs on "<<m_dataCenterProxies[dc_id].GetName()<<"time: "<<getLocalTime()<<endl;
	m_dataCenterProxies[dc_id].SubmitJob(pJob);
}

void DataCenter:: DecreaseBarrier(){
    for (int i = 0; i < DC_COUNT; i++) {
        if (i != nDCid)
            m_dataCenterProxies[i].InformLeaving();
    }
}

vector<int> DataCenter::GetDCSchedulable(Job *pJob)
{
	vector<int> ret;
	for(int index = 0; index < DC_COUNT; ++index)
	{
		if(CheckDCFit(pJob,m_mapDCtoResource[index]))
			ret.push_back(index);
	}
	return ret;
}
string DataCenter:: GetName(){
    return name;
}

void DataCenter::ProgressRunningJobs() {
	NodeMap &availResource = m_mapDCtoResource[nDCid];
	
    list<Job*>::iterator j = m_vRunningJobs.begin();
    while (j != m_vRunningJobs.end())
    {

		Job* pJob = *j;
		pJob->IncCurrTime(TIMEINC);
		if (pJob->GetTotalRunTime() <= pJob->getCurrTime()) {
            cout <<"Job Completed at "<<name<<" time: "<<getLocalTime()<<endl;
            std::vector<Task*> Tasks = pJob->getTasks();
			// iterate through each task and reclaim the node resource
			// also remove task from node's task list
            
			for (auto t = Tasks.begin(); t != Tasks.end(); t++) {
				//reclaiming node resource
				int Tcpu = (*t)->getCpu();
				int Tmem = (*t)->getMem();
				int nodeId = (*t)->getNodeID();
				m_resourceMutex.lock();
				availResource[nodeId]->increaseCPU(Tcpu);
				availResource[nodeId]->increaseMem(Tmem);
                cout<<name<<" Free the resource and delete tasks "<<getLocalTime()<<endl;
                				//removing task from node task list
				availResource[nodeId]->vTasks.erase(remove(availResource[nodeId]->vTasks.begin(), availResource[nodeId]->vTasks.end(), *t), availResource[nodeId]->vTasks.end());
               
				m_resourceMutex.unlock();

			}
			delete pJob;
			j = m_vRunningJobs.erase(j);
		}
        else
        {
            j++;
        }
	}
    
}

void DataCenter::UpdateResourceData() {

	for (int i = 0; i < DC_COUNT; i++) {
		if (i != nDCid)
			m_mapDCtoResource[i] = m_dataCenterProxies[i].GetResourceData();
	}
    string s = name + " Completed Updating Resource at " + getLocalTime()+"\n";
    L.print(s);
}

void DataCenter::Join() {
	m_pThread->join();
}

void DataCenter::Simulation() {
	m_pThread = new std::thread(&DataCenter::StartSimulation, this);
}

bool DataCenter::CheckDCFit(Job *pJob, NodeMap &nodeMap) {
	vector<Task*> fit;
	vector<Task *> vTasks = pJob->getTasks();
	for (auto node_iter = nodeMap.begin(); node_iter != nodeMap.end();
			++node_iter) {
		// check if task can fit into this node
		for (auto task_iter = vTasks.begin(); task_iter != vTasks.end();
				task_iter++) {
			// if element not in fit
			if (find(fit.begin(), fit.end(), *task_iter) == fit.end()) {
				if ((*task_iter)->getCpu() <= node_iter->second->getFreeCPU() && // cpu
						(*task_iter)->getMem() <= node_iter->second->getFreeMem()) //mem
					fit.push_back(*task_iter);

			}
		}
		if (fit.size() == vTasks.size())
			return true;
	}
	return false;
}

int DataCenter::ScheduleJob(Job *pJob) {
	NodeMap &nodeMap = m_mapDCtoResource[nDCid];
	vector<Task*> fit;
	vector<Task *> vTasks = pJob->getTasks();
	for (auto node_iter = nodeMap.begin(); node_iter != nodeMap.end();
			++node_iter) {
		// check if task can fit into this node
		for (auto task_iter = vTasks.begin(); task_iter != vTasks.end();
				task_iter++) {
			// if element not in fit
			if (find(fit.begin(), fit.end(), *task_iter) == fit.end()) {
				if ((*task_iter)->getCpu() <= node_iter->second->getFreeCPU() && // cpu
						(*task_iter)->getMem() <= node_iter->second->getFreeMem()) //mem
								{
					// scheduling time
					(*task_iter)->setNodeID(node_iter->second->getNodeID());
					int CPU = (*task_iter)->getCpu();
					int MEM = (*task_iter)->getMem();
					m_resourceMutex.lock();
					node_iter->second->decreaseCPU(CPU);
					node_iter->second->decreaseMem(MEM);
					node_iter->second->vTasks.push_back(*task_iter);
					m_resourceMutex.unlock();
					fit.push_back(*task_iter);
				}
			}
		}

		if (fit.size() == vTasks.size()) {
			break; // no more nodes needed to be checked
		}
	}

	if (fit.size() == vTasks.size()) {

		m_vRunningJobs.push_back(pJob);
		RemoveJobFromWaitingQueue(pJob);
		//cout << "Finnished scheduling job " << pJob->getJobID() << endl;
	}
	else {
		// remove assigned tasks from nodes and reclaim resources
		for (auto t = fit.begin(); t != fit.end(); t++) {
			//reclaiming node resource
			int Tcpu = (*t)->getCpu();
			int Tmem = (*t)->getMem();
			int nodeId = (*t)->getNodeID();
			m_resourceMutex.lock();
			nodeMap[nodeId]->increaseCPU(Tcpu);
			nodeMap[nodeId]->increaseMem(Tmem);
			//removing task from node task list
			nodeMap[nodeId]->vTasks.erase(remove(nodeMap[nodeId]->vTasks.begin(), nodeMap[nodeId]->vTasks.end(), *t), \
					nodeMap[nodeId]->vTasks.end());

			m_resourceMutex.unlock();

		}
	//	cout << "Unable to schedule job " << pJob->getJobID() << endl;
	}
	return SUCCESS;
}

void DataCenter::RemoveJobFromWaitingQueue(Job* pJob)
{
	m_waitMutex.lock();
	m_vWaitingJobs.remove(pJob);
	m_waitMutex.unlock();
}


void DataCenter::ScheduleJobsFromWaitingList() {

	// get reference of local resources
	NodeMap &availResource = m_mapDCtoResource[nDCid];
	// get COPY of waiting jobs
	std::list<Job*> waitJobs = GetWaitingJobs();

	for (auto iter = waitJobs.begin(); iter != waitJobs.end(); ++iter) {
		Job* pJob = *iter;
		if (CheckDCFit(pJob, availResource))
		{
			ScheduleJob(pJob);
		}
	}
}

void DataCenter::set_dataCenterProxies(DataCenterProxy *proxy) {

m_dataCenterProxies = proxy;
}
string DataCenter:: localtime(int i){
    
    string Time;
    time_t m_Time;
    struct tm * ptm;
    
    time ( &m_Time );
    ptm = gmtime(&m_Time);
    if(ptm->tm_hour+i<=0){
        Time=to_string(ptm->tm_hour+i+12);
    }
    else
        Time=to_string((ptm->tm_hour+i)%24);
    Time+=":";
    if((ptm->tm_min)<9) {
        Time+="0" ;
    }
    Time+=to_string((ptm->tm_min));
    Time+=":";
    if((ptm->tm_sec)<9){
        Time+="0" ;
    }
    Time+=to_string((ptm->tm_sec));
    return Time;
}

