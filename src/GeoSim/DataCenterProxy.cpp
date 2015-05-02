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
DataCenterProxy::DataCenterProxy() {
	// TODO Auto-generated constructor stub

}

DataCenterProxy::~DataCenterProxy() {
	// TODO Auto-generated destructor stub
}

int Initialize(DataCenter *dc)
{
	return SUCCESS;
}

int SubmitJob(Job* pJob)
{
	return SUCCESS;
}

std::unordered_map<int,Node*> GetResourceData()
{
	return std::unordered_map<int,Node*>();
}
