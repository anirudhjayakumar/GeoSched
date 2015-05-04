/*
 * DataCenterProxy.cpp
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#include "DataCenterProxy.h"
#include "common.h"
#include "DataCenter.h"
#include "Job.h"
#include <iostream>
using namespace std;
DataCenterProxy::DataCenterProxy() {
	// TODO Auto-generated constructor stub

}

DataCenterProxy::~DataCenterProxy() {
	// TODO Auto-generated destructor stub
}

int DataCenterProxy:: Initialize(DataCenter *dc)
{
	//cout<<"Creating Proxy\n";
	 m_dc = dc;
	return SUCCESS;
}

int DataCenterProxy::SubmitJob(Job* pJob)
{
	cout<<"Submitting job\n";
	m_dc->AddJobsToWaitingList(pJob);
	return SUCCESS;
}

std::unordered_map<int,Node*> DataCenterProxy:: GetResourceData()
{
	cout<<"Proxy resource"<<endl;
	return m_dc->GetResourceData();
	//return std::unordered_map<int,Node*>();
}
