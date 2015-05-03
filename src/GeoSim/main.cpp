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

using namespace std;
const string workload_1("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/5_1.csv");
const string workload_2("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/5_2.csv");
const string workload_3("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/5_3.csv");
const string workload_4("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/5_4.csv");
const string workload_5("/Users/harshitdokania/Desktop/cs525/geosched/workloads/google/5_5.csv");


int main()
{
	cout << "Running" << endl;
	DataCenter chile(1, workload_1);
	DataCenter finland(2, workload_2);
	DataCenter singapore(3, workload_3);
	DataCenter oregon(4, workload_4);
	DataCenter iowa(5, workload_5);
	DataCenterProxy *dcProxy= new DataCenterProxy[5];
	dcProxy[0].Initialize(&chile);
	dcProxy[1].Initialize(&finland);
	dcProxy[2].Initialize(&singapore);
	dcProxy[3].Initialize(&oregon);
	dcProxy[4].Initialize(&iowa);
	//unordered_map<int,Node*> 	m_map1= dcProxy[0].GetResourceData();
			/*
		dcProxy[1].GetResourceData()
		dcProxy[2].GetResourceData()
		dcProxy[3].GetResourceData()
*/

	chile.set_dataCenterProxies(dcProxy);
	finland.set_dataCenterProxies(dcProxy);
	singapore.set_dataCenterProxies(dcProxy);
	oregon.set_dataCenterProxies(dcProxy);
    iowa.set_dataCenterProxies(dcProxy);
    chile.Simulation();
    finland.Simulation();
    singapore.Simulation();
    oregon.Simulation();
    iowa.Simulation();
    cout<<"End of Simulation"<<endl;






	return 0;
}


