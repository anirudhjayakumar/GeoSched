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

#define CORE_IDLE  				245
#define CORE_PEAK  				320
#define AVG_UTIL    			0.4
#define FREE_AIR_CUT_OFF		64
#define COP 					2 //for 15 degree celcius
#define MONTH					1

using namespace std;

#define TIMEINC 				300000000   // 5 mins in us
#define RESOURCE_SYNC_TIME		1800000000 // 1800 secs in us
#define ENERGY_CALC_INTERVAL	900000000 // 15 mins

#define MAX_TIME				86400000000 //24 hours


const string machineConfig("../../datacenters/machine_config.csv");

DataCenter::DataCenter(int id, const std::string &workloadPath, Barrier *pBarrier, string n, int gmtDiff, const string &tracepath, \
		const string &tempPath, const string &elecPath, bool isAirEco) {
	// TODO Auto-generated constructor stub
	nDCid = id;
	m_sWorkloadTrace = workloadPath;
	m_TempTrace = tempPath;
	m_ElecTrace = elecPath;
	m_pBarrier = pBarrier;
	name = n;
	m_GMT = gmtDiff;
	m_ExecutionTraces = tracepath;
	bAirEco = isAirEco;
	m_TotalCost = 0;
	m_TotalEnergy = 0;
	InitStartPoint();
	string s= "Started creating " + name + " DataCenter at " +getLocalTime();

	Logfile(s);
}

DataCenter::~DataCenter() {
	// TODO Auto-generated destructor stub
	execTraces.close();
}

double 	DataCenter::isAirEco()
{
	return bAirEco;
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
	string s= "Completed creating "+ name +" DataCenter at "+ getLocalTime();

	Logfile(s);


	m_workLoad.Initialize(m_sWorkloadTrace, name, m_GMT, m_ExecutionTraces);
	m_Temp.Initialize(m_TempTrace, name, m_GMT, m_ExecutionTraces);
	m_Electric.Initialize(m_ElecTrace, name, m_GMT, m_ExecutionTraces);
	execTraces.open (m_ExecutionTraces, std::fstream::in | std::fstream::out | std::fstream::app);

	return SUCCESS;
}

void DataCenter::AddJobsToWaitingList(Job *pJob) {

	m_waitMutex.lock();
	m_vWaitingJobs.push_back(pJob);
	m_waitMutex.unlock();
	//cout << "Added to wait list\n";

}
double DataCenter::GetUtilization() {
	NodeMap &nodeMap = m_mapDCtoResource[nDCid];
	INT64_ totalCpu(0);
	INT64_ freeCpu(0);
	INT64_ useCpu(0);
	INT64_ totalMem(0);
	INT64_ freeMem(0);
	INT64_ useMem(0);
	double pCpu;
	double pMem;
	string s;

	m_resourceMutex.lock();
	for (auto node_iter = nodeMap.begin(); node_iter != nodeMap.end();
			++node_iter) {


		freeCpu+= node_iter->second->getFreeCPU();

		freeMem+= node_iter->second->getFreeMem();

		totalCpu+= node_iter->second->getTotalCPU();

		totalMem+= node_iter->second->getTotalMem();

	}
	m_resourceMutex.unlock();

	useCpu = totalCpu - freeCpu;

	useMem = totalMem - freeMem;


	pCpu=((double)useCpu / (double)totalCpu)*100;



	return pCpu;
}




































