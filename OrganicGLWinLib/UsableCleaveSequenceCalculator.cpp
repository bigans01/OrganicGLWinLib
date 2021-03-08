#include "stdafx.h"
#include "UsableCleaveSequenceCalculator.h"

void UsableCleaveSequenceCalculator::runJudgement()
{
	OperableIntSet baseSet;
	usableLogger.log("(UsableCleaveSequenceCalculator): judging for the CleaveSequence ID of: ", judgingForCleaveSequenceID, "\n");
	baseSet.intSet = cleaveSequenceCandidateListRef->getCandidateSet();	// load all of the candidates from the border line we're looking at.
	baseSet.intSet.erase(judgingForCleaveSequenceID);					// remove the candidate that is the CleaveSequenceID we are judging for.

	
	std::map<int, DistanceToPoint> distances;
	auto baseSetBegin = baseSet.intSet.begin();
	auto baseSetEnd = baseSet.intSet.end();
	for (; baseSetBegin != baseSetEnd; baseSetBegin++)
	{
		DistanceToPoint currentShortestPointStats;
		currentShortestPointStats = (*cleaveMapRef)[*baseSetBegin].fetchClosestPointOnBorderLineID(judgementOriginPoint, borderLineIDToJudgeFor, usableLoggerDebugLevel);
		distances[*baseSetBegin] = currentShortestPointStats;
	}

	
	std::map<int, CategorizedLine> fetchedCategorizedLines;
	auto distancesBegin = distances.begin();
	auto distancesEnd = distances.end();
	for (; distancesBegin != distancesEnd; distancesBegin++)
	{
		fetchedCategorizedLines[distancesBegin->first] = (*cleaveMapRef)[distancesBegin->first].fetchCategorizedLineFromSequenceThatHasBorderPoint(distancesBegin->second.point, usableLoggerDebugLevel);
		
	}
	

	
	usableLogger.log("(UsableCleaveSequenceCalculator): printing out fetched categorized lines: ", "\n");
	if (usableLogger.isLoggingSet())
	{
		auto printLinesBegin = fetchedCategorizedLines.begin();
		auto printLinesEnd = fetchedCategorizedLines.end();
		for (; printLinesBegin != printLinesEnd; printLinesBegin++)
		{
			usableLogger.log("(UsableCleaveSequenceCalculator): chosen categorized line for CleaveSequence having id: ", printLinesBegin->first, "\n");
			usableLogger.log("(UsableCleaveSequenceCalculator): >> point A: ", printLinesBegin->second.line.pointA.x, ", ", printLinesBegin->second.line.pointA.y, ", ", printLinesBegin->second.line.pointA.z,
							" | >> point B: ", printLinesBegin->second.line.pointB.x, ", ", printLinesBegin->second.line.pointA.y, ", ", printLinesBegin->second.line.pointA.z,
							" | >> normal: ", printLinesBegin->second.emptyNormal.x, ", ", printLinesBegin->second.emptyNormal.y, ", ", printLinesBegin->second.emptyNormal.z, "\n");
		}
	}
	
	auto judgementRunBegin = fetchedCategorizedLines.begin();
	auto judgementRunEnd = fetchedCategorizedLines.end();
	for (; judgementRunBegin != judgementRunEnd; judgementRunBegin++)
	{
		if (executeJudgementOnCandidate(judgementRunBegin->second) == false)
		{
			usableLogger.log("(UsableCleaveSequenceCalculator): CleaveSequence with ID ", judgementRunBegin->first, " isn't usable.", "\n");
			unusableSet.intSet.insert(judgementRunBegin->first);
		}
	}

}

bool UsableCleaveSequenceCalculator::executeJudgementOnCandidate(CategorizedLine in_lineToJudge)
{
	bool isCandidateUsable = true;


	bool areLinesCoLinear = QuatUtils::checkIfLinesAreColinear(
																judgingLine.line.pointA,
																judgingLine.line.pointB,
																in_lineToJudge.line.pointA,
																in_lineToJudge.line.pointB, 
																PolyDebugLevel::NONE
															  );
	if (areLinesCoLinear == true)
	{
		usableLogger.log("(UsableCleaveSequenceCalculator): notice, line to judge is colinear!", "\n");
		isCandidateUsable = false;
	}
	return isCandidateUsable;
}

OperableIntSet UsableCleaveSequenceCalculator::getUnusables()
{
	return unusableSet;
}