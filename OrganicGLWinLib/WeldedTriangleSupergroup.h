#pragma once

#ifndef WELDEDTRIANGLESUPERGROUP_H
#define WELDEDTRIANGLESUPERGROUP_H

#include <map>
#include "WeldedTriangleContainer.h"
#include "QuatRotationPoints.h"

class WeldedTriangleSupergroup
{
	public:
		std::map<int, WeldedTriangleContainer> triangleContainerMap;
		void setOriginalSPolyID(int in_originalSPolyID)
		{
			originalSPolyID = in_originalSPolyID;
		}

		void insertTriangleContainer(WeldedTriangleContainer in_weldedTriangleContainer)
		{
			triangleContainerMap[triangleContainerMap.size()] = std::move(in_weldedTriangleContainer);
		};

		void printPointsInSupergroup()
		{
				auto triangleContainerMapBegin = triangleContainerMap.begin();
			auto triangleContainerMapEnd = triangleContainerMap.end();
			for (; triangleContainerMapBegin != triangleContainerMapEnd; triangleContainerMapBegin++)
			{
				std::cout << ">>>> WeldedTriangleContainer: " << triangleContainerMapBegin->first << std::endl;
				auto trianglesBegin = triangleContainerMapBegin->second.triangleMap.begin();
				auto trianglesEnd = triangleContainerMapBegin->second.triangleMap.end();
				for (; trianglesBegin != trianglesEnd; trianglesBegin++)
				{
					std::cout << ">>>> WeldedTriangle: " << trianglesBegin->first << std::endl;

					//glm::vec3 point0 = trianglesBegin->second.fetchTriangleLineRef(0)->pointA;
					//glm::vec3 point1 = trianglesBegin->second.fetchTriangleLineRef(1)->pointA;
					//glm::vec3 point2 = trianglesBegin->second.fetchTriangleLineRef(2)->pointA;

					for (int x = 0; x < 3; x++)
					{
						glm::vec3 currentPoint = trianglesBegin->second.fetchTriangleLineRef(x)->pointA;
						std::cout << "Current point: " << currentPoint.x << ",  " << currentPoint.y << ", " << currentPoint.z << std::endl;
					}

					//trianglesBegin->second.fetchTriangleLineRef(0)->pointA;
					//trianglesBegin->second.fetchTriangleLineRef(1)->pointA;
					//trianglesBegin->second.fetchTriangleLineRef(2)->pointA;
				}
			}
		};

		void loadTrianglesIntoQuatRotationPoints(QuatRotationPoints* in_quatRotationPointsRef)
		{
			//quatPoints.pointsRefVector.push_back(&(*weldedTrianglesBegin).fetchTriangleLineRef(0)->pointA);
			auto triangleContainerMapBegin = triangleContainerMap.begin();
			auto triangleContainerMapEnd = triangleContainerMap.end();
			for (; triangleContainerMapBegin != triangleContainerMapEnd; triangleContainerMapBegin++)
			{
				auto trianglesBegin = triangleContainerMapBegin->second.triangleMap.begin();
				auto trianglesEnd = triangleContainerMapBegin->second.triangleMap.end();
				for (; trianglesBegin != trianglesEnd; trianglesBegin++)
				{
					in_quatRotationPointsRef->pointsRefVector.push_back(&trianglesBegin->second.fetchTriangleLineRef(0)->pointA);
					in_quatRotationPointsRef->pointsRefVector.push_back(&trianglesBegin->second.fetchTriangleLineRef(1)->pointA);
					in_quatRotationPointsRef->pointsRefVector.push_back(&trianglesBegin->second.fetchTriangleLineRef(2)->pointA);
				}
			}
			//in_quatRotationPointsRef->points
		};

		int originalSPolyID = 0;

};

#endif