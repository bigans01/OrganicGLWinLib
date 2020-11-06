#pragma once

#ifndef WELDEDTRIANGLE_H
#define WELDEDTRIANGLE_H

#include "WeldedLine.h"
#include "OrganicGLWinUtils.h"
#include <glm/glm.hpp>
#include "QuatRotationPoints.h"
#include "QuatRotationManager.h"

class WeldedTriangle
{
	public:
		WeldedTriangle() {};
		WeldedTriangle(WeldedLine in_lineOfSight, WeldedLine in_rearHook) 
		{
			lines[0] = in_lineOfSight;
			lines[1] = in_rearHook;

			buildFinalLine();
			findCentroid();
			determineFinalLineNormal();

			std::cout << ":::: WeldedTriangle lines are: " << std::endl;
			for (int x = 0; x < 3; x++)
			{
				std::cout << "[" << x << "]: point A: " << lines[x].pointA.x << ", " << lines[x].pointA.y << ", " << lines[x].pointA.z << " | point B: " << lines[x].pointB.x << ", " << lines[x].pointB.y << ", " << lines[x].pointB.z << std::endl;
			}
		}

		bool checkIfPointIsWithinTriangle(glm::vec3 in_point)
		{
			bool isWithinTriangle = false;
			bool planeArrayCheckResult[3];
			for (int x = 0; x < 3; x++)
			{
				glm::vec3 pointACopy = lines[x].pointA;
				glm::vec3 pointBCopy = lines[x].pointB;
				glm::vec3 pointToCheck = in_point;
				glm::vec3 centroidFacingNormal = lines[x].emptyNormal;

				// first, check for point translation; only translate pointA/B, and the point to check against.
				PointTranslationCheck pointCheck;
				pointCheck.performCheck(pointACopy);
				if (pointCheck.requiresTranslation == 1)
				{
					glm::vec3 translationValue = pointCheck.getTranslationValue();
					pointACopy += translationValue;
					pointBCopy += translationValue;
					pointToCheck += translationValue;
				}

				QuatRotationPoints rotationPoints;
				rotationPoints.pointsRefVector.push_back(&pointACopy);
				rotationPoints.pointsRefVector.push_back(&pointBCopy);
				rotationPoints.pointsRefVector.push_back(&pointToCheck);
				rotationPoints.pointsRefVector.push_back(&centroidFacingNormal);		// fetched by rotationManager, to check for flipping on Z-axis

				QuatRotationManager rotationManager;
				planeArrayCheckResult[x] = rotationManager.initializeAndRunForCheckingIfPointIswithinPlane(&rotationPoints);

			}

			int withinPlaneCount = 0;
			for (int x = 0; x < 3; x++)
			{
				if (planeArrayCheckResult[x] == true)
				{
					withinPlaneCount++;
				}
			}

			if (withinPlaneCount == 3)
			{
				std::cout << "Point is WITHIN triangle. " << std::endl;
				isWithinTriangle = true;
			}

			return isWithinTriangle;
		}
	private:
		WeldedLine lines[3];
		glm::vec3 centroid;

		void buildFinalLine()
		{
			WeldedLine finalWeldedLine;
			finalWeldedLine.pointA = lines[1].pointB;
			finalWeldedLine.pointB = lines[0].pointA;
			lines[2] = finalWeldedLine;
		}

		void findCentroid()
		{
			centroid = OrganicGLWinUtils::findTriangleCentroid(lines[0].pointA, lines[1].pointA, lines[2].pointA);
		}

		void determineFinalLineNormal()
		{
			glm::vec3 pointACopy = lines[2].pointA;
			glm::vec3 pointBCopy = lines[2].pointB;
			glm::vec3 centroidPointCopy = centroid;

			// first, check for point translation
			PointTranslationCheck pointCheck;
			pointCheck.performCheck(pointACopy);
			if (pointCheck.requiresTranslation == 1)
			{
				glm::vec3 translationValue = pointCheck.getTranslationValue();
				pointACopy += translationValue;
				pointBCopy += translationValue;
				centroidPointCopy += translationValue;
			}

			QuatRotationPoints rotationPoints;
			rotationPoints.pointsRefVector.push_back(&pointACopy);
			rotationPoints.pointsRefVector.push_back(&pointBCopy);
			rotationPoints.pointsRefVector.push_back(&centroidPointCopy);

			QuatRotationManager rotationManager;
			rotationManager.initializeAndRunForFindingBorderLineEmptyNormal(&rotationPoints);

			lines[2].emptyNormal = centroidPointCopy;

			//std::cout << ":::: Final line calculated empty normal is: " << lines[2].emptyNormal.x << ", " << lines[2].emptyNormal.y << ", " << lines[2].emptyNormal.z << std::endl;
			
			/*
			std::cout << "::::::::::::: Line stats" << std::endl;
			for (int x = 0; x < 3; x++)
			{
				std::cout << "[" << x << "]: point A: " << lines[x].pointA.x << ", " << lines[x].pointA.y << ", " << lines[x].pointA.z <<
					" | point B: " << lines[x].pointB.x << ", " << lines[x].pointB.y << ", " << lines[x].pointB.z <<
					" | centroid-facing normal: " << lines[x].emptyNormal.x << ", " << lines[x].emptyNormal.y << ", " << lines[x].emptyNormal.z << std::endl;
			}
			*/
		}
};

#endif
