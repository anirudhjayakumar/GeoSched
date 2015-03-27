#ifndef GOOGLETRACE_H_
#define GOOGLETRACE_H_

#include "common.h"

#define JOB_ID  		0
#define JOB_ARRIVAL  	1
#define JOB_SCHEDTIME  	2
#define JOB_FINISH  	3
#define JOB_SCHEDCLASS 	4
#define JOB_TASKS	  	5
#define JOB_CPU		  	6
#define JOB_MEM		  	7
#define JOB_USER	  	8
#define JOB_SLA1	  	9
#define JOB_SLA2	  	10
#define JOB_SLA3	  	11

namespace Workload
{


struct Job
{
	// any changes to the variables here must be updated in the
	// trace format xml
	INT64_ 			nJobID;  // job event
	INT64_ 			nArrivalTime; // job event
	INT64_  		nScheduledTime;  // job event
	INT64_ 			nFinishTime;   // job event
	int			    nSchedClass; // job event, may not need it once SLA is decided
	int 			nTasks;
	double			fTotalCPU;
	double 			fTotalMem;
	std::string 	sUser;      // job event
	double 			fSLAParam1; // synthetic
	double 			fSLAParam2; // synthetic
	double 			fSLAParam3; // synthetic

	Job();
	int 			Init(const std::string &sCSV);
	INT64_ 			WaitTime();
	INT64_ 			RunningTime();
	std::string 	GenerateCSV();

	// these are some additional info not part of the trace format but useful
	int 	nCPUMissing;
	int     nMemMissing;

};

class GoogleTrace
{
private:
	std::vector<Job> vJobs;
	std::vector<Job>::iterator currIter;
public:
	int Initialize(const std::string &filePath);
	Job* GetNext();
	int WriteToFile(const std::string &filePath);
};


}

#endif /* GOOGLETRACE_H_ */


