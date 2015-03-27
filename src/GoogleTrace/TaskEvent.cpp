/*
 * TaskEvent.cpp
 *
 *  Created on: Mar 25, 2015
 *      Author: Anirudh Jayakumar
 */

#include "TaskEvent.h"
#include "common.h"
#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <utility>
#include <algorithm>
#include <unordered_map>
#include <stdlib.h>
#include "GoogleTrace.h"
using namespace std;

int FillEventInfoIntoCustomTrace(const std::string &sUniqueJobIdFile,const std::string &sEventsPath,
		const std::string &sCustomTraceSource,const std::string &sCustomTraceDest)
{
	// load uniquejobIds
	ifstream jobIDstream(sUniqueJobIdFile);
	string sLine;
	char *pEnd; //dummy for API completeness
	set<INT64_> uniqueJobIds;
	while(!jobIDstream.eof())
	{
		jobIDstream >> sLine;
		INT64_ nJobId = strtoll(sLine.c_str(),&pEnd,10);
		uniqueJobIds.insert(nJobId);
	}
	unordered_map<INT64_,JobEventsInfo> jobIDtoInfo;
	// open each file
	for (int fileIndex = 1; fileIndex <= 10; ++fileIndex)
	{
		// go through the traces
		string sTraceName = sEventsPath + "/" + to_string(fileIndex) + ".csv";
		cout << "Opening file" << sTraceName << endl;
		ifstream eventTrace(sTraceName);
		while(!eventTrace.eof())
		{
			eventTrace >> sLine;
			vector<string> vTokens;
			Utils::Tokenize(sLine,',',vTokens);
			INT64_ nJobID = strtoll(vTokens[JOBID].c_str(),&pEnd,10);
			// consider only job ids that we want
			if(uniqueJobIds.find(nJobID) != uniqueJobIds.end() )
			{
				//see if the job already present in hash table
				if( jobIDtoInfo.find(nJobID) == jobIDtoInfo.end())
					jobIDtoInfo.insert(make_pair(nJobID,JobEventsInfo()));
				// see if the event was already considered
				int nTaskIndex = strtol(vTokens[TASKID].c_str(),&pEnd,10);
				if(find(jobIDtoInfo[nJobID].vEventIndices.begin(),jobIDtoInfo[nJobID].vEventIndices.end(),nTaskIndex)
										== jobIDtoInfo[nJobID].vEventIndices.end())
				{
					// task not considered
					cout << "adding task," << nJobID << "," << nTaskIndex << endl;
					jobIDtoInfo[nJobID].vEventIndices.push_back(nTaskIndex);
					// check cpu missing
					if(vTokens[CPU].empty())
					{
						//missing
						jobIDtoInfo[nJobID].nCPUMissing++;
						cout << "Missing,CPU," << nJobID << "," << nTaskIndex << endl;
					} else {
						jobIDtoInfo[nJobID].dTotalCPU+=strtod(vTokens[CPU].c_str(),&pEnd);
					}
					//check mem missing
					if(vTokens[MEM].empty())
					{
						//missing
						jobIDtoInfo[nJobID].nMemMissing++;
						cout << "Missing,MEM," << nJobID << "," << nTaskIndex << endl;
					} else {
						jobIDtoInfo[nJobID].dTotalMem+=strtod(vTokens[MEM].c_str(),&pEnd);
					}
				}
			}

		}
		eventTrace.close();

	} //end going through all files

	// time to fill the trace class and write the complete trace to file
	Workload::GoogleTrace oTrace;
	oTrace.Initialize(sCustomTraceSource);
	// iterate through the trace
	Workload::Job *pJob = oTrace.GetNext();
	while(pJob)
	{
		// find pJob in hash table. if present update values else log details
		if( jobIDtoInfo.find(pJob->nJobID) != jobIDtoInfo.end())
		{
			pJob->nTasks		= jobIDtoInfo[pJob->nJobID].vEventIndices.size();
			pJob->fTotalCPU		= jobIDtoInfo[pJob->nJobID].dTotalCPU;
			pJob->fTotalMem		= jobIDtoInfo[pJob->nJobID].dTotalMem;
			pJob->nCPUMissing	= jobIDtoInfo[pJob->nJobID].nCPUMissing;
			pJob->nMemMissing	= jobIDtoInfo[pJob->nJobID].nMemMissing;
		}
		else {
			cout << pJob->nJobID << " not found in hash table. This entry will remain partially filled" << endl;
		}
		pJob = oTrace.GetNext();
	}

	oTrace.WriteToFile(sCustomTraceDest);
	return SUCCESS;
}
