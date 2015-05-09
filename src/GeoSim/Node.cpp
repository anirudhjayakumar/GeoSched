/*
 * Node.cpp
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#include "Node.h"
#include "common.h"
Node::Node() {
	// TODO Auto-generated constructor stub

}
int nTotalCPU;
	int nTotalMem;
	int nFreeCPU;
	int nFreeMem;
	int nNodeID;
	std::vector<Task*> vTasks;

int Node:: getFreeCPU(){
		return nFreeCPU;
}
int Node:: getTotalCPU(){
    return nTotalCPU;
}
int Node:: getTotalMem(){
    return nTotalMem;
}
	int Node:: getFreeMem(){
		return nFreeMem;
	}
	int Node:: getNodeID(){
		return nNodeID;
	}
	int Node::increaseCPU(int cpu){
	    nFreeCPU= nFreeCPU+ cpu;
	    return SUCCESS;
	}
	int Node::decreaseCPU(int cpu){
		    if(nFreeCPU>cpu){
		    nFreeCPU= nFreeCPU- cpu;
		    return SUCCESS;
		    }
		    return FAIL;
		}
	int Node::increaseMem(int mem){
		    nFreeMem= nFreeMem+ mem;
		    return SUCCESS;
		}
		int Node::decreaseMem(int mem){
			    if(nFreeMem>mem){
			    nFreeMem= nFreeMem- mem;
			    return SUCCESS;
			    }
			    return FAIL;
			}



Node::~Node() {
	// TODO Auto-generated destructor stub
}

