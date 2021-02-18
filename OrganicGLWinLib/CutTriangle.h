#pragma once

#ifndef CUTTRIANGLE_H
#define CUTTRIANGLE_H

#include "CutLine.h"
#include "OrganicGLWinUtils.h"
#include <glm/glm.hpp>
#include "QuatRotationPoints.h"
#include "QuatRotationManager.h"
#include "QuatUtils.h"

class CutTriangle
{
	public:
		CutTriangle() {};
	private:
		friend class CuttableTriangle;
		friend class CutTriangleProducer;
		friend class CutTriangleGroupBuilder;
		friend class CutLineTraceObserver;
		friend class STriangle;

		//CutTriangle() {};
		CutTriangle(CutLine in_lineOfSight, CutLine in_rearHook)
		{
			lines[0] = in_lineOfSight;
			lines[1] = in_rearHook;

			buildFinalLine();
			findCentroid();
			determineFinalLineNormal();
		};

		CutTriangle(CutLine in_line0, CutLine in_line1, CutLine in_line2)
		{
			lines[0] = in_line0;
			lines[1] = in_line1;
			lines[2] = in_line2;
			findCentroid();
		}

		void shiftLines()
		{
			CutLine line2Copy = lines[2];
			lines[2] = lines[1];
			lines[1] = lines[0];
			lines[0] = line2Copy;
		}

		bool checkIfPointIsWithinTriangle(glm::vec3 in_point)
		{
			return QuatUtils::checkIfPointLiesWithinTrianglePBZ(in_point, lines[0].pointA, lines[1].pointA, lines[2].pointA);
		}

		CutLine fetchTriangleLine(int in_lineID)
		{
			return lines[in_lineID];
		}

		CutLine* fetchTriangleLineRef(int in_lineID)
		{
			return &lines[in_lineID];
		}

		void printPoints()
		{
			std::cout << "#### Printing points of this CutTriangle: " << std::endl;
			for (int x = 0; x < 3; x++)
			{
				std::cout << "point " << x << ": " << lines[x].pointA.x << ",  " << lines[x].pointA.y << ",  " << lines[x].pointA.z << std::endl;
			}
		}


		void buildFinalLine()
		{
			CutLine finalWeldedLine;
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
			rotationPoints.insertPointRefs(&pointACopy, &pointBCopy, &centroidPointCopy);

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

		CutLine lines[3];
		glm::vec3 centroid;
};

#endif
