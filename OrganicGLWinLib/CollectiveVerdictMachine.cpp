#include "stdafx.h"
#include "CollectiveVerdictMachine.h"

void CollectiveVerdictMachine::insertMappableIndividualVerdict(MappableIndividualVerdict in_mappableIndividualVerdict)
{
	// first, check for any existing points.
	/*
	if (individualVerdictMap.size() == 0)	// it's empty, just do a typical insert.
	{
		individualVerdictMap[0] = in_mappableIndividualVerdict;
	}
	else if (individualVerdictMap.size() != 0)
	{
		auto individualVerdictsBegin = individualVerdictMap.begin();
		auto individualVerdictsEnd = individualVerdictMap.end();
		bool doesPointAlreadyExist = false;
		for (; individualVerdictsBegin != individualVerdictsEnd; individualVerdictsBegin++)
		{
			if (individualVerdictsBegin->second.point == in_mappableIndividualVerdict.point)
			{
				doesPointAlreadyExist = true;
			}
		}
	}
	*/
	individualVerdictMap[individualVerdictMap.size()] = in_mappableIndividualVerdict;
}

bool CollectiveVerdictMachine::doesAnomalousMassSPolyExist()
{
	bool doesAnomalyExist = false;
	std::set<PointToMassRelationshipType> relationshipList;
	auto individualVerdictsBegin = individualVerdictMap.begin();
	auto individualVerdictsEnd = individualVerdictMap.end();
	for (; individualVerdictsBegin != individualVerdictsEnd; individualVerdictsBegin++)
	{
		auto currentVerdictRelationshipsBegin = individualVerdictsBegin->second.relationshipTypeMap.begin();
		auto currentVerdictRelationshipsEnd = individualVerdictsBegin->second.relationshipTypeMap.end();
		for (; currentVerdictRelationshipsBegin != currentVerdictRelationshipsEnd; currentVerdictRelationshipsBegin++)
		{
			relationshipList.insert(currentVerdictRelationshipsBegin->second);
		}
	}

	// check to see if the set simultaneously contains WITHIN_MASS and OUTSIDE_OF_MASS.
	auto withinFinder = relationshipList.find(PointToMassRelationshipType::WITHIN_MASS);
	auto outsideOfFinder = relationshipList.find(PointToMassRelationshipType::OUTSIDE_OF_MASS);
	if
	(
		(
			(withinFinder != relationshipList.end())
		)
		&&
		(outsideOfFinder != relationshipList.end())
	)
	{
		
		doesAnomalyExist = true;
	}

	return doesAnomalyExist;	
}
