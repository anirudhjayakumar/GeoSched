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
#include <mutex>
#include "GoogleTrace.h"
#include "common.h"
#include <chrono>
#include <ctime>
#include <fstream>

class DataCenterProxy; //forward declare
class Node;
class ConfigAccessor;

#define DC_COUNT    5
#define CHILE		0
#define FINLAND		1
#define SINGAPORE	2
#define OREGON		3
#define IOWA		4

typedef std::map<int,std::unordered_map<int,Node*> > DCResource;
typedef std::unordered_map<int,Node*>                NodeMap;
class Job;
class DataCenter {
public:
	DataCenter(int id, const std::string &workloadPath, Barrier *pBarrier, string name, int gmtDiff, const  string &traces\
			,const string &tempPath, const string &elecPath, bool);
	virtual ~DataCenter();
	int 			Initialize(DataCenterProxy * dataCenterProxies, ConfigAccessor *pAccessor);
	void 			AddJobsToWaitingList(Job *pJob);
	void 			ScheduleJobsFromWaitingList();
	void 			GetUtilization(double &util, int &free, int &total_cpu);
	NodeMap 		GetResourceData();
	void 			UpdateResourceData();
	void 			set_dataCenterProxies(DataCenterProxy *proxy);
	GoogleTrace* 	getWorkLoad();
	void 			Join();
	void 			Simulation();
    string          getLocalTime();
    string          GetName();
    Barrier*        getBarrier();
    void            decrementBarrier();
    double          TemperatureNextHours(int hour);
    double          ElectricityNextHours(int hour);
    double 			isAirEco();

private:
	int 				nDCid;
	DataCenterProxy* 	m_dataCenterProxies; // dcid -> proxy
	DCResource			m_mapDCtoResource;
	std::thread *		m_pThread;
	NodeMap				m_mapNodes; // nodeid-> node map
	std::mutex          m_waitMutex;
	std::mutex          m_resourceMutex;
	std::list<Job*>  	m_vRunningJobs;
	std::list<Job*>  	m_vWaitingJobs;
	GoogleTrace 		m_workLoad;
    TempElectric        m_Temp;
    TempElectric        m_Electric;
    std::string			m_sWorkloadTrace;
    std::string			m_TempTrace;
    std::string			m_ElecTrace;
	Barrier				*m_pBarrier;
	ConfigAccessor      *m_pAccessor;
    int                 m_GMT;
    bool				bAirEco;
    double 				m_TotalCost;
    double				m_TotalEnergy;
    std::chrono::system_clock::time_point startPoint;
    INT64_ 				arrivalTime;
    std::fstream 		execTraces;
private:
    bool				CheckDCFit(Job *,NodeMap &);
	void 				StartSimulation(); //main scheduling logic goes in here
	std::vector<int>    GetDCSchedulable(Job *);
	void				MetaSchedJobToDC(std::vector<int> vecDCs,Job*);
	void 				ProgressRunningJobs();
	int 				ScheduleJob(Job *);
	void 				RemoveJobFromWaitingQueue(Job*);
	std::list<Job*> 	GetWaitingJobs();
    string              localtime(int gmtDiff);
    string              name;
    string              m_ExecutionTraces;
    int 				Logfile(string msg);
    // speculated cost calculation
    double 				CalculateDynamicJobCost(DataCenterProxy* proxy,Job *pJob);
    double 				CalculateCoolingCost(DataCenterProxy* proxy,Job *pJob);
    //
    void 				UpdateEnergyCost();
    void 				InitStartPoint();

    
   
   
};

#endif /* DATACENTER_H_ */
