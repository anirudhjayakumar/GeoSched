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
#include "Job.h"
using namespace std;
#include <fstream>
#include <iostream>
const string machineConfig("/Users/harshitdokania/Desktop/cs525/machine_config.csv");
DataCenter::DataCenter(int id, const std::string &filePath) {
	// TODO Auto-generated constructor stub
	nDCid= id;

    string sLine;
    int nodeCount;
    int cpuCount;
    int memAvail;
	ifstream fileStream(machineConfig);
	int nodeID=0;
	workLoad = new GoogleTrace();
	while(!fileStream.eof()){
		vector<int> vTokens;
		fileStream >> sLine;

		Tokenize(sLine,',',vTokens);
	    nodeCount = vTokens[0];
	    cpuCount = vTokens[1];
		memAvail = vTokens[2];

		Node* node = new Node[nodeCount];
		for(int i=0;i<nodeCount; i++){
			node[i].nNodeID = nodeID;
			node[i].nTotalCPU = cpuCount;
			node[i].nTotalMem = memAvail;
			node[i].nFreeCPU = cpuCount;
			node[i].nFreeMem = memAvail;
			m_mapNodes[nodeID]=&node[i];
            nodeID++;
		}
	}
	//cout<<"Started tracing"<<endl;
	//cout <<filePath;
	workLoad->Initialize(filePath);
	m_mapDCtoResource[nDCid]=m_mapNodes;
}

DataCenter::~DataCenter() {
	// TODO Auto-generated destructor stub
}

int DataCenter::Initialize(std::vector<DataCenterProxy *> dataCenterProxies, const std::string &resourceXML)
{
	return SUCCESS;
}

int DataCenter::AddJobsToWaitingList(Job *pJob)
{
	m_vWaitingJobs.push_back(pJob);
	cout<<"Added to wait list\n";
	return SUCCESS;
}

void DataCenter::PrintUtilization()
{
	return;
}

std::unordered_map<int,Node*> DataCenter::GetResourceData()
{
	return m_mapNodes;
}
GoogleTrace* DataCenter::getWorkLoad(){
	return workLoad;
}

void DataCenter::StartSimulation()
{
	INT64_ arrivalTime=30000000;
	while(arrivalTime<1000000000000){
		vector<TraceItem*> currLoad = workLoad->GetNextSet(arrivalTime);
		if(currLoad.size())


		//cout<<"Vector Size = "<<currLoad.size()<<endl;

		for (auto it = currLoad.begin(); it != currLoad.end(); it++) {
		//
			Job* pJob= (*it)->createJob();
			//cout<<"while pushing"<<endl;
			AddJobsToWaitingList(pJob);
			//m_vWaitingJobs.push_back(pJob);

			//cout<<pJob->getJobID()<<endl;
		    //cout << (*it)->ArrivalTime()<<"\t"<<(*it)->RunningTime() << endl;
		}
		//if(arrivalTime%500000000 > 45000000)
		 UpdateResourceData();
		ScheduleJobsFromWaitingList();
		cout<<"FINISHED\n";






		//cout<<"arrivalTime = "<<arrivalTime<<" thread ID = "<<this_thread::get_id()<<endl;
//		for(auto)
		arrivalTime+= 30000000;
	}

	 cout << "Simulation Started"<<endl;
	return;
}

void DataCenter::UpdateResourceData(){

	for(int i=0; i<5;i++){
		cout<<"Updating Resource\n";


		//m_mapDCtoResource[i+1]=NULL;
	 m_mapDCtoResource[i+1]= m_dataCenterProxies[i].GetResourceData();
	 cout<<" After Updating Resource  " <<i<<endl;
	}

}


void DataCenter::Simulation(){
	m_pThread = new std::thread(&DataCenter::StartSimulation, this);
}

void DataCenter::ScheduleJobsFromWaitingList(){
	//UpdateResourceData();


  cout <<"Scheduling from waiting list"<<endl;
  if(m_vWaitingJobs.size()){
   Job* pJob=m_vWaitingJobs.front();
   m_vWaitingJobs.pop_front();

  int dcID=rand()%6;
  if(dcID==nDCid || pJob->sClass()>1){
	  cout<<"I am the DataCenter"<<endl;


  }
  else
  m_dataCenterProxies[dcID-1].SubmitJob(pJob);
  }







}
void DataCenter::set_dataCenterProxies(DataCenterProxy *proxy){
	//std::map<int,DataCenterProxy *> m_dataCenterProxies;
		m_dataCenterProxies = proxy;
}
