/*
 * GoogleTrace.cpp
 *
 *  Created on: Mar 26, 2015
 *      Author: Anirudh Jayakumar
 */


#include "GoogleTrace.h"
#include <string>
#include <fstream>
using namespace std;



Workload::Job::Job()
{
	nJobID 			= -1;
	nArrivalTime 	= -1;
	nScheduledTime  = -1;
	nFinishTime 	= -1;
	nSchedClass     = -1;
	nTasks 			= 0;
	fTotalCPU 		= 0;
	fTotalMem 		= 0;
	sUser 			= "";
	fSLAParam1 		= 0;
	fSLAParam2 		= 0;
	fSLAParam3 		= 0;
	nCPUMissing     = 0;
	nMemMissing     = 0;
}

int Workload::Job::Init(const std::string &sCSV)
{
	vector<string> vTokens;
	Utils::Tokenize(sCSV,',',vTokens);
	char *pEnd;
	if(!vTokens[JOB_ID].empty())
		nJobID = strtoll(vTokens[JOB_ID].c_str(),&pEnd,10);
	if(!vTokens[JOB_ARRIVAL].empty())
		nArrivalTime = strtoll(vTokens[JOB_ARRIVAL].c_str(),&pEnd,10);
	if(!vTokens[JOB_SCHEDTIME].empty())
		nScheduledTime = strtoll(vTokens[JOB_SCHEDTIME].c_str(),&pEnd,10);
	if(!vTokens[JOB_FINISH].empty())
		nFinishTime = strtoll(vTokens[JOB_FINISH].c_str(),&pEnd,10);
	if(!vTokens[JOB_SCHEDCLASS].empty())
		nSchedClass = strtol(vTokens[JOB_SCHEDCLASS].c_str(),&pEnd,10);
	if(!vTokens[JOB_TASKS].empty())
		nTasks = strtol(vTokens[JOB_TASKS].c_str(),&pEnd,10);
	if(!vTokens[JOB_CPU].empty())
		fTotalCPU = strtod(vTokens[JOB_CPU].c_str(),&pEnd);
	if(!vTokens[JOB_MEM].empty())
		fTotalMem = strtod(vTokens[JOB_MEM].c_str(),&pEnd);
	if(!vTokens[JOB_USER].empty())
		sUser = vTokens[JOB_USER];
	if(!vTokens[JOB_SLA1].empty())
		fSLAParam1 = strtod(vTokens[JOB_SLA1].c_str(),&pEnd);
	if(!vTokens[JOB_SLA2].empty())
		fSLAParam2 = strtod(vTokens[JOB_SLA2].c_str(),&pEnd);
	if(!vTokens[JOB_SLA3].empty())
		fSLAParam3 = strtod(vTokens[JOB_SLA3].c_str(),&pEnd);
	return SUCCESS;

}

INT64_ Workload::Job::WaitTime()
{
	return (nScheduledTime - nArrivalTime);
}

INT64_ Workload::Job::RunningTime()
{
	return (nFinishTime - nScheduledTime);
}

std::string Workload::Job::GenerateCSV()
{
	// the ordering of the elements is critical. Order according to the format xml
	std::string retStr = std::to_string(nJobID) + ",";
	retStr += std::to_string(nArrivalTime) + ",";
	retStr += std::to_string(nScheduledTime) + ",";
	retStr += std::to_string(nFinishTime) + ",";
	retStr += std::to_string(nSchedClass) + ",";
	retStr += std::to_string(nTasks) + ",";
	retStr += std::to_string(fTotalCPU) + ",";
	retStr += std::to_string(fTotalMem) + ",";
	retStr += sUser + ",";
	retStr += std::to_string(fSLAParam1) + ",";
	retStr += std::to_string(fSLAParam2) + ",";
	retStr += std::to_string(fSLAParam3);
	return retStr;

}


int Workload::GoogleTrace::Initialize(const std::string &filePath)
{
	ifstream traceStream(filePath);
	string sLine;
	while(!traceStream.eof())
	{
		traceStream >> sLine;
		Job job_;
		job_.Init(sLine);
		vJobs.push_back(job_);
	}
	currIter = vJobs.begin();
	traceStream.close();
	return SUCCESS;
}

Workload::Job * Workload::GoogleTrace::GetNext()
{
	if(currIter == vJobs.end())
		return NULL;
	Workload::Job *nextJob;
	nextJob = &(*currIter);
	currIter++;
	return nextJob;
}

int Workload::GoogleTrace::WriteToFile(const std::string &filePath)
{
	ofstream traceStream(filePath);
	for(auto iter = vJobs.begin(); iter != vJobs.end(); ++iter)
	{
		traceStream << iter->GenerateCSV() << endl;
	}
	traceStream.close();
	return SUCCESS;
}
