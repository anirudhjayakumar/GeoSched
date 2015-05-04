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

	workLoad->Initialize(filePath);
	m_mapDCtoResource[nDCid]=m_mapNodes;

	/*fix this */
	//barrier= Barrier::getBarrier(5);


}

DataCenter::~DataCenter() {
	// TODO Auto-generated destructor stub
}

int DataCenter::Initialize(std::vector<DataCenterProxy *> dataCenterProxies, const std::string &resourceXML)
{
	return SUCCESS;
}

void DataCenter::AddJobsToWaitingList(Job *pJob)
{
	try{
		std::lock_guard<std::mutex> lock(m_waitMutex);
		m_vWaitingJobs.push_back(pJob);
		cout<<"Added to wait list\n";
		//return SUCCESS;
	}
	catch(...){
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void DataCenter::PrintUtilization()
{
	return;
}
std::list<Job*>&   DataCenter::GetWaitingJobs()
{  try{
	std::lock_guard<std::mutex> lock(m_waitMutex);
	return m_vWaitingJobs;
      }
catch(...){
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

}

std::unordered_map<int,Node*>& DataCenter::GetResourceData()
{  try{
	std::lock_guard<std::mutex> lock(m_resourceMutex);
	return m_mapNodes;
      }
catch(...){
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

}
GoogleTrace* DataCenter::getWorkLoad(){
	return workLoad;
}

void DataCenter::StartSimulation()
{

	std::this_thread::sleep_for(std::chrono::milliseconds(1*nDCid));
	cout << "Simulation Started"<<endl;
	INT64_ arrivalTime=30000000;
	while(arrivalTime<10000000000){
		/* Take load for this interval */
		vector<TraceItem*> currLoad = workLoad->GetNextSet(arrivalTime);
		if(currLoad.size()){



			for (auto it = currLoad.begin(); it != currLoad.end(); it++) {
				//
				Job* pJob= (*it)->createJob();

				/* high sensitive jobs put in my own queue */
				if(pJob->sClass()==2 || pJob->sClass()==3){
					//	cout<<"job = "<<pJob->sClass()<<endl;
					AddJobsToWaitingList(pJob);
					std::vector<Task*> Tasks = pJob->getTasks();
					for(auto t = Tasks.begin(); t != Tasks.end(); t++){
						int Tcpu = (*t)->getCpu();
						int Tmem = (*t)->getMem();
						for(auto i=m_mapNodes.begin();i!=m_mapNodes.end();i++)
						{
							if ((*i).second->getFreeCPU()>= Tcpu && (*i).second->getFreeMem()>=Tmem){
								(*i).second->decreaseCPU(Tcpu);
								(*i).second->decreaseMem(Tmem);

								(*t)->setNodeID((*i).second->getNodeID());
								(*t)->setNode((*i).second);
								cout<<"While writing Node Id from own "<<"\t"<<(*t)->getNodeID()<<endl;
								break;
							}

						}
						(*t)->setJob(pJob);


					}
				}
				else{
					std::vector<Task*> Tasks = pJob->getTasks();
					bool avail[]={true,true,true,true, true};

					for(auto t = Tasks.begin(); t != Tasks.end(); t++){
						int Tcpu = (*t)->getCpu();
						int Tmem = (*t)->getMem();
						map<int, int> pN;
						for(int i=0;i<m_mapDCtoResource.size();i++){
							for(int n=0;n < m_mapDCtoResource[i].size();n++){
								if (m_mapDCtoResource[i][n]->getFreeCPU()>= Tcpu && m_mapDCtoResource[i][n]->getFreeMem()>=Tmem){
									m_mapDCtoResource[i][n]->decreaseCPU(Tcpu);
									m_mapDCtoResource[i][n]->decreaseMem(Tmem);
									pN[i]=n;
									break;
								}
								else if(n==m_mapDCtoResource[i].size()-1){
									avail[i]=false;
									pN[i]=0;
								}

							}
						}
						(*t)->setPossibleNodes(pN);
						(*t)->setJob(pJob);

					}
					int executed;
					for(int i=0;i<5;i++){
						if(avail[i]){
							m_dataCenterProxies[i].SubmitJob(pJob);
							executed = i;
						}

					}
					// return the nodes which we didn't use

					for(auto t = Tasks.begin(); t != Tasks.end(); t++){
						map<int,int> pN = (*t)->getPossibleNodes();
						for(int i=0;i<5;i++){
							if(i==executed){
								(*t)->setNodeID(m_mapDCtoResource[i][pN[i]]->getNodeID());
								(*t)->setNode(m_mapDCtoResource[i][pN[i]]);
								cout<<"While writing Node Id from random "<<"\t"<<(*t)->getNodeID()<<endl;
							}
							else{
							   m_mapDCtoResource[i][pN[i]]->increaseCPU((*t)->getCpu());
							   m_mapDCtoResource[i][pN[i]]->increaseMem((*t)->getMem());
							}
						}

					}


				}

			}
		}
		ScheduleJobsFromWaitingList(arrivalTime);

                /*Fix this */
  
		//		barrier->Wait();
		arrivalTime+= 30000000;
		if(arrivalTime%900000000>=850000000){
			UpdateResourceData();
		}
	}


	return;
}

void DataCenter::UpdateResourceData(){

	for(int i=0; i<5;i++){
		cout<<"Updating i= "<<i<<"\n";
		m_mapDCtoResource[i]= m_dataCenterProxies[i].GetResourceData();

	}
}

void DataCenter:: Synchronize(){
	m_pThread->join();
}

void DataCenter::Simulation(){
	m_pThread = new std::thread(&DataCenter::StartSimulation, this);
}

void DataCenter::ScheduleJobsFromWaitingList(INT64_ arrival){

	std::unordered_map<int,Node*> availResource= GetResourceData();
	for(auto j=m_vRunningJobs.begin(); j!=m_vRunningJobs.end();j++){

		Job* pJob=*j;
		pJob->INcCurrTime(arrival);
		if(pJob->getRetireTime()<=pJob->getCurrTime()){
			std::vector<Task*> Tasks = pJob->getTasks();
			for(auto t = Tasks.begin(); t != Tasks.end(); t++){
			int Tcpu = (*t)->getCpu();
			int Tmem = (*t)->getMem();
			cout<<"Node Id  "<<"\t"<<(*t)->getNodeID()<<endl;
			availResource[(*t)->getNodeID()]->increaseCPU(Tcpu);
			availResource[(*t)->getNodeID()]->increaseMem(Tmem);
			cout<<"Completed Jobs"<<endl;
			m_vRunningJobs.erase(j);
			}
		}
	}

	std::list<Job*>   waitJobs = GetWaitingJobs();
	while(!waitJobs.empty()){
		Job* pJob=waitJobs.front();
		waitJobs.pop_front();
		std::vector<Task*> Tasks = pJob->getTasks();
		for(auto t = Tasks.begin(); t != Tasks.end(); t++){
		  int Tcpu = (*t)->getCpu();
		  int Tmem = (*t)->getMem();
		  cout<<"Node Id  "<<"\t"<<(*t)->getNodeID()<<endl;
		  availResource[(*t)->getNodeID()]->decreaseCPU(Tcpu);
		  availResource[(*t)->getNodeID()]->decreaseMem(Tmem);
		}
		pJob->setCurrTime(arrival);
		m_vRunningJobs.push_back(pJob);
		cout <<"Scheduling from waiting list"<<endl;
	}

	// Synchronize();
}


void DataCenter::set_dataCenterProxies(DataCenterProxy *proxy){

	m_dataCenterProxies = proxy;
}
