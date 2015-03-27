/*
 * TaskEvent.h
 *
 *  Created on: Mar 12, 2015
 *      Author: Anirudh Jayakumar
 */

#ifndef TASKEVENT_H_
#define TASKEVENT_H_
#include <map>
#include <vector>
#include <string>
/*
1. timestamp 2. missinginfo 3. jobID
4. taskindex-withinthejob 5. machineID
6. eventtype  7. username 8. schedulingclass 9. priority
10. resource request for CPU cores 11. resource request for RAM
12. resource request for local disk space 13. differentt-machine constraint
*/

#define JOBID 	2
#define TASKID 	3
#define CPU 	9
#define MEM		10

/* bookkeeping struct */

struct JobEventsInfo
{
	std::vector<int> 	vEventIndices;
	double 				dTotalCPU;
	double 				dTotalMem;
	int 				nCPUMissing;
	int 				nMemMissing;
	JobEventsInfo()
	{
		dTotalCPU = dTotalMem = 0.0;
		nCPUMissing = nMemMissing = 0;
	}
};


/* memory intensive function */
int FillEventInfoIntoCustomTrace(const std::string &sUniqueJobIdFile,const std::string &sEventsPath,
		const std::string &sCustomTraceSource,const std::string &sCustomTraceDest);

#endif /* TASKEVENT_H_ */
