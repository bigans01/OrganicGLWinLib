#include "stdafx.h"
#include "GuestLineReactor.h"

void GuestLineReactor::buildNonBorderLine()
{
	auto firstPoint = guestFusionAnalysisRef->fusedPoints.fusedPointMap.begin();
	auto secondPoint = guestFusionAnalysisRef->fusedPoints.fusedPointMap.rbegin();

	glm::vec3 pointA = firstPoint->second.point;
	glm::vec3 pointB = secondPoint->second.point;

	resultantLine.type = IntersectionType::NON_BOUND;
	resultantLine.line.pointA = pointA;
	resultantLine.line.pointB = pointB;
	resultantLine.emptyNormal = guestFusionAnalysisRef->sPolyRef->polyEmptyNormal;

	std::cout << "(GuestLineReactor): finished producing NON_BOUND, stats are: " << std::endl;
	std::cout << "(GuestLineReactor): point A: " << resultantLine.line.pointA.x << ", " << resultantLine.line.pointA.y << ", " << resultantLine.line.pointA.z << std::endl;
	std::cout << "(GuestLineReactor): point B: " << resultantLine.line.pointB.x << ", " << resultantLine.line.pointB.y << ", " << resultantLine.line.pointB.z << std::endl;
	std::cout << "(GuestLineReactor): empty normal: " << resultantLine.emptyNormal.x << ", " << resultantLine.emptyNormal.y << ", " << resultantLine.emptyNormal.z << std::endl;
}