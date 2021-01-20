#include "stdafx.h"
#include "FusedPoint.h"

bool FusedPoint::doesPointMatch(glm::vec3 in_pointToMatchAgainst)
{
	bool result = false;
	if (in_pointToMatchAgainst == point)
	{
		result = true;
	}
	return result;
}

void FusedPoint::printData()
{
	std::cout << ">> FusedPoint: " << point.x << ", " << point.y << ", " << point.z << std::endl;
	auto subDataBegin = subDataVector.begin();
	auto subDataEnd = subDataVector.end();
	for (; subDataBegin != subDataEnd; subDataBegin++)
	{
		std::cout << "| Origin: ";
		if (subDataBegin->origin == FusionCandidateOrigin::HOST)
		{
			std::cout << " HOST ";
		}
		else if (subDataBegin->origin == FusionCandidateOrigin::GUEST)
		{
			std::cout << " GUEST ";
		}
		std::cout << "| Line index: " << subDataBegin->triangleLineIndex << " | is border line: " << subDataBegin->isBorderLine << " | border line value: " << subDataBegin->borderLineValue << std::endl;
	}
}

void FusedPoint::insertFusedPointSubData(FusedPointSubData in_fusedPointSubData)
{
	subDataVector.push_back(in_fusedPointSubData);
}

FusedPointMeta FusedPoint::getMetaForPoint(glm::vec3 in_pointToGetMetaFor)
{
	FusedPointMeta returnMeta;
	auto subDataBegin = subDataVector.begin();
	auto subDataEnd = subDataVector.end();
	for (; subDataBegin != subDataEnd; subDataBegin++)
	{
		returnMeta.numberOfSubdataEntries++;
		if (subDataBegin->isBorderLine == 1)
		{
			returnMeta.highestLineType = FusedPointSubdataLineType::BORDER;
		}
	}

	// determine the summary type
	if (returnMeta.numberOfSubdataEntries == 1)
	{
		if (returnMeta.highestLineType == FusedPointSubdataLineType::BORDER)
		{
			returnMeta.summary = FusedPointSummary::TYPICAL_BORDERLINE;
		}
		else if (returnMeta.highestLineType == FusedPointSubdataLineType::NONBORDER)
		{
			returnMeta.summary = FusedPointSummary::TYPICAL_NONBORDERLINE;
		}
	}
	else if (returnMeta.numberOfSubdataEntries == 2)
	{
		if (returnMeta.highestLineType == FusedPointSubdataLineType::BORDER)
		{
			returnMeta.summary = FusedPointSummary::PRECISE_BORDERLINE;
		}
		else if (returnMeta.highestLineType == FusedPointSubdataLineType::NONBORDER)
		{
			returnMeta.summary = FusedPointSummary::PRECISE_NONBORDERLINE;
		}
	}

	return returnMeta;
}