/*
 * DataCenter.h
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#ifndef DATACENTER_H_
#define DATACENTER_H_

#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <map>
#include <thread>
#include "GoogleTrace.h"
#include "common.h"
class DataCenterProxy; //forward declare
class Node;

class Job;
class DataCenter {
public:
	DataCenter(int id, const std::string &filePath);
	virtual ~DataCenter();
	int Initialize(std::vector<DataCenterProxy *> dataCenterProxies, const std::string &resourceXML);
	void AddJobsToWaitingList(Job *pJob);
	void ScheduleJobsFromWaitingList(INT64_ arrival);
	void PrintUtilization();
	std::unordered_map<int,Node*>& GetResourceData();
	void UpdateResourceData();
	void set_dataCenterProxies(DataCenterProxy *proxy);
	GoogleTrace* getWorkLoad();
	void Synchronize();
	void Simulation();
	std::list<Job*>&  GetWaitingJobs();
private:
	int 		nDCid;
	DataCenterProxy* m_dataCenterProxies; // dcid -> proxy
	std::map<int,std::unordered_map<int,Node*> > m_mapDCtoResource;
	std::thread *					m_pThread;
	std::unordered_map<int,Node*> 	m_mapNodes; // nodeid-> node map
	//Barrier *barrier;
	mutex                       m_waitMutex;
	mutex                       m_resourceMutex;
	std::list<Job*>  			m_vRunningJobs;
	std::list<Job*>  			m_vWaitingJobs;
	GoogleTrace* workLoad;
	void 						StartSimulation(); //main scheduling logic goes in here
};

#endif /* DATACENTER_H_ */
