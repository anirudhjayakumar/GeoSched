/*
 * ConfigAccessor.h
 *
 *  Created on: May 2, 2015
 *      Author: Anirudh Jayakumar
 */

#ifndef CONFIGACCESSOR_H_
#define CONFIGACCESSOR_H_

#include <iostream>
#include <string>
#include <vector>

class ConfigAccessor {
public:
	ConfigAccessor();
	virtual ~ConfigAccessor();

	/*
	 * Init reads the xmls and stores all the data in the local
	 * state. When methods are called, the data is taken from the
	 * local state and returned to the caller.
	 * return FAIL/SUCCESS
	 */
	int 				Init(const std::string &filePath);
	/*
	 * All the below methods are getter for accessing the state of the
	 * config object
	 */
	int 				GetSchedPeriod();
	int 				GetResourceSyncPeriod();
	std::vector<int>	GetSchedClassToAvoid();

	// add more interfaces as we add more configs
};

#endif /* CONFIGACCESSOR_H_ */
