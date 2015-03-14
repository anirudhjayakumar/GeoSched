/*
 * JobEvent.h
 *
 *  Created on: Mar 12, 2015
 *      Author: Anirudh Jayakumar
 */

#ifndef JOBEVENT_H_
#define JOBEVENT_H_

#include <string>
#include <vector>
#include "common.h"

#define TIMESTAMP 	0
#define JOBID     	2
#define EVENT     	3
#define USER      	4
#define SCHEDCLASS  5
class JobEvent
{
private:
	/*
	 * The job events table contains the following fields:
		1. timestamp 2. missinginfo 3. jobID 4. eventtype
		5. username 6. schedulingclass 7. jobname 8. logicaljobname

		We only consider a subset of the above fields
	 */
	std::vector<INT64_> 		vTimeStamp;
	std::vector<INT64_> 		vJobID;
	std::vector<int> 			vEventType;
	std::vector<std::string>	vUserName;
	std::vector<int>       		vSchedClass;
	int 						nRecords;
public:
	JobEvent();
	int Initialize(const std::string &filePath);
	//this method will create a partial trace, the other part of the trace
	//file will be extracted from task events and SLA generators
    int GenerateCSVWithEventRecordsInSingleRow(const std::string &fileWithJobIDs,
    		const std::string &destFilePath);
    /* add methods in future for newer requirements */
private:


};



#endif /* JOBEVENT_H_ */
