#pragma once

#ifndef CUTTRIANGLE_H
#define CUTTRIANGLE_H

#include "CutLine.h"
#include "OrganicGLWinUtils.h"
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

		void printCutTrianglePoints()
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
			lines[2].emptyNormal = QuatUtils::findOrientatedLineNormal(lines[2].pointA, lines[2].pointB, centroid);
		}

		CutLine lines[3];
		glm::vec3 centroid;
};

#endif
