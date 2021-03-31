#include "stdafx.h"
#include "PointToMassRelationshipJudge.h"

void PointToMassRelationshipJudge::insertShellSliceForSPolyID(int in_sPolyID,
	int in_sTriangleID,
	STriangle* in_sTriangleRef,
	glm::vec3 in_shellSliceBaseEmptyNormal,
	glm::vec3 in_relationshipPointToCompare,
	std::map<int, SPoly*> in_shellSliceClippingShellMap)
{
	MassZoneShellSlice newShellSlice(in_sTriangleRef, in_sPolyID, in_sTriangleID, in_shellSliceBaseEmptyNormal, in_relationshipPointToCompare, in_shellSliceClippingShellMap);
	shellSliceMap[in_sPolyID] = newShellSlice;
}

IndividualVerdict PointToMassRelationshipJudge::executeJudgementOnShellSlices()
{
	// a map for storing the analysis results of each shell slice.
	//std::map<int, PointToMassRelationshipType> analysisMap;

	// Step 1: run analysis on all the slices, to compute the appropriate values of the analyzedType in each.
	auto shellSlicesBegin = shellSliceMap.begin();
	auto shellSlicesEnd = shellSliceMap.end();
	int coplanarAnalysisCount = 0;
	int withinMassAnalysisCount = 0;
	int outsideOfMassAnalysisCount = 0;
	int noLineOfSightCount = 0;
	for (; shellSlicesBegin != shellSlicesEnd; shellSlicesBegin++)
	{
		shellSlicesBegin->second.setShellSliceDebugLevel(PolyDebugLevel::DEBUG);
		shellSlicesBegin->second.runAnalysis();
		PointToMassRelationshipType currentAnalysisResult = shellSlicesBegin->second.getAnalysisResult();
		if (currentAnalysisResult == PointToMassRelationshipType::COPLANAR_TO_STRIANGLE)
		{
			coplanarAnalysisCount++;
			analysisMap[shellSlicesBegin->first] = currentAnalysisResult;
			break;	// we can break out of the rest of the shell slices, the moment we discover a coplanar result.
		}
		else if (currentAnalysisResult == PointToMassRelationshipType::WITHIN_MASS)
		{
			withinMassAnalysisCount++;
			analysisMap[shellSlicesBegin->first] = currentAnalysisResult;
		}
		else if (currentAnalysisResult == PointToMassRelationshipType::OUTSIDE_OF_MASS)
		{
			outsideOfMassAnalysisCount++;
			analysisMap[shellSlicesBegin->first] = currentAnalysisResult;
		}
		else if (currentAnalysisResult == PointToMassRelationshipType::NO_LINE_OF_SIGHT)
		{
			noLineOfSightCount++;
			analysisMap[shellSlicesBegin->first] = currentAnalysisResult;
		}

	}

	// optional: print the analysis map.
	auto analysisBegin = analysisMap.begin();
	auto analysisEnd = analysisMap.end();
	std::cout << "::::::::::::::::: Printing resulting analysis. " << std::endl;
	for (; analysisBegin != analysisEnd; analysisBegin++)
	{
		switch (analysisBegin->second)
		{
			case (PointToMassRelationshipType::COPLANAR_TO_STRIANGLE): { std::cout << "[" << analysisBegin->first << "]: COPLANAR_TO_STRIANGLE" << std::endl; break; };
			case (PointToMassRelationshipType::WITHIN_MASS): { std::cout << "[" << analysisBegin->first << "]: WITHIN_MASS" << std::endl; break; };
			case (PointToMassRelationshipType::OUTSIDE_OF_MASS): { std::cout << "[" << analysisBegin->first << "]: OUTSIDE_OF_MASS" << std::endl; break; };
			case (PointToMassRelationshipType::NO_LINE_OF_SIGHT): { std::cout << "[" << analysisBegin->first << "]: NO_LINE_OF_SIGHT" << std::endl; break; };
		}
	}
	//int waitVal = 3;
	//std::cin >> waitVal;
	// Step 2: analyze the results of each.
	IndividualVerdict individualResult(determineVerdict(), analysisMap);
	return individualResult;
	//return determineVerdict();

}

