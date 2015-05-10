/*
 * GoogleTrace.cpp
 *
 *  Created on: Mar 26, 2015
 *      Author: Anirudh Jayakumar
 */


#include "GoogleTrace.h"
#include "common.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Job.h"

using namespace std;



TraceItem::TraceItem()
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

int TraceItem::Init(const std::string &sCSV)
{
	vector<string> vTokens;
	Tokenize(sCSV,',',vTokens);
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

INT64_ TraceItem::WaitTime()
{
	return (nScheduledTime - nArrivalTime);
}

INT64_ TraceItem::RunningTime()
{
	return (nFinishTime - nScheduledTime);
}

INT64_ TraceItem::ArrivalTime()
{
	return nArrivalTime;
}
Job* TraceItem::createJob(){
    //cout << "=====creating jobs " << nJobID << "  " << nTasks  << "======"<< endl; 
	Job* nJob= new Job(nJobID, RunningTime(), nSchedClass, nTasks,fTotalCPU, fTotalMem );
   // cout << GenerateCSV() << endl;
	return nJob;

}
string  localtime(int i){
    
    string Time;
    time_t m_Time;
    struct tm * ptm;
    
    time ( &m_Time );
    ptm = gmtime(&m_Time);
    if(ptm->tm_hour+i<=0){
        Time=to_string(ptm->tm_hour+i+12);
    }
    else
        Time=to_string((ptm->tm_hour+i)%24);
    Time+=":";
    if((ptm->tm_min)<9) {
        Time+="0" ;
    }
    Time+=to_string((ptm->tm_min));
    Time+=":";
    if((ptm->tm_sec)<9){
        Time+="0" ;
    }
    Time+=to_string((ptm->tm_sec));
    return Time;
}


std::string TraceItem::GenerateCSV()
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


int GoogleTrace::Initialize(const std::string &filePath, string name, int GMT, string path)
{

	ifstream traceStream(filePath);
	string sLine;
	while(!traceStream.eof())
	{
		traceStream >> sLine;
    
		TraceItem job_;
		job_.Init(sLine);
		vTraceItems.push_back(job_);
	}
	currIter = vTraceItems.begin();
    
	traceStream.close();
    string s= name+ " Completed Workload tracing at "+ localtime(GMT);
    Logfile(s,path);
	return SUCCESS;
}





int GoogleTrace:: Logfile(string msg, string path)
{
    
    std::fstream execTraces;
    execTraces.open (path, std::fstream::in | std::fstream::out | std::fstream::app);
    
    execTraces << msg<<endl;
    
    execTraces.close();
    
    
    return SUCCESS;
}


vector<TraceItem *> GoogleTrace::GetNextSet(INT64_ us, string name, int GMT)
{
	vector<TraceItem *> ret;
	if(currIter == vTraceItems.end())
		return ret;
	TraceItem *nextJob;
	nextJob = &(*currIter);
	while (nextJob->nArrivalTime < us)
	{
		ret.push_back(nextJob);
		currIter++;
		if(currIter == vTraceItems.end())
			break;
		nextJob = &(*currIter);
	}
    
	return ret;
}

bool  GoogleTrace::FileEnd()
{
	if(currIter == vTraceItems.end())
		return true;
	else return false;
}

int GoogleTrace::WriteToFile(const std::string &filePath)
{
	ofstream traceStream(filePath);
	for(auto iter = vTraceItems.begin(); iter != vTraceItems.end(); ++iter)
	{
		traceStream << iter->GenerateCSV() << endl;
	}
	traceStream.close();
	return SUCCESS;
}
traceTE::traceTE(){
    
}

 
string traceTE:: parse(const std::string &sCSV){
    vector<string> vTokens;
    Tokenize(sCSV,',',vTokens);
    char *pEnd;
    if(!vTokens[YEAR].empty())
        year= vTokens[YEAR].c_str();
    
    if(!vTokens[MONT].empty())
        month = vTokens[MONT].c_str();
    if(!vTokens[DAY].empty())
        day = vTokens[DAY].c_str();
    if(!vTokens[HOUR].empty())
        hour = vTokens[HOUR].c_str();
    if(!vTokens[DATA].empty())
        TE = vTokens[DATA].c_str();
    string date=year+month+day+hour;
    return date;
       

    
}
string traceTE:: getTE(){
    return TE;
}

int TempElectric::Initialize(const std::string &filePath, string name, int GMT, string path)
{
    string key, data;
    ifstream traceStream(filePath);
    string sLine;
    
    while(!traceStream.eof())
    {
        traceStream >> sLine;
       // cout<<sLine;
        
        traceTE nDate;
        key= nDate.parse(sLine);
        data = nDate.getTE();
       // cout<<"data "<<data<<endl;
        //cout<<"key "<<key<<endl;
        vTempElectric[key]= stoi(data);
    }
 
    
    traceStream.close();
    string s= name+ " Completed Temp/Electric tracing at "+ localtime(GMT);
   Logfile(s,path);
    return SUCCESS;
}
int TempElectric:: Logfile(string msg, string path)
{
    
    std::fstream execTraces;
    execTraces.open (path, std::fstream::in | std::fstream::out | std::fstream::app);
    
    execTraces << msg<<endl;
    
    execTraces.close();
    
    
    return SUCCESS;
}
double TempElectric:: TempElectricNextHours(string date, int hour, int start){
    std::map<string,int>:: iterator currTE;
   
    
    int count = 0;
    int total = 0;
    string tempDate;
    double avgTE;
    
   
    vector<string> vTokens;
    Tokenize(date,',',vTokens);

    
    
    for(int i=start; (i<=23)&&(i<=start+hour);i++)
    {
        tempDate = vTokens[0] + vTokens[1]+ vTokens[2] + to_string(i);
        currTE=vTempElectric.find(tempDate);
        if(currTE!=vTempElectric.end()){
          total+= currTE->second;
          count++;
        }
    }
    if(count!=hour){
      
        string day= vTokens[2];
        int d= atoi(day .c_str()) +1;
        
        if(d<=31){
          std::string temp = std::to_string(d);
          date[(date.size())-1]=temp[0];
          int rem=hour-count;
          for(int i=0;i<rem;i++){
            
             tempDate = vTokens[0] + vTokens[1]+ temp + to_string(i);
             currTE=vTempElectric.find(tempDate);
             if(currTE!=vTempElectric.end()){
                total+= currTE->second;
                count++;
             }
            
          }
        }
        else{
          day="1";
          string mo = vTokens[1];
          int  m = atoi(mo.c_str()) +1;
          std::string temp = std::to_string(m);
          if(m<=12){
            int rem=hour-count;
            for(int i=0;i<rem;i++){
              tempDate = vTokens[0]+temp+day+to_string(i);
              currTE=vTempElectric.find(tempDate);
              if(currTE!=vTempElectric.end()){
                 total+= currTE->second;
                 count++;
               }
                    
             }
          }
        }
    }
    
    avgTE = (double)total/(double)count;
    return avgTE;
   
    
}


