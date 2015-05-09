/*
 * DataCenterProxy.h
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#ifndef DATACENTERPROXY_H_
#define DATACENTERPROXY_H_

#include <unordered_map>

/*
 * acts as a wrapper to datacenter
 */
class DataCenter;
class Node;
class Job;
class DataCenterProxy {
public:
	DataCenterProxy();
	virtual ~DataCenterProxy();
	int Initialize(DataCenter *dc);
	int SubmitJob(Job* pJob);
	std::unordered_map<int,Node*> GetResourceData();
    void InformLeaving();
    std::string GetName();
    double          TemperatureNextHours(std::string date, int hour);
    double          ElectricityNextHours(std::string date, int hour);
private:
	DataCenter *m_dc;
};

#endif /* DATACENTERPROXY_H_ */
