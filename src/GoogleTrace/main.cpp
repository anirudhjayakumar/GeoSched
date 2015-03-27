/*
 * main.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: Anirudh Jayakumar
 */

#include <string>
#include <iostream>
#include "JobEvent.h"
#include "TaskEvent.h"
#include "common.h"
using namespace std;

const string sJobTrace("/Users/anirudhnair/Downloads/googletrace/clusterdata-2011-2/job_events/full.csv");
const string sCompleteJobIDs("/Users/anirudhnair/Downloads/googletrace/clusterdata-2011-2/job_events/complete_jobIds");
const string sCustomFile("/Users/anirudhnair/Downloads/googletrace/clusterdata-2011-2/job_events/custom.csv");
const string sTaskTracePath("/Users/anirudhnair/Downloads/googletrace/clusterdata-2011-2/task_events");

int GenerateJobEventInfo()
{
	JobEvent oJobTrace;
	if( oJobTrace.Initialize(sJobTrace) == FAIL)
	{
		cout << "Jobtrace init fail" << endl;
		return FAIL;
	}

	if( oJobTrace.GenerateCSVWithEventRecordsInSingleRow(sCompleteJobIDs,sCustomFile) == FAIL)
	{
		cout << "GenerateCSVWithEventRecordsInSingleRow failed" << endl;
		return FAIL;
	}
	return SUCCESS;
}

int GenerateTaskEventInfo()
{
	string sDestFile = "/Users/anirudhnair/Downloads/googletrace/clusterdata-2011-2/job_events/custom_dest.csv";
	return FillEventInfoIntoCustomTrace(sCompleteJobIDs,sTaskTracePath,sCustomFile,sDestFile);
}

int main(int argc,char *argv[])
{

	GenerateJobEventInfo();
	return 0;
}