void DataCenter::GetUtilization(double &cpu_util,int &free_cpu, int &total_cpu) {
	NodeMap &nodeMap = m_mapDCtoResource[nDCid];
	INT64_ totalCpu(0);
	INT64_ freeCpu(0);
	INT64_ useCpu(0);
	INT64_ totalMem(0);
	INT64_ freeMem(0);
	INT64_ useMem(0);
	double pCpu;
	double pMem;
	string s;

	m_resourceMutex.lock();
	for (auto node_iter = nodeMap.begin(); node_iter != nodeMap.end();
			++node_iter) {


		freeCpu+= node_iter->second->getFreeCPU();
#ifdef DEBUG
		s="free CPU "+ to_string(freeCpu)+ " Node id "+ to_string(node_iter->second->getNodeID());
		Logfile(s);
#endif
		freeMem+= node_iter->second->getFreeMem();
#ifdef DEBUG
		s="free Mem "+ to_string(freeMem)+ " Node id "+ to_string(node_iter->second->getNodeID());
		Logfile(s);
#endif
		totalCpu+= node_iter->second->getTotalCPU();
#ifdef DEBUG
		s="free Total CPU "+ to_string(totalCpu)+ " Node id "+ to_string(node_iter->second->getNodeID());
		Logfile(s);
#endif
		totalMem+= node_iter->second->getTotalMem();
#ifdef DEBUG
		s="free Total Mem "+ to_string(totalMem)+ " Node id "+ to_string(node_iter->second->getNodeID());
		Logfile(s);
#endif
	}
	m_resourceMutex.unlock();

	useCpu = totalCpu - freeCpu;

	useMem = totalMem - freeMem;
#ifdef DEBUG
	s=" while utilization used " + to_string(useCpu)+ "  free CPU "+ to_string(freeCpu)  + " total cpu "+to_string(totalCpu) + " at  time: " +getLocalTime();
	Logfile(s);
#endif

	pCpu=((double)useCpu / (double)totalCpu)*100;
	pMem=((double)useMem / (double)totalMem)*100;
	cpu_util = pCpu;
	free_cpu = freeCpu;
	total_cpu = totalCpu;
	s = name + "Time: " + to_string(arrivalTime) + " CPU Utilization: "+ to_string(pCpu) +"% Memory Utilization: "+ to_string(pMem)+"%";
	Logfile(s);


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
	string s = name + " Getting resource data at " +getLocalTime()+"\n";
	Logfile(s);
	m_resourceMutex.lock();
	NodeMap nodeMap = m_mapDCtoResource[nDCid];
	m_resourceMutex.unlock();
	return nodeMap;
}

GoogleTrace* DataCenter::getWorkLoad() {
	return &m_workLoad;
}

int DataCenter:: Logfile(string msg)
{
	m_printMutex.lock();
	execTraces << msg<<endl;
	m_printMutex.unlock();
	return SUCCESS;

}

int DataCenter:: Logfile(double value,string n)
{
	char buffer[200] ;

	sprintf(buffer, "%s \t %.30f",n.c_str(),value);

	m_printMutex.lock();

	execTraces <<buffer<< endl;
	m_printMutex.unlock();
	return SUCCESS;

}

double DataCenter:: TemperatureNextHours(int hour)
{
	chrono::system_clock::time_point tp = startPoint + std::chrono::microseconds(arrivalTime);
	// Convert std::chrono::system_clock::time_point to std::time_t
	std::time_t tt = std::chrono::system_clock::to_time_t(tp);
	// Convert std::time_t to std::tm (popular extension)
	std::tm tm = std::tm{0};
	gmtime_r(&tt, &tm);
	string date = to_string(tm.tm_year+1900) + "," + to_string(tm.tm_mon + 1) + "," + to_string(tm.tm_mday);

	double temp = m_Temp.TempElectricNextHours(date, hour, tm.tm_hour);
#ifdef DEBUG
	string log = name + " TemperatureNextHours: " + date + " hour:" + to_string(tm.tm_hour) + " count:" + to_string(hour)
		+ " temperature: " + to_string(temp);
	Logfile(log);
#endif
	return temp;

}

