/*
 * main.cpp
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#include <iostream>
#include "common.h"
#include <fstream>
#include "DataCenter.h"
#include "DataCenterProxy.h"
#include "ConfigAccessor.h"
using namespace std;
const string workload_chile("../../workloads/google/5_1.csv");
const string workload_finland("../../workloads/google/5_2.csv");
const string workload_singapore("../../workloads/google/5_3.csv");
const string workload_oregon("/../../workloads/google/5_4.csv");
const string workload_iowa("../../workloads/google/5_5.csv");
const string config_xml("config.xml");


int main()
{
	//Barrier(5);
	cout << "Running" << endl;
	
	Barrier oBarrier(DC_COUNT);

	DataCenter chile(CHILE, workload_chile,&oBarrier);
	DataCenter finland(FINLAND, workload_finland,&oBarrier);
	DataCenter singapore(SINGAPORE, workload_singapore,&oBarrier);
	DataCenter oregon(OREGON, workload_oregon,&oBarrier);
	DataCenter iowa(IOWA, workload_iowa,&oBarrier);
	
	DataCenterProxy *dcProxy= new DataCenterProxy[DC_COUNT];
	dcProxy[CHILE].Initialize(&chile);
	dcProxy[FINLAND].Initialize(&finland);
	dcProxy[SINGAPORE].Initialize(&singapore);
	dcProxy[OREGON].Initialize(&oregon);
	dcProxy[IOWA].Initialize(&iowa);

	ConfigAccessor oAccessor;
	oAccessor.Init(config_xml);

	chile.Initialize(dcProxy,&oAccessor);
	chile.Initialize(dcProxy,&oAccessor);
	finland.Initialize(dcProxy,&oAccessor);
	singapore.Initialize(dcProxy,&oAccessor);
	oregon.Initialize(dcProxy,&oAccessor);
    iowa.Initialize(dcProxy,&oAccessor);


    chile.UpdateResourceData();
    finland.UpdateResourceData();
    singapore.UpdateResourceData();
    oregon.UpdateResourceData();
    iowa.UpdateResourceData();


    chile.Simulation();
    finland.Simulation();
    singapore.Simulation();
    oregon.Simulation();
    iowa.Simulation();


    chile.Join();
	finland.Join();
	singapore.Join();
	oregon.Join();
	iowa.Join();
    cout<<"End of Simulation"<<endl;







	return 0;
}


