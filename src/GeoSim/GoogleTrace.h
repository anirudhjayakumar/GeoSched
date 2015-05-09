#ifndef GOOGLETRACE_H_
#define GOOGLETRACE_H_

#include "common.h"
#include "Job.h"
#include <unordered_map>
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

#define YEAR 0
#define MONTH 1
#define DAY 2
#define HOUR 3
#define DATA 4


struct TraceItem
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

	TraceItem();
	int 			Init(const std::string &sCSV);
	INT64_ 			WaitTime();
	INT64_ 			RunningTime();
	INT64_ 			ArrivalTime();
	std::string 	GenerateCSV();
	Job* createJob();

	// these are some additional info not part of the trace format but useful
	int 	nCPUMissing;
	int     nMemMissing;

};


struct traceTE{
    string year;
    string month;
    string day;
    string hour;
    string TE;
    traceTE();
    string parse(const std::string &sCSV);
    
    string getTE();
    
    
};








class GoogleTrace
{
private:
	std::vector<TraceItem> vTraceItems;
	std::vector<TraceItem>::iterator currIter;
public:

	int Initialize(const std::string &filePath, string name, int GMT, string path);
	std::vector<TraceItem*> GetNextSet(INT64_ us, string name, int GMT);
	bool  FileEnd();
	int WriteToFile(const std::string &filePath);
    int Logfile(string msg, string path);
    };

class TempElectric{
  private:
     std::map<string,int> vTempElectric;
    std::vector<traceTE> traces;
  public:
    int Initialize(const std::string &filePath, string name, int GMT, string path);
    double TempElectricNextHours(string date, int hour, int start);
    int  Logfile(string msg, string path);
    
    
};




#endif /* GOOGLETRACE_H_ */


