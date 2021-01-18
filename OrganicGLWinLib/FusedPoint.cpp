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