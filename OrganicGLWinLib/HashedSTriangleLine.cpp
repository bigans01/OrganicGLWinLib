#include "stdafx.h"
#include "HashedSTriangleLine.h"

HashedSTriangleLine::HashedSTriangleLine(glm::vec3 in_pointA, glm::vec3 in_pointB)
{
	pointA = in_pointA;
	pointB = in_pointB;
	buildHashes();
}

void HashedSTriangleLine::buildHashes()
{
	std::string pointAConverted = HashUtils::convertVec3ToHash(pointA);
	std::string pointBConverted = HashUtils::convertVec3ToHash(pointB);

	hashAB = HashUtils::sha256(pointAConverted + pointBConverted);
	hashBA = HashUtils::sha256(pointBConverted + pointAConverted);
}

bool HashedSTriangleLine::doLinesMatch(HashedSTriangleLine* in_otherLine)
{
	bool linesMatch = false;
	if
	(
		(hashAB == in_otherLine->hashAB)
		||
		(hashAB == in_otherLine->hashBA)
	)
	{
		linesMatch = true;
	}

	return linesMatch;
}

void HashedSTriangleLine::printHashedLineStats()
{
	std::cout << " point A: " << pointA.x << ", " << pointA.y << ", " << pointA.z << " | pointB ";
	std::cout << pointB.x << ", " << pointB.y << ", " << pointB.z << std::endl;
}