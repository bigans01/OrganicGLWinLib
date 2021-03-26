#include "stdafx.h"
#include "PointToMassRelationshipJudge.h"

void PointToMassRelationshipJudge::insertShellSliceForSPolyID(int in_sPolyID,
	STriangle* in_sTriangleRef,
	glm::vec3 in_shellSliceBaseEmptyNormal,
	glm::vec3 in_relationshipPointToCompare,
	std::map<int, SPoly*> in_shellSliceClippingShellMap)
{
	MassZoneShellSlice newShellSlice(in_sTriangleRef, in_shellSliceBaseEmptyNormal, in_relationshipPointToCompare, in_shellSliceClippingShellMap);
	shellSliceMap[in_sPolyID] = newShellSlice;
}

void PointToMassRelationshipJudge::executeJudgementOnShellSlices()
{
	// Step 1: run analysis on all the slices, to compute the appropriate values of the analyzedType in each.
	auto shellSlicesBegin = shellSliceMap.begin();
	auto shellSlicesEnd = shellSliceMap.end();
	int coplanarAnalysisCount = 0;
	int withinMassAnalysisCount = 0;
	for (; shellSlicesBegin != shellSlicesEnd; shellSlicesBegin++)
	{
		shellSlicesBegin->second.runAnalysis();
		PointToMassRelationshipType currentAnalysisResult = shellSlicesBegin->second.getAnalysisResult();
		if (currentAnalysisResult == PointToMassRelationshipType::COPLANAR_TO_STRIANGLE)
		{
			coplanarAnalysisCount++;
		}
		else if (currentAnalysisResult == PointToMassRelationshipType::WITHIN_MASS)
		{
			withinMassAnalysisCount++;
		}

	}

	// Step 2: analyze the results of each.
	// 
	// -if ALL of the analyzedType values == PointToMassRelationshipType::NO_LINE_OF_SIGHT	->	the point is outside of the mass.
	// -if there are no cases of WITHIN_MASS or COPLANAR_TO_STRIANGLE						->	the point is outside of the mass.
	// -if there is at least one value of OUTSIDE_OF_MASS (this maintains line of sight)	->  the point is outside of the mass.
	// -if there is at least one COPLANAR_TO_SHELL_SPOLY									->  the point is WITHIN the mass.
	// -if the number of WITHIN_MASS counts equals the entire size of the shell slices		->  the point is WITHIN the mass.

}