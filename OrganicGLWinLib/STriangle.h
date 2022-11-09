#pragma once

#ifndef STRIANGLE_H
#define STRIANGLE_H

#include "STriangleLine.h"

class STriangle
{
public:
	STriangle(glm::vec3 in_pointA, glm::vec3 in_pointB, glm::vec3 in_pointC);
	STriangle();

	void setSTriangleMaterialID(short in_sTriangleMaterialID)
	{

	}

	void printPoints()
	{
		std::cout << "!! Printing points of STriangle. " << std::endl;
		for (int x = 0; x < 3; x++)
		{
			std::cout << "point " << x << ": " << triangleLines[x].pointA.x << ", " << triangleLines[x].pointA.y << ", " << triangleLines[x].pointA.z << std::endl;
		}
	}


	struct PointCounter
	{
		PointCounter() {};
		PointCounter(glm::vec3 in_vec3) :
			point(in_vec3)
		{};
		glm::vec3 point;
		int pointCount = 0;
		bool equals(glm::vec3 in_pointToCheck)
		{
			bool isEqual = false;
			if (in_pointToCheck == point)
			{
				isEqual = true;
			}
			return isEqual;
		}
	};

	int getPerfectClampingValue()
	{
		int perfectClampValue = 0;
		// x clamp check
		if
		(
			(triangleLines[0].pointA.x == triangleLines[1].pointA.x)
			&&
			(triangleLines[1].pointA.x == triangleLines[2].pointA.x)
		)
		{
			//std::cout << "Perfect X-clamp detected! " << std::endl;
			perfectClampValue = 1;			// 1 == clamped to X
		}

		// y clamp check
		if
		(
			(triangleLines[0].pointA.y == triangleLines[1].pointA.y)
			&&
			(triangleLines[1].pointA.y == triangleLines[2].pointA.y)
		)
		{
			//std::cout << "Perfect Y-clamp detected! " << std::endl;
			perfectClampValue = 2;			// 2 == clamped to X
		}

		// z clamp check
		if
		(
			(triangleLines[0].pointA.z == triangleLines[1].pointA.z)
			&&
			(triangleLines[1].pointA.z == triangleLines[2].pointA.z)
		)
		{
			//std::cout << "Perfect Z-clamp detected! " << std::endl;
			perfectClampValue = 3;			// 3 == clamped to X
		}
		return perfectClampValue;
	}

	bool isSTriangleValid()	// an STriangle may only be valid if all 3 points of it are unique.
	{
		bool validityMaintained = true;
		std::map<int, PointCounter> tempPointCounter;
		for (int x = 0; x < 3; x++)
		{
			insertIntoPointCounterMap(&tempPointCounter, triangleLines[x].pointA);
		}

		// if the size of the tempPointCounter map isn't 3, there aren't 3 unique points.
		if (tempPointCounter.size() != 3)
		{
			validityMaintained = false;
		}
		return validityMaintained;
	};

	void insertIntoPointCounterMap(std::map<int, STriangle::PointCounter>* in_tempPointCounterRef, glm::vec3 pointToInsert)
	{
		if (in_tempPointCounterRef->size() == 0)
		{
			PointCounter newCounter(pointToInsert);
			(*in_tempPointCounterRef)[0] = newCounter;	// insert
			(*in_tempPointCounterRef)[0].pointCount++;	// and increment
		}
		else if (in_tempPointCounterRef->size() != 0)
		{
			bool wasExistingPointFound = false;
			auto pointCounterMapRefBegin = in_tempPointCounterRef->begin();
			auto pointCounterMapRefEnd = in_tempPointCounterRef->end();
			for (; pointCounterMapRefBegin != pointCounterMapRefEnd; pointCounterMapRefBegin++)
			{
				if (pointCounterMapRefBegin->second.point == pointToInsert)
				{
					wasExistingPointFound = true;					// if found, flag it
					pointCounterMapRefBegin->second.pointCount++;	// and increment it again
				}
			}

			if (wasExistingPointFound == false)	// if we didn't find the point at all, insert a new one.
			{
				int refedMapSize = int(in_tempPointCounterRef->size());
				PointCounter newCounter(pointToInsert);
				(*in_tempPointCounterRef)[refedMapSize] = pointToInsert;
				(*in_tempPointCounterRef)[refedMapSize].pointCount++;
			}
		}
	}

	STriangleLine triangleLines[3];	// the lines of the STriangle
	short sTriangleMaterialID = 0;	// the materialID that would be inherited from the parent SPoly (needed for use by "R" family of classes)
};

#endif
