/*
 * JobEvent.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: Anirudh Jayakuamr
 */
#include "JobEvent.h"
#include "common.h"
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "GoogleTrace.h"
#include <unordered_map>
using namespace std;

JobEvent::JobEvent()
{
}

int JobEvent::Initialize(const std::string &filePath)
{
	ifstream fileStream(filePath);
	string sLine;
	vector<string> vTokens;
	nRecords = 0;
	while(!fileStream.eof())
	{
		fileStream >> sLine;
		vTokens.clear();
		Utils::Tokenize(sLine,',',vTokens);
		char *pEnd;
		vTimeStamp.push_back(strtoll(vTokens[TIMESTAMP].c_str(),&pEnd,10));
		vJobID.push_back(strtoll(vTokens[JOBID].c_str(),&pEnd,10));
		vEventType.push_back(strtol(vTokens[EVENT].c_str(),&pEnd,10));
		vUserName.push_back(vTokens[USER]);
		vSchedClass.push_back(strtol(vTokens[SCHEDCLASS].c_str(),&pEnd,10));
		nRecords++;
	}
    cout << "Complete Initialization. Total records read:" << nRecords << endl;
	fileStream.close();
	return SUCCESS;
}

int JobEvent::GenerateCSVWithEventRecordsInSingleRow(const std::string &
		fileWithJobIDs, const std::string &destFilePath)
{
	//load job ids in hash map
	unordered_map<INT64_,Workload::Job> jobIDtoInfo;
	string sLine;
	char *pEnd; //dummy for API completeness
	cout << "Loading JobIDs into hash table:" << endl;
	ifstream srcFileStream(fileWithJobIDs);
	while(!srcFileStream.eof())
	{
		srcFileStream >> sLine;
		INT64_ nJobId = strtoll(sLine.c_str(),&pEnd,10);
		auto it = jobIDtoInfo.find(nJobId);
		if( it != jobIDtoInfo.end() ) {
		    cout << "Duplicate Job ID:" << sLine << endl;
		}
		else {
			jobIDtoInfo.insert(std::make_pair(nJobId,Workload::Job()));
			jobIDtoInfo[nJobId].nJobID = nJobId;
		}
	}
	srcFileStream.close();
	cout << "Finished loading JobIDs. Total size of the hash table is " <<
			jobIDtoInfo.size() << endl;
	// iterate through the job event trace
	cout << "Start iterating through the job event trace. Total size of job events is"
			<< nRecords << endl;
	for(int index = 0; index < nRecords; ++index)
	{
		INT64_ nJobID = vJobID[index];
		auto it = jobIDtoInfo.find(nJobID);
		if(it != jobIDtoInfo.end())
		{
			if(vEventType[index] == 4) //finsih
			{
				jobIDtoInfo[nJobID].nFinishTime = vTimeStamp[index];
				jobIDtoInfo[nJobID].nSchedClass = vSchedClass[index];
				jobIDtoInfo[nJobID].sUser       = vUserName[index];
			}
			else if (vEventType[index] == 1) //schedule
			{
				jobIDtoInfo[nJobID].nScheduledTime = vTimeStamp[index];
			}
			else if (vEventType[index] == 0) // submit
			{
				jobIDtoInfo[nJobID].nArrivalTime = vTimeStamp[index];
			}
		}
	}
	cout << "Done iterating through the job event trace. Now writing trace to file" << endl;
	std::fstream desfileStream(destFilePath, std::ios::out);
	for ( auto iter = jobIDtoInfo.begin(); iter != jobIDtoInfo.end(); ++iter)
	{
		desfileStream << iter->second.GenerateCSV() << endl;
	}
	desfileStream.close();

	return SUCCESS;

}
