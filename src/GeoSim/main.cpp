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

//all these paths are relative
const string workload_chile("../../workloads/google/5_1_.csv");
const string workload_finland("../../workloads/google/5_2_.csv");
const string workload_singapore("../../workloads/google/5_3_.csv");
const string workload_oregon("../../workloads/google/5_4_.csv");
const string workload_iowa("../../workloads/google/5_5_.csv");

const string output_chile("chile.trace");
const string output_finland("finalnd.trace");
const string output_singapore("singapore.trace");
const string output_oregon("oregon.trace");
const string output_iowa("iowa.trace");

const string temp_chile("../../datacenters/temperature/chile.csv");
const string temp_finland("../../datacenters/temperature/finland.csv");
const string temp_iowa("../../datacenters/temperature/iowa.csv");
const string temp_oregon("../../datacenters/temperature/oregon.csv");
const string temp_singapore("../../datacenters/temperature/singapore.csv");

const string elec_chile("../../datacenters/electricity/chile.csv");
const string elec_finland("../../datacenters/electricity/finland.csv");
const string elec_iowa("../../datacenters/electricity/iowa.csv");
const string elec_oregon("../../datacenters/electricity/oregon.csv");
const string elec_singapore("../../datacenters/electricity/singapore.csv");

const string config_xml("config.xml");






int main()
{
	
	cout << "GeoSim Simulation" << endl;
	
	Barrier oBarrier(DC_COUNT);

	DataCenter chile(CHILE, workload_chile,&oBarrier, "Chile", -3, output_chile, temp_chile, elec_chile,false);
	DataCenter finland(FINLAND, workload_finland,&oBarrier, "Finland",3, output_finland, temp_finland, elec_finland,true);
	DataCenter singapore(SINGAPORE, workload_singapore,&oBarrier, "Singapore", 8, output_singapore, temp_singapore, elec_singapore,false);
	DataCenter oregon(OREGON, workload_oregon,&oBarrier, "Oregon", -7, output_oregon, temp_oregon, elec_oregon,true);
	DataCenter iowa(IOWA, workload_iowa,&oBarrier,"Iowa", -5, output_iowa, temp_iowa, elec_iowa,true);
	
    
    
	DataCenterProxy *dcProxy= new DataCenterProxy[DC_COUNT];
	dcProxy[CHILE].Initialize(&chile);
	dcProxy[FINLAND].Initialize(&finland);
	dcProxy[SINGAPORE].Initialize(&singapore);
	dcProxy[OREGON].Initialize(&oregon);
	dcProxy[IOWA].Initialize(&iowa);
    
       /*oregon.TemperatureNextHours("201313",17);
    singapore.TemperatureNextHours("201313",5);
    iowa.TemperatureNextHours("201313",5);*/
    
   
    /*
    finalnd.TemperatureNextHours("201313",5);
    chile.TemperatureNextHours("201313",5);
    finalnd.TemperatureNextHours("201313",5);

    finalnd.TemperatureNextHours("201313",5);*/

    ConfigAccessor oAccessor;
	oAccessor.Init(config_xml);

	chile.Initialize(dcProxy,&oAccessor);
	finland.Initialize(dcProxy,&oAccessor);
	singapore.Initialize(dcProxy,&oAccessor);
	oregon.Initialize(dcProxy,&oAccessor);
    iowa.Initialize(dcProxy,&oAccessor);
   
   /* temp and electricity
    cout<<"Chile Temp: "<<chile.TemperatureNextHours("2013,1,31",17)<<endl;
    cout<<"Finland Temp"<<finland.TemperatureNextHours("2013,1,3",5)<<endl;
    cout<<"Chile.Elec:= "<<chile.ElectricityNextHours("2013,1,3",5)<<endl;
    cout<<"Chile.Elec:= "<<chile.ElectricityNextHours("2013,1,3",5)<<endl;
    
    */
    
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


