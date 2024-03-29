#pragma once

#ifndef COLLECTIVEVERDICTMACHINE_H
#define COLLECTIVEVERDICTMACHINE_H

#include "MappableIndividualVerdict.h"

class CollectiveVerdictMachine
{
	public:
		std::map<int, MappableIndividualVerdict> individualVerdictMap;
		void insertMappableIndividualVerdict(MappableIndividualVerdict in_mappableIndividualVerdict);
		bool doesAnomalousMassSPolyExist();

};

#endif
