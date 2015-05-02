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
class DataCenterProxy; //forward declare
class Node;
class Job;
class DataCenter {
public:
	DataCenter();
	virtual ~DataCenter();
	int Initialize(std::vector<DataCenterProxy *> dataCenterProxies, const std::string &resourceXML);
	int AddJobsToWaitingList();
	void PrintUtilization();
	std::unordered_map<int,Node*> GetResourceData();
private:
	int 		nDCid;
	std::map<int,DataCenterProxy *> m_dataCenterProxies; // dcid -> proxy
	std::map<int,std::unordered_map<int,Node*> > m_mapDCtoResource;
	std::thread *					m_pThread;
	std::unordered_map<int,Node*> 	m_mapNodes; // nodeid-> node map
	std::list<Job*>  			m_vRunningJobs;
	std::list<Job*>  			m_vWaitingJobs;
	void 						StartSimulation(); //main scheduling logic goes in here
};

#endif /* DATACENTER_H_ */
