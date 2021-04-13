#include "stdafx.h"
#include "CoplanarFusionMachine.h"

void CoplanarFusionMachine::insertCoplanarRelationship(int in_polySetGroupID, int in_trackedSPolyID, SPoly* in_trackedSPolyRef, int in_relatedSPolyID, SPoly* in_relatedSPolyRef)
{
	if (in_polySetGroupID == 0)
	{
		oldSetTracker.insertCoplanarRelationship(in_trackedSPolyID, in_trackedSPolyRef, in_relatedSPolyID, in_relatedSPolyRef);
	}
	else if (in_polySetGroupID == 1)
	{
		newSetTracker.insertCoplanarRelationship(in_trackedSPolyID, in_trackedSPolyRef, in_relatedSPolyID, in_relatedSPolyRef);
	}
}

void CoplanarFusionMachine::printRelationshipsInTrackers()
{
	std::cout << "(CoplanarFusionMachine): |||||||| Printing relationships of oldSetTracker: " << std::endl;
	oldSetTracker.printRelationshipData();
	std::cout << "(CoplanarFusionMachine): |||||||| Printing relationships of newSetTracker: " << std::endl;
	newSetTracker.printRelationshipData();
}

void CoplanarFusionMachine::insertSPolyGroupMapLink(int in_sPolyID, int in_groupID, SPoly* in_sPolyRef)
{
	//sPolyGroupMap[in_sPolyID] = in_groupID;
	SPolyRefAndGroup newRefAndGroup(in_groupID, in_sPolyRef);
	sPolyGroupMap[in_sPolyID] = newRefAndGroup;
}

void CoplanarFusionMachine::determineFusionType()
{
	int group0Count = 0;
	int group1Count = 0;

	// check group0 and group1 counts.
	auto sPolyGroupMapBegin = sPolyGroupMap.begin();
	auto sPolyGroupMapEnd = sPolyGroupMap.end();
	for (; sPolyGroupMapBegin != sPolyGroupMapEnd; sPolyGroupMapBegin++)
	{
		// if the SPoly is group 1 and has valid CleaveSequences, increment group0Count.
		if
		(
			(sPolyGroupMapBegin->second.groupID == 0)
			&&
			(sPolyGroupMapBegin->second.sPolyRef->cleaveMap.size() != 0)
		)
		{
			group0Count++;
		}
		else if
		(
			(sPolyGroupMapBegin->second.groupID == 1)
			&&
			(sPolyGroupMapBegin->second.sPolyRef->cleaveMap.size() != 0)
		)
		{
			group1Count++;
		}
	}

	// check the counts, to determine the enum type.
	// both are > 0
	if 
	(
		(group0Count > 0)
		&&
		(group1Count > 0)
	)
	{

	}

	// group0 is > 0
	else if
	(
		(group0Count > 0)
	)
	{
		std::cout << "Type would be: KEEP_NEW_APPEND_OLD" << std::endl;
	}

	// group1 is > 0
	else if
	(
		(group1Count > 0)
	)
	{
		std::cout << "Type would be: KEEP_OLD_APPEND_NEW" << std::endl;
	}

}