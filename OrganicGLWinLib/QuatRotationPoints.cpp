#include "stdafx.h"
#include "OrganicGLWinUtils.h"
#include "QuatRotationPoints.h"

/*
std::vector<glm::vec3*>::iterator QuatRotationPoints::getPointsVectorBegin()
{
	return pointsRefVector.begin();
}
*/

std::vector<glm::vec3*>::iterator QuatRotationPoints::getPointsVectorBegin()
{
	return pointsRefVector.begin();
}

std::vector<glm::vec3*>::iterator QuatRotationPoints::getPointsVectorEnd()
{
	return pointsRefVector.end();
}

void QuatRotationPoints::applyQuaternion(glm::quat in_quat)
{
	auto pointsStart = pointsRefVector.begin();
	auto pointsEnd = pointsRefVector.end();
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
	auto pointsStart = pointsRefVector.begin();
	auto pointsEnd = pointsRefVector.end();
	for (pointsStart; pointsStart != pointsEnd; pointsStart++)
	{
		auto pointsPtr = *pointsStart;

		//std::cout << "--------> Point, pre-translation: " << pointsPtr->x << ", " << pointsPtr->y << ", " << pointsPtr->z << std::endl;
		*pointsPtr += in_translation;
		//std::cout << "--------> Point, post-translation: " << pointsPtr->x << ", " << pointsPtr->y << ", " << pointsPtr->z << std::endl;
	}
}

void QuatRotationPoints::applyTranslationToIndexRange(glm::vec3 in_translation, int in_indexStart, int in_indexEnd)
{
	auto pointsStart = pointsRefVector.begin() + in_indexStart;
	int numberOfIterations = (in_indexEnd - in_indexStart) + 1; // number of iterations = to the difference between the index range, +1 (to include the beginning of the range)
	for (int x = 0; x < numberOfIterations; x++)
	{
		auto pointsPtr = *pointsStart;
		*pointsPtr += in_translation;
		pointsStart++;
	}
}

void QuatRotationPoints::printTrianglePoints()
{
	
	auto pointsStart = pointsRefVector.begin();
	auto pointsEnd = pointsRefVector.end();
	for (pointsStart; pointsStart != pointsEnd; pointsStart++)
	{
		auto pointsPtr = *pointsStart;
		std::cout << "##-> Point: " << pointsPtr->x << ", " << pointsPtr->y << ", " << pointsPtr->z << std::endl;
	}
	
}

void QuatRotationPoints::applyNormalization(float in_normalizationValue)
{
	auto pointsStart = pointsRefVector.begin();
	auto pointsEnd = pointsRefVector.end();
	for (pointsStart; pointsStart != pointsEnd; pointsStart++)
	{
		auto pointsPtr = *pointsStart;
		pointsPtr->x = (pointsPtr->x) / in_normalizationValue;
		pointsPtr->y = (pointsPtr->y) / in_normalizationValue;
	}
}

void QuatRotationPoints::applyDimensionCorrections()
{
	auto pointsStart = pointsRefVector.begin();
	auto pointsEnd = pointsRefVector.end();
	for (pointsStart; pointsStart != pointsEnd; pointsStart++)
	{
		auto pointsPtr = *pointsStart;
		// check the x value
		if (pointsPtr->x > 1.0f)
		{
			pointsPtr->x = 1.0f;
		}
		else if (pointsPtr->x < 0.0f)
		{
			pointsPtr->x = 0.0f;
		}

		// check the y value
		if (pointsPtr->y > 1.0f)
		{
			pointsPtr->y = 1.0f;
		}
		else if (pointsPtr->y < 0.0f)
		{
			pointsPtr->y = 0.0f;
		}

	}
}

void QuatRotationPoints::printPoints()
{
	std::vector<glm::vec3*>::iterator pointsStart = pointsRefVector.begin();
	std::vector<glm::vec3*>::iterator pointsEnd = pointsRefVector.end();
	for (pointsStart; pointsStart != pointsEnd; pointsStart++)
	{
		glm::vec3 lol;
		glm::vec3* lolPtr = &lol;
		glm::vec3* currentPtr = *pointsStart;
		std::cout << currentPtr->x << ", " << currentPtr->y << ", " << currentPtr->z << std::endl;
	}
}

void QuatRotationPoints::clearPoints()
{
	pointsRefVector.clear();
}

void QuatRotationPoints::roundAllPointsToHundredths()
{
	auto pointsStart = pointsRefVector.begin();
	auto pointsEnd = pointsRefVector.end();
	for (pointsStart; pointsStart != pointsEnd; pointsStart++)
	{
		**pointsStart = OrganicGLWinUtils::roundVec3ToHundredths(**pointsStart);					// will cause a break, due to namespaace conflict with std::min?
	}
}

int QuatRotationPoints::getSize()
{
	return int(pointsRefVector.size());
}

glm::vec3 QuatRotationPoints::getFirstPoint()
{
	auto pointsStart = pointsRefVector.begin();
	return **pointsStart;
}

glm::vec3 QuatRotationPoints::getLastPoint()
{
	auto pointsStart = pointsRefVector.rbegin();
	return **pointsStart;
}

glm::vec3* QuatRotationPoints::getLastPointRef()
{
	auto pointsStart = pointsRefVector.rbegin();
	return *pointsStart;
}

glm::vec3 QuatRotationPoints::getSecondPoint()
{
	auto pointsStart = pointsRefVector.begin();
	pointsStart++;
	return **pointsStart;
}

glm::vec3* QuatRotationPoints::getFirstPointRef()
{
	auto pointsStart = pointsRefVector.begin();
	return *pointsStart;
}

glm::vec3* QuatRotationPoints::getSecondPointRef()
{
	auto pointsStart = pointsRefVector.begin();
	pointsStart++;
	return *pointsStart;
}

glm::vec3* QuatRotationPoints::getThirdPointRef()
{
	auto pointsStart = pointsRefVector.begin();
	pointsStart++;
	pointsStart++;
	return *pointsStart;
}

glm::vec3* QuatRotationPoints::getMRPRef()
{
	auto pointsStart = pointsRefVector.begin();
	pointsStart++;
	pointsStart++;
	pointsStart++;
	return *pointsStart;
}

glm::vec3* QuatRotationPoints::getNormalRef()
{
	auto pointsStart = pointsRefVector.begin();
	pointsStart++;
	pointsStart++;
	pointsStart++;
	pointsStart++;
	return *pointsStart;
}

glm::vec3 QuatRotationPoints::getPointByIndex(int in_index)
{
	auto pointsStart = pointsRefVector.begin();
	if (in_index == 0)	// get the first point
	{
		return **pointsStart;
	}
	else if (in_index != 0)
	{
		for (int x = 0; x < in_index; x++)	// iterate a number of times equal to in_index, if it isn't 0
		{
			pointsStart++;	// iterate
		}
		return **pointsStart;	// return whatever returns after in_index number of iterations
	}
}

glm::vec3* QuatRotationPoints::getPointRefByIndex(int in_index)
{
	auto pointsStart = pointsRefVector.begin();
	if (in_index == 0)	// get the first point
	{
		return *pointsStart;
	}
	else if (in_index != 0)
	{
		for (int x = 0; x < in_index; x++)	// iterate a number of times equal to in_index, if it isn't 0
		{
			pointsStart++;	// iterate
		}
		return *pointsStart;	// return whatever returns after in_index number of iterations
	}
}

void QuatRotationPoints::eraseLastElement()
{
	pointsRefVector.pop_back();
}