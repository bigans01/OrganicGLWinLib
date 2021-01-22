#include "stdafx.h"
#include "IntersectionLineGroup.h"

IntersectionLineGroup::IntersectionLineGroup(PolyDebugLevel in_polyDebugLevel)
{
	lineGroupLogger.setDebugLevel(in_polyDebugLevel);
	returnLine.completedAnalysis.fusionAnalysisLogger.setDebugLevel(in_polyDebugLevel);
}

void IntersectionLineGroup::setFusionAnalyzerSPolyRef(SPoly* in_sPolyRef)
{
	returnLine.completedAnalysis.setSPolyRef(in_sPolyRef);
}

void IntersectionLineGroup::addIntersectionLine(IntersectionLine in_intersectionLine)
{
	lineMap[numberOfIntersectionLines++] = in_intersectionLine;
	//std::cout << "Intersection line border value is: " << in_intersectionLine.numberOfBorderLines << std::endl;
}

void IntersectionLineGroup::runFusionAnalysisAndDetermineClassifications()
{
	returnLine.completedAnalysis.setMapRefAndRunAnalysis(&lineMap);
	returnLine.completedAnalysis.determineClassifications();
}

int IntersectionLineGroup::findNumberOfBorderLineIntercepts()
{
	int numberOfIntercepts = 0;
	auto mapIter = lineMap.begin();
	auto mapEnd = lineMap.end();
	//std::cout << "Looping through " << lineMap.size() << " line(s)" << std::endl;
	for (mapIter; mapIter != mapEnd; mapIter++)
	{
		if (mapIter->second.numberOfBorderLines == 1)
		{
			numberOfIntercepts++;
		}
		//std::cout << "Line looped. " << std::endl;
	}
	//std::cout << "Num intercepts: " << numberOfIntercepts << std::endl;
	return numberOfIntercepts;
}

void IntersectionLineGroup::insertFusionCandidateIntoAnalyzer(FusionCandidateOrigin in_fusionCandidateOrigin, int in_lineIndex, FusionCandidate in_fusionCandidate)
{
	returnLine.completedAnalysis.insertFusionCandidate(in_fusionCandidateOrigin, in_lineIndex, in_fusionCandidate);
}

void IntersectionLineGroup::reset()
{
	//FusedIntersectionLine newLineReplacement;
	//returnLine = newLineReplacement;
	IntersectionLine blankIntersectionLine;
	returnLine.deprecatedLine = blankIntersectionLine;
	numberOfIntersectionLines = 0;		// reset the number of intersection lines
	lineMap.clear();					// clear the map
	returnLine.completedAnalysis.clearCandidateData();
}

IntersectionLine* IntersectionLineGroup::findSingleBorderLine()
{
	IntersectionLine* returnPtr = &lineMap[0];		// make the compiler happy
	auto mapIter = lineMap.begin();
	auto mapEnd = lineMap.end();
	for (mapIter; mapIter != mapEnd; mapIter++)
	{
		if (mapIter->second.numberOfBorderLines == 1)		// grab the one with the border line
		{
			returnPtr = &mapIter->second;
		}
	}
	return returnPtr;
}

IntersectionLine* IntersectionLineGroup::findSingleNonBorderLine()
{
	IntersectionLine* returnPtr = &lineMap[0];		// make the compiler happy
	auto mapIter = lineMap.begin();
	auto mapEnd = lineMap.end();
	for (mapIter; mapIter != mapEnd; mapIter++)
	{
		if (mapIter->second.numberOfBorderLines == 0)		// grab the one with no border line
		{
			returnPtr = &mapIter->second;
		}
	}
	return returnPtr;
}