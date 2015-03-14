#ifndef GOOGLETRACE_H_
#define GOOGLETRACE_H_

namespace Workload
{


struct Job
{
	INT64_ 			nJobID;  // job event
	INT64_ 			nArrivalTime; // job event
	INT64_  		nScheduledTime;  // job event
	INT64_ 			nFinishTime;   // job event
	int			    nSchedClass; // job event, may not need it once SLA is decided
	int 			nTasks;
	float			fTotalCPU;
	float 			fTotalMem;
	std::string 	sUser;      // job event
	float 			fSLAParam1; // synthetic
	float 			fSLAParam2; // synthetic
	float 			fSLAParam3; // synthetic

	Job()
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
	}

	INT64_ WaitTime()
	{
		return (nScheduledTime - nArrivalTime);
	}

	INT64_ RunningTime()
	{
		return (nFinishTime - nScheduledTime);
	}

	std::string GenerateCSV()
	{
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

};

class GoogleTrace
{
private:
	std::vector<Job> vJobs;
public:
	int Initialize(const std::string &filePath);
	int GetNext(Job &nextJob);
};


}

#endif /* GOOGLETRACE_H_ */


