#pragma once

#ifndef WELDEDTRIANGLE_H
#define WELDEDTRIANGLE_H

#include "WeldedLine.h"
#include "OrganicGLWinUtils.h"
#include "QuatUtils.h"

class WeldedTriangle
{
	public:
		WeldedTriangle() {};
	private:
		friend class TracingObserver;
		friend class WeldedTriangleProducer;
		friend class WeldedTriangleSuperGroup;
		friend class WeldedTriangleGroupBuilder;
		friend class SPolyFracturer;

		WeldedTriangle(WeldedLine in_lineOfSight, WeldedLine in_rearHook) 
		{
			lines[0] = in_lineOfSight;
			lines[1] = in_rearHook;

			buildFinalLine();
			findCentroid();
			determineFinalLineNormal();
		}

		WeldedTriangle(WeldedLine in_line0, WeldedLine in_line1, WeldedLine in_line2)
		{
			lines[0] = in_line0;
			lines[1] = in_line1;
			lines[2] = in_line2;
			findCentroid();
		}

		void shiftLines()
		{
			WeldedLine line2Copy = lines[2];
			lines[2] = lines[1];
			lines[1] = lines[0];
			lines[0] = line2Copy;
		}

		bool checkIfPointIsWithinTriangle(glm::vec3 in_point)
		{
			return QuatUtils::checkIfPointLiesWithinTrianglePBZ(in_point, lines[0].pointA, lines[1].pointA, lines[2].pointA);
		}

		WeldedLine fetchTriangleLine(int in_lineID)
		{
			return lines[in_lineID];
		}

		WeldedLine* fetchTriangleLineRef(int in_lineID)
		{
			return &lines[in_lineID];
		}

		void printPoints()
		{
			std::cout << "#### Printing points of this WeldedTriangle: " << std::endl;
			for (int x = 0; x < 3; x++)
			{
				std::cout << "point " << x << ": " << lines[x].pointA.x << ",  " << lines[x].pointA.y << ",  " << lines[x].pointA.z << std::endl;
			}
		}



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
			lines[2].emptyNormal = QuatUtils::findOrientatedLineNormal(lines[2].pointA, lines[2].pointB, centroid);
		}

		WeldedLine lines[3];
		glm::vec3 centroid;
};

#endif
