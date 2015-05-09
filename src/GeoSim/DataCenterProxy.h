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
    std::string GetName();
    double          TemperatureNextHours(int hour);
    double          ElectricityNextHours(int hour);
    bool			isAirEco();
private:
	DataCenter *m_dc;
};

#endif /* DATACENTERPROXY_H_ */
