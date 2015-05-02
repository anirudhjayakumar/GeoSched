/*
 * DataCenter.cpp
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#include "DataCenter.h"
#include "common.h"
#include "Node.h"
#include "Job.h"
using namespace std;
DataCenter::DataCenter() {
	// TODO Auto-generated constructor stub

}

DataCenter::~DataCenter() {
	// TODO Auto-generated destructor stub
}

int DataCenter::Initialize(std::vector<DataCenterProxy *> dataCenterProxies, const std::string &resourceXML)
{
	return SUCCESS;
}

int DataCenter::AddJobsToWaitingList()
{
	return SUCCESS;
}

void DataCenter::PrintUtilization()
{
	return;
}

std::unordered_map<int,Node*> DataCenter::GetResourceData()
{
	return std::unordered_map<int,Node*>();
}

void 	DataCenter::StartSimulation()
{
	return;
}
