#include "stdafx.h"
#include "QuatRotationPoints.h"

void QuatRotationPoints::applyQuaternion(glm::quat in_quat)
{
	auto pointsStart = pointsRef.begin();
	auto pointsEnd = pointsRef.end();
	for (pointsStart; pointsStart != pointsEnd; pointsStart++)
	{
		//glm::vec3 currentPoint = **pointsStart;
		glm::vec3* currentPtr = *pointsStart;
		//*currentPtr = in_quat * *currentPtr;
		**pointsStart = in_quat * **pointsStart;
	}
}

void QuatRotationPoints::applyTranslation(glm::vec3 in_translation)
{
	auto pointsStart = pointsRef.begin();
	auto pointsEnd = pointsRef.end();
	for (pointsStart; pointsStart != pointsEnd; pointsStart++)
	{
		auto pointsPtr = *pointsStart;
		*pointsPtr += in_translation;
	}
}

void QuatRotationPoints::printPoints()
{
	std::vector<glm::vec3*>::iterator pointsStart = pointsRef.begin();
	std::vector<glm::vec3*>::iterator pointsEnd = pointsRef.end();
	for (pointsStart; pointsStart != pointsEnd; pointsStart++)
	{
		glm::vec3 lol;
		glm::vec3* lolPtr = &lol;
		glm::vec3* currentPtr = *pointsStart;
		std::cout << currentPtr->x << ", " << currentPtr->y << ", " << currentPtr->z << std::endl;
	}
}

glm::vec3 QuatRotationPoints::getFirstPoint()
{
	auto pointsStart = pointsRef.begin();
	return **pointsStart;
}

glm::vec3 QuatRotationPoints::getSecondPoint()
{
	auto pointsStart = pointsRef.begin();
	pointsStart++;
	return **pointsStart;
}

glm::vec3* QuatRotationPoints::getFirstPointRef()
{
	auto pointsStart = pointsRef.begin();
	return *pointsStart;
}

glm::vec3* QuatRotationPoints::getSecondPointRef()
{
	auto pointsStart = pointsRef.begin();
	pointsStart++;
	return *pointsStart;
}

glm::vec3* QuatRotationPoints::getThirdPointRef()
{
	auto pointsStart = pointsRef.begin();
	pointsStart++;
	pointsStart++;
	return *pointsStart;
}

glm::vec3* QuatRotationPoints::getMRPRef()
{
	auto pointsStart = pointsRef.begin();
	pointsStart++;
	pointsStart++;
	pointsStart++;
	return *pointsStart;
}

glm::vec3* QuatRotationPoints::getNormalRef()
{
	auto pointsStart = pointsRef.begin();
	pointsStart++;
	pointsStart++;
	pointsStart++;
	pointsStart++;
	return *pointsStart;
}