double DataCenter:: ElectricityNextHours( int hour){

	chrono::system_clock::time_point tp = startPoint + std::chrono::microseconds(arrivalTime);
	// Convert std::chrono::system_clock::time_point to std::time_t
	std::time_t tt = std::chrono::system_clock::to_time_t(tp);
	// Convert std::time_t to std::tm (popular extension)
	std::tm tm = std::tm{0};
	gmtime_r(&tt, &tm);
	string date = to_string(tm.tm_year+1900) + "," + to_string(tm.tm_mon + 1) + "," + to_string(tm.tm_mday);
	double elec = m_Electric.TempElectricNextHours(date, hour, tm.tm_hour);
	//string log = name + " ElectricityNextHours: " + date + " hour:" + to_string(tm.tm_hour) + " count:" + to_string(hour)
	//		+ " Electricty: " + to_string(elec);
	//Logfile(log);
	return elec;
}




void DataCenter::StartSimulation() {

	string s =name + " Entered Simulation at "+getLocalTime();
	Logfile(s);
	arrivalTime = TIMEINC;
	int cycle_count = 1;
	// loop till the time there are jobs left in the trace
	// or till the time all jobs complete running.

	while (!m_workLoad.FileEnd() || m_vRunningJobs.size()) {
		/* Take load for this interval */
		s= "Running Queue for "+ name+ " size: "+to_string(m_vRunningJobs.size()) +" time: "+ getLocalTime()+"\n";
		Logfile(s);

		vector<TraceItem*> currLoad = m_workLoad.GetNextSet(arrivalTime, name, m_GMT);
		if (currLoad.size()) {
			s= "Job Arrival at "+ name+ " size: "+to_string(currLoad.size()) +" time: "+ getLocalTime()+"\n";
			Logfile(s);
			for (auto it = currLoad.begin(); it != currLoad.end(); it++) {
				//
				Job* pJob = (*it)->createJob();

				/* high sensitive jobs put in my own queue */
				if (pJob->sClass() == 2 || pJob->sClass() == 3) {
					s="High sensitive jobs at " + name + " time: "+ getLocalTime();
					Logfile(s);

					AddJobsToWaitingList(pJob); // schedule during schedule phase
				} else {
					s= name + " Checking resource available at time: "+getLocalTime();
					Logfile(s);
					auto vecDCs = GetDCSchedulable(pJob);

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
			//PrintUtilization();
		}

		if(arrivalTime % ENERGY_CALC_INTERVAL == 0)
		{
			UpdateEnergyCost();
		}

		s =name + " Waiting at "+ getLocalTime();
		Logfile(s);

		m_pBarrier->Wait(cycle_count);
		s =name + " Resuming at "+ getLocalTime();
		Logfile(s);

		// barrier: all threads stop here before proceeding

		if(nDCid == 0) cout<<"Cycle Count "<<cycle_count<<endl;
		cycle_count++;
	}

	decrementBarrier();
	s =name + " Leaving simulation at"+ getLocalTime() + " TotalCost:" + to_string(m_TotalCost) + \
	   " Total Energy:" + to_string(m_TotalEnergy);
	Logfile(s);
	return;
}

void DataCenter::  decrementBarrier(){
	m_pBarrier->decrement();
}


void DataCenter::MetaSchedJobToDC(std::vector<int> vecDCs, Job* pJob)
{
	string s;
	if(vecDCs.size() == 0)
	{
		//skip the job
		delete pJob;
	}
	else if (vecDCs.size() == 1)
	{
		m_dataCenterProxies[vecDCs[0]].SubmitJob(pJob);
	}
	else
	{

		double total_cost;
		double cpu_util=0;
		int    freeCPUs,totalCPUs;

		// will be replaced with advanced algo. for now choose random dc id and send job
		int dc_id = vecDCs[0];

#ifdef GEO
		total_cost = CalculateDynamicJobCost(&m_dataCenterProxies[vecDCs[0]],pJob, m_dataCenterProxies[0].GetName()) + \
			     CalculateCoolingCost(&m_dataCenterProxies[vecDCs[0]],pJob,m_dataCenterProxies[0].GetName());
#elif LOAD

		total_cost = CoolLoadBalancer(&m_dataCenterProxies[vecDCs[0]],pJob, m_dataCenterProxies[0].GetName());


#endif
		s = name + " recieved total cost from " + m_dataCenterProxies[0].GetName();
		Logfile(total_cost, s );

		for (int index = 1; index < vecDCs.size(); ++index)
		{
			int dc = vecDCs[index];
			double dc_cost;

#ifdef GEO
			dc_cost = CalculateDynamicJobCost(&m_dataCenterProxies[dc],pJob,m_dataCenterProxies[dc].GetName()) + \
				  CalculateCoolingCost(&m_dataCenterProxies[dc],pJob,m_dataCenterProxies[dc].GetName());


#elif LOAD
			dc_cost = CoolLoadBalancer(&m_dataCenterProxies[dc],pJob,m_dataCenterProxies[dc].GetName());
#endif



			s = name + " recieved total cost from " + m_dataCenterProxies[dc].GetName();
			Logfile(dc_cost, s );


			if( dc_cost < total_cost)
			{
				//    cout<<"changed"<< dc_cost<<endl;
				total_cost = dc_cost;
				dc_id = dc;
			}
		}
		s= name+ " Scheduling jobs on "+ m_dataCenterProxies[dc_id].GetName()+ " time: " +getLocalTime();
		Logfile(s);

		m_dataCenterProxies[dc_id].SubmitJob(pJob);


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


string DataCenter:: GetName()
{
	return name;
}

void DataCenter::ProgressRunningJobs() {
	NodeMap &availResource = m_mapDCtoResource[nDCid];
	string s;
	if(arrivalTime > MAX_TIME)
	{
		string log = "time up. removing jobs from running list.";
		Logfile(log);
		// delete all running jobs
		for(auto iter = m_vRunningJobs.begin(); iter != m_vRunningJobs.end(); ++iter)
		{
			Job *pJob = *iter;
			delete pJob;
		}
		m_vRunningJobs.clear();
	}
	list<Job*>::iterator j = m_vRunningJobs.begin();
	while (j != m_vRunningJobs.end())
	{

		Job* pJob = *j;
		pJob->IncCurrTime(TIMEINC);
		if (pJob->GetTotalRunTime() <= pJob->getCurrTime()) {
			s="Job Completed at "+ name+ " time: "+getLocalTime();
			Logfile(s);

			std::vector<Task*> Tasks = pJob->getTasks();
			// iterate through each task and reclaim the node resource
			// also remove task from node's task list

			for (auto t = Tasks.begin(); t != Tasks.end(); t++) {
				//reclaiming node resource
				int Tcpu = (*t)->getCpu();
				int Tmem = (*t)->getMem();
				int nodeId = (*t)->getNodeID();
				m_resourceMutex.lock();
#ifdef DEBUG
				s=" Before increasing " +to_string(Tcpu)+ " CPU available at "+ to_string(nodeId) + "\t"+ to_string(availResource[nodeId]->getFreeCPU() ) + " at  time: " +getLocalTime();
				Logfile(s);
				s="Before increasing " + to_string(Tmem)+ "MEM available at " +to_string(nodeId)+ "\t"+  to_string(availResource[nodeId]->getFreeMem()) + " at  time: " +getLocalTime();
				Logfile(s);
#endif
				availResource[nodeId]->increaseCPU(Tcpu);
				availResource[nodeId]->increaseMem(Tmem);
#ifdef DEBUG
				s=" After increasing " +to_string(Tcpu)+ " CPU available at "+ to_string(nodeId) + "\t"+ to_string(availResource[nodeId]->getFreeCPU())+  " at  time: " +getLocalTime();
				Logfile(s);
				s="After increasing " + to_string(Tmem)+ "MEM available at " +to_string(nodeId)+ "\t"+  to_string(availResource[nodeId]->getFreeMem())+ " at  time: " +getLocalTime();
				Logfile(s);

#endif                              				//removing task from node task list
				availResource[nodeId]->vTasks.erase(remove(availResource[nodeId]->vTasks.begin(), availResource[nodeId]->vTasks.end(), *t), availResource[nodeId]->vTasks.end());


				m_resourceMutex.unlock();

			}
			delete pJob;
			j = m_vRunningJobs.erase(j);
			s = name + " Freed the resource and deleted tasks "+ getLocalTime();
			Logfile(s);
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
	string s = name + " Completed Updating Resource at " + getLocalTime();
	Logfile(s);
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
	string s;

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
					int tn  = (*task_iter)->getNodeID();
#ifdef DEBUG
					s=" while scheduling Before decreasing  " + to_string(CPU)+ " CPU available at "+ to_string(tn) + "\t" + to_string(node_iter->second->getFreeCPU()) + " at  time: " +getLocalTime();
					Logfile(s);

					s="while scheduling Before decreasing  " + to_string(MEM)+ "MEM available at " +to_string(tn)+ "\t"+  to_string(node_iter->second->getFreeMem()) + " at  time: " + getLocalTime();
					Logfile(s);
#endif

					m_resourceMutex.lock();

					node_iter->second->decreaseCPU(CPU);
					node_iter->second->decreaseMem(MEM);
#ifdef DEBUG
					s=" while scheduling after decreasing  " +to_string(CPU)+ " CPU available at "+ to_string(tn) + "\t"+ to_string(node_iter->second->getFreeCPU())+  " at  time: " +getLocalTime();
					Logfile(s);
					s="while scheduling after decreasing  " + to_string(MEM)+ "MEM available at " +to_string(tn)+ "\t"+  to_string(node_iter->second->getFreeMem()) + " at  time: " +getLocalTime();
					Logfile(s);
#endif
					node_iter->second->vTasks.push_back(*task_iter);
					m_resourceMutex.unlock();
					fit.push_back(*task_iter);
				}
			}
		}

		if (fit.size() == vTasks.size()) {
			pJob->setNodeID(node_iter->second->getNodeID());

			break; // no more nodes needed to be checked
		}
	}

	if (fit.size() == vTasks.size()) {


		m_vRunningJobs.push_back(pJob);
		s= name + " Running Job " + to_string(pJob->getJobID())+ " at Node Id " + to_string(pJob->getNodeID())+ " time: " + getLocalTime();
		Logfile(s);
		RemoveJobFromWaitingQueue(pJob);

	}
	else {
		// remove assigned tasks from nodes and reclaim resources
		for (auto t = fit.begin(); t != fit.end(); t++) {
			//reclaiming node resource
			int Tcpu = (*t)->getCpu();
			int Tmem = (*t)->getMem();
			int nodeId = (*t)->getNodeID();
#ifdef DEBUG
			s=" while could n't schedule Before increasing  " +to_string(Tcpu)+ " CPU available at "+ to_string(nodeId) + "\t"+ to_string(nodeMap[nodeId]->getFreeCPU()) + " at  time: " +getLocalTime();
			Logfile(s);
			s="while couldn't schedule Before increasing " + to_string(Tmem)+ "MEM available at " +to_string(nodeId)+ "\t"+  to_string(nodeMap[nodeId]->getFreeMem()) + " at  time: " +getLocalTime();
			Logfile(s);
#endif
			m_resourceMutex.lock();

			nodeMap[nodeId]->increaseCPU(Tcpu);
			nodeMap[nodeId]->increaseMem(Tmem);
#ifdef DEBUG
			s=" while could n't schedule After increasing  " +to_string(Tcpu)+ " CPU available at "+ to_string(nodeId) + "\t"+ to_string(nodeMap[nodeId]->getFreeCPU() ) + " at  time: " +getLocalTime();
			Logfile(s);
			s="while couldn't schedule After increasing " + to_string(Tmem)+ "MEM available at " +to_string(nodeId)+ "\t"+ to_string( nodeMap[nodeId]->getFreeMem())+  " at  time: " +getLocalTime();
			Logfile(s);
#endif
			//removing task from node task list
			nodeMap[nodeId]->vTasks.erase(remove(nodeMap[nodeId]->vTasks.begin(), nodeMap[nodeId]->vTasks.end(), *t), \
					nodeMap[nodeId]->vTasks.end());

			m_resourceMutex.unlock();

		}
		s= name + "Unable to schedule job" + to_string(pJob->getJobID())+" time: " + getLocalTime();
		Logfile(s);


	}
	return SUCCESS;
}


double DataCenter::CalculateDynamicJobCost(DataCenterProxy* proxy,Job *pJob, string n)
{
	/*
	 * Get electricity price
	 * calculate dynamic power
	 * calculate cost
	 */
	double elec_cost = proxy->ElectricityNextHours(4);
	int dynamic_power_max = CORE_PEAK - CORE_IDLE;
	int power_per_core = AVG_UTIL*dynamic_power_max;
	// we will max the CPU's to 64, else the energy numbers will be incomprehensible
	double cores = pJob->nCores * (double) (64.0/1600.0);
	double secs = ((double)pJob->GetTotalRunTime()/1000000);
	double hours = secs/3600;
	double total_energy_for_job = hours*cores*power_per_core;
	double total_cost = (total_energy_for_job*elec_cost)/1000000;
	return total_cost;

}

double DataCenter::CalculateCoolingCost(DataCenterProxy* proxy,Job *pJob, string n)
{
	/*
	 * see which equation to use using aireco info
	 * calculate cooling contribution due to dynamic power
	 */
	double elec_cost = proxy->ElectricityNextHours(4);
	int dynamic_power_max = CORE_PEAK - CORE_IDLE;
	int power_per_core = AVG_UTIL*dynamic_power_max;
	// we will max the CPU's to 64, else the energy numbers will be incomprehensible
	double cores = pJob->nCores * (double)(64.0/1600.0);
	double total_power_for_job = cores*power_per_core;
	double temp_avg = proxy->TemperatureNextHours(4);
	double cooling_power = 0;
	if(proxy->isAirEco() && temp_avg <= (double)FREE_AIR_CUT_OFF)
	{
		float PUE =0;
		if(temp_avg <= 25)
			PUE = 1.05;
		else if(temp_avg > 25 && temp_avg <= 35)
			PUE = 1.07;
		else if(temp_avg > 35 && temp_avg <= 50)
			PUE = 1.09;
		else if(temp_avg > 50 && temp_avg <= 60)
			PUE = 1.10;
		else
			PUE = 1.17;
		cooling_power = total_power_for_job*(PUE - 1);
	}
	else
	{
		cooling_power = total_power_for_job/COP;
	}
	double secs = ((double)pJob->GetTotalRunTime()/1000000);
	double hours = secs/3600;
	double total_energy_for_cooling = hours*cooling_power;
	double total_cost = (total_energy_for_cooling*elec_cost)/1000000;
	return total_cost;

}

double DataCenter::CoolLoadBalancer(DataCenterProxy* proxy,Job *pJob, string n)
{
	double cool = CalculateCoolingCost(proxy, pJob, n);
	double dynamic = CalculateDynamicJobCost(proxy, pJob, n);
	double raw_cost = dynamic + cool;
	// get util
	double cpu_util=0;
	int    freeCPUs,totalCPUs;
	cpu_util= proxy->GetUtilization();
	double total_cool = 0.999 * raw_cost + 0.001*cpu_util;
	return total_cool;
}



















void DataCenter::UpdateEnergyCost()
{
	// get util
	double cpu_util=0;
	int    freeCPUs,totalCPUs;
	GetUtilization(cpu_util,freeCPUs,totalCPUs);

	double it_power = (freeCPUs*CORE_IDLE + (totalCPUs-freeCPUs)*CORE_PEAK*AVG_UTIL)* (double)(64.0/1600.0);
	double cooling_power = 0;
	string s;
	//cooling
	double temp_avg = TemperatureNextHours(1);
	if(isAirEco() && temp_avg <= (double)FREE_AIR_CUT_OFF)
	{
		float PUE =0;
		if(temp_avg <= 25)
			PUE = 1.05;
		else if(temp_avg > 25 && temp_avg <= 35)
			PUE = 1.07;
		else if(temp_avg > 35 && temp_avg <= 50)
			PUE = 1.09;
		else if(temp_avg > 50 && temp_avg <= 60)
			PUE = 1.10;
		else
			PUE = 1.17;
		cooling_power = it_power*(PUE - 1);
	}
	else
	{
		cooling_power = it_power/COP;
	}
	int elec_cost = ElectricityNextHours(1);

	s = "elec_cost ";
	Logfile(elec_cost, s );
	s = "cooling_power ";
	Logfile(cooling_power, s );
	s = "it_power ";
	Logfile(it_power, s );





	double hours = ((double)ENERGY_CALC_INTERVAL/1000000)/3600;
	//#ifdef DEBUG
	s = "hours ";
	Logfile(hours, s );
	//#endif

	double total_energy = hours*(it_power + cooling_power);
	//  #ifdef DEBUG
	s =  "Total Energy: " ;
	Logfile(total_energy, s );
	//#endif
	double total_cost = (total_energy*elec_cost)/1000000;
	//    #ifdef DEBUG
	s =  "Total cost: " ;
	Logfile(total_cost, s );
	//#endif

	m_TotalCost+=total_cost;
	//    #ifdef DEBUG
	s =  "Data Total Cost: " ;
	Logfile(m_TotalCost, s );
	//   #endif
	m_TotalEnergy+=total_energy;
	//   #ifdef DEBUG
	s =  "Data Total Energy: " ;
	Logfile(m_TotalEnergy, s );
	//   #endif

	/*
	   s =  "Energy: " ;
	   Logfile(total_energy, s );
	   s =  "Total Energy: " ;
	   Logfile(m_TotalEnergy, s );
	   s =  "Cost: " ;
	   Logfile(total_cost, s );
	   s =  "Total Cost: " ;
	   Logfile(m_TotalCost, s );*/


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
	if(arrivalTime > MAX_TIME)
	{
		//delete all jobs
		string log = "time up. remvoing jobs from waiting list.";
		Logfile(log);
		m_waitMutex.lock();
		for (auto iter = m_vWaitingJobs.begin(); iter != m_vWaitingJobs.end(); ++iter) {
			Job* pJob = *iter;
			delete pJob;
		}
		m_vWaitingJobs.clear();
		m_waitMutex.unlock();
	}
	else
	{
		std::list<Job*> waitJobs = GetWaitingJobs();
		for (auto iter = waitJobs.begin(); iter != waitJobs.end(); ++iter) {
			Job* pJob = *iter;
			if (CheckDCFit(pJob, availResource))
			{
				ScheduleJob(pJob);
			}
		}
	}
}

void DataCenter::set_dataCenterProxies(DataCenterProxy *proxy)
{
	m_dataCenterProxies = proxy;
}

string DataCenter:: localtime(int i)
{
	return to_string(arrivalTime); // for logs just arrival time should be good

}


void 	DataCenter::InitStartPoint()
{
	// we always consider 15 of each month
	std::tm tm = {0};
	tm.tm_sec = 00;
	tm.tm_min = 00;
	tm.tm_mon = MONTH - 1;
	tm.tm_year = 113;
	tm.tm_isdst = -1;
	switch(nDCid)
	{
		case CHILE:
			tm.tm_mday = 14;
			tm.tm_hour = 21;
			break;
		case FINLAND:
			tm.tm_mday = 15;
			tm.tm_hour = 3;
			break;
		case SINGAPORE:
			tm.tm_mday = 15;
			tm.tm_hour = 8;
			break;
		case OREGON:
			tm.tm_mday = 14;
			tm.tm_hour = 17;
			break;
		case IOWA:
			tm.tm_mday = 14;
			tm.tm_hour = 19;
			break;
	}
	time_t tt = timegm(&tm);
	// Convert std::time_t to std::chrono::system_clock::time_point
	startPoint = chrono::system_clock::from_time_t(tt);
}
