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
const string workload_chile("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/5_1.csv");
const string workload_finland("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/5_2.csv");
const string workload_singapore("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/5_3.csv");
const string workload_oregon("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/5_4.csv");
const string workload_iowa("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/5_5.csv");
const string output_chile("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/chile.trace");
const string output_finland("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/finalnd.trace");
const string output_singapore("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/singapore.trace");
const string output_oregon("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/oregon.trace");
const string output_iowa("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/iowa.trace");
const string config_xml("config.xml");






int main()
{
	
	cout << "GeoSim Simulation" << endl;
	
	Barrier oBarrier(DC_COUNT);

	DataCenter chile(CHILE, workload_chile,&oBarrier, "Chile", -3, output_chile);
	DataCenter finland(FINLAND, workload_finland,&oBarrier, "Finland",3, output_finland);
	DataCenter singapore(SINGAPORE, workload_singapore,&oBarrier, "Singapore", -4, output_singapore);
	DataCenter oregon(OREGON, workload_oregon,&oBarrier, "Oregon", -7, output_oregon);
	DataCenter iowa(IOWA, workload_iowa,&oBarrier,"Iowa", -5, output_iowa);
	
    
    
	DataCenterProxy *dcProxy= new DataCenterProxy[DC_COUNT];
	dcProxy[CHILE].Initialize(&chile);
	dcProxy[FINLAND].Initialize(&finland);
	dcProxy[SINGAPORE].Initialize(&singapore);
	dcProxy[OREGON].Initialize(&oregon);
	dcProxy[IOWA].Initialize(&iowa);
    
   

	ConfigAccessor oAccessor;
	oAccessor.Init(config_xml);

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