bool PointToMassRelationshipJudge::determineVerdict()
{
	// 
	// -if ALL of the analyzedType values == PointToMassRelationshipType::NO_LINE_OF_SIGHT	->	the point is outside of the mass. (runTestDoAllHaveNoLineOfSight() == true)
	// -if there are no cases of WITHIN_MASS or COPLANAR_TO_STRIANGLE						->	the point is outside of the mass. (runTestAnyCasesOfWithinOrCoplanar() == false)
	// -if there is at least one value of OUTSIDE_OF_MASS (this maintains line of sight)	->  the point is outside of the mass. (runTestAnyCaseOfOutsideOfMass() == true)
	// -if there is at least one COPLANAR_TO_SHELL_SPOLY									->  the point is WITHIN the mass.
	// -if the number of WITHIN_MASS counts equals the entire size of the shell slices		->  the point is WITHIN the mass.
	bool shouldPointBeClipped = false;
	if
	(
		(runTestDoAllHaveNoLineOfSight() == true)
		||
		(runTestAnyCasesOfWithinOrCoplanar() == false)
		||
		(runTestAnyCaseOfOutsideOfMass() == true)
	)
	{
		std::cout << "||||| ----> Verdict of this judged point is that it should be clipped. " << std::endl;
		shouldPointBeClipped = true;
	}
	return shouldPointBeClipped;
}

bool PointToMassRelationshipJudge::runTestDoAllHaveNoLineOfSight()
{
	bool testResult = false;
	int sizeOfAnalysisMap = analysisMap.size();
	int noLOSCounter = 0;	// number of detected analysis of type NO_LINE_OF_SIGHT
	auto analysisBegin = analysisMap.begin();
	auto analysisEnd = analysisMap.end();
	for (; analysisBegin != analysisEnd; analysisBegin++)
	{
		if (analysisBegin->second == PointToMassRelationshipType::NO_LINE_OF_SIGHT)
		{
			noLOSCounter++;
		}
	}

	// if the counter equals the map size, it needs to be purged.
	if (noLOSCounter == sizeOfAnalysisMap)
	{
		testResult = true;
	}
	return testResult;
}
bool PointToMassRelationshipJudge::runTestAnyCasesOfWithinOrCoplanar()
{
	bool wereAnyWithinsOrCoplanarsFound = false;
	auto analysisBegin = analysisMap.begin();
	auto analysisEnd = analysisMap.end();
	int coplanarOrWithinCounter = 0;
	for (; analysisBegin != analysisEnd; analysisBegin++)
	{
		if
		(
			(analysisBegin->second == PointToMassRelationshipType::WITHIN_MASS)
			||
			(analysisBegin->second == PointToMassRelationshipType::COPLANAR_TO_STRIANGLE)
		)
		{
			wereAnyWithinsOrCoplanarsFound = true;	
			break;	// break, as soon as this condition is found (if it is) -- we have found what we needed.
		}
	}
	return wereAnyWithinsOrCoplanarsFound;
}
bool PointToMassRelationshipJudge::runTestAnyCaseOfOutsideOfMass()
{
	bool wereAnyOutsideOfMassFound = false;
	auto analysisBegin = analysisMap.begin();
	auto analysisEnd = analysisMap.end();
	for (; analysisBegin != analysisEnd; analysisBegin++)
	{
		if (analysisBegin->second == PointToMassRelationshipType::OUTSIDE_OF_MASS)
		{
			wereAnyOutsideOfMassFound = true;
			break;	// break, as soon as this condition is found (if it is) -- we have found what we needed.
		}
	}
	return wereAnyOutsideOfMassFound;

}