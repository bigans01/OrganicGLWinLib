#include "stdafx.h"
#include "PointToMassTrial.h"

void PointToMassTrial::insertShellSlice(glm::vec3 in_pointToJudge, 
										int in_sPolyID,
										int in_sTriangleID,
										STriangle* in_sTriangleRef,
										glm::vec3 in_shellSliceBaseEmptyNormal,
										std::map<int, SPoly*> in_shellSliceClippingShellMap
										)
{
	// first, check for any existing points.
	if (judgeMap.size() == 0) // it's empty, just do a typical insert.
	{
		judgeMap[0].point = in_pointToJudge;	// set the point at the first index, so we can match against it later.
		judgeMap[0].judge.insertShellSliceForSPolyID(in_sPolyID, 
													in_sTriangleID, 
													in_sTriangleRef, 
													in_shellSliceBaseEmptyNormal, 
													in_pointToJudge, 
													in_shellSliceClippingShellMap);
	}
	else if (judgeMap.size() != 0)
	{
		// map isn't empty, so we need to check if point exists.
		auto judgeMapBegin = judgeMap.begin();
		auto judgeMapEnd = judgeMap.end();
		bool doesPointAlreadyExist = false;		// was it found? 
		int foundPointIndexValue = 0;			// what index was it found at?
		for (; judgeMapBegin != judgeMapEnd; judgeMapBegin++)
		{
			if (judgeMapBegin->second.point == in_pointToJudge)
			{
				doesPointAlreadyExist = true;
				foundPointIndexValue = judgeMapBegin->first;	// store the index value
				break;											// break out, since we found it.
			}
		}

		// if we DID find it, just update that existing judge.
		if (doesPointAlreadyExist == true)
		{
			judgeMap[foundPointIndexValue].judge.insertShellSliceForSPolyID(in_sPolyID, 
																			in_sTriangleID, 
																			in_sTriangleRef, 
																			in_shellSliceBaseEmptyNormal, 
																			in_pointToJudge, 
																			in_shellSliceClippingShellMap);
		}

		// if we DID NOT find it, we need to insert a new point into the map.
		if (doesPointAlreadyExist == false)
		{
			int newIndexToUse = judgeMap.size();	// size of a map also equals the next index to use for map. (in our case)
			judgeMap[newIndexToUse].point = in_pointToJudge;
			judgeMap[newIndexToUse].judge.insertShellSliceForSPolyID(in_sPolyID, 
																	in_sTriangleID, 
																	in_sTriangleRef, 
																	in_shellSliceBaseEmptyNormal, 
																	in_pointToJudge, 
																	in_shellSliceClippingShellMap);
		}
	}

}

void PointToMassTrial::printJudgeMetaData()
{
	auto judgeMapBegin = judgeMap.begin();
	auto judgeMapEnd = judgeMap.end();
	for (; judgeMapBegin != judgeMapEnd; judgeMapBegin++)
	{
		std::cout << ">>>> Judged point: " << judgeMapBegin->second.point.x << ", " << judgeMapBegin->second.point.y << ", " << judgeMapBegin->second.point.z << std::endl;
		auto slicesToJudgeBegin = judgeMapBegin->second.judge.shellSliceMap.begin();
		auto slicesToJudgeEnd = judgeMapBegin->second.judge.shellSliceMap.end();
		for (; slicesToJudgeBegin != slicesToJudgeEnd; slicesToJudgeBegin++)
		{
			std::cout << ">> Slice for SPoly " << slicesToJudgeBegin->first << ": " << std::endl;
			auto rayCastAgainstsBegin = slicesToJudgeBegin->second.shellSliceClippingShellMap.begin();
			auto rayCastAgainstsEnd = slicesToJudgeBegin->second.shellSliceClippingShellMap.end();
			for (; rayCastAgainstsBegin != rayCastAgainstsEnd; rayCastAgainstsBegin++)
			{
				std::cout << "Raycast-against ID: " << rayCastAgainstsBegin->first << std::endl;
			}
		}
	}
}

void PointToMassTrial::executeAllJudgements(PolyDebugLevel in_polyDebugLevel)
{
	auto judgeMapBegin = judgeMap.begin();
	auto judgeMapEnd = judgeMap.end();
	for (; judgeMapBegin != judgeMapEnd; judgeMapBegin++)
	{
		IndividualVerdict currentVerdict = judgeMapBegin->second.runJudgements(in_polyDebugLevel);
		/*
		if (currentVerdict.shouldBeClipped == true)	// if the point should be clipped, insert that judged point into the vector.
		{
			clippablePointsVector.push_back(judgeMapBegin->second.point);
		}
		*/
		MappableIndividualVerdict mappableVerdict(judgeMapBegin->second.point, currentVerdict);
		collectiveVerdicts.insertMappableIndividualVerdict(mappableVerdict);
	}

	// if the BELOW is true; we will eventually need to special logic for Expunging the SPoly from the MassZone.
	//if (collectiveVerdicts.doesAnomalousMassSPolyExist() == false)		// if this statement were true, an anomaly was detected and must be clipped.
	//{
		auto individualVerdictsBegin = collectiveVerdicts.individualVerdictMap.begin();
		auto individualVerdictsEnd = collectiveVerdicts.individualVerdictMap.end();
		for (; individualVerdictsBegin != individualVerdictsEnd; individualVerdictsBegin++)
		{
			if (individualVerdictsBegin->second.shouldBeClipped == true)
			{
				clippablePointsVector.push_back(individualVerdictsBegin->second.point);
			}
		}
	//}
	
}