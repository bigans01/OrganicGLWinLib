#pragma once

#ifndef RTRIANGLE_H
#define RTRIANGLE_H

#include "RTriangleLine.h"
#include "MassGridArray.h"

class RTriangle
{
	public:
		RTriangle() {};
		RTriangle(RTriangleLine in_line0, RTriangleLine in_line1, RTriangleLine in_line2)
		{
			rLines[0] = in_line0;
			rLines[1] = in_line1;
			rLines[2] = in_line2;
		};
		void printRPoints()
		{
			std::cout << "!! Printing points of RTriangle. " << std::endl;
			for (int x = 0; x < 3; x++)
			{
				std::cout << "point " << x << ": " << rLines[x].rLinePointA.x << ", " << rLines[x].rLinePointA.y << ", " << rLines[x].rLinePointA.z << std::endl;
			}
		}
		void traceRasterLines()
		{
			for (int x = 0; x < 3; x++)
			{
				rLines[x].runRasterTrace();
			}
		}

		void traceRasterLinesIntoGrid(MassGridArray* in_massGridArrayRef, glm::vec3 in_triangleEmptyNormal)
		{
			// step 1: determine the value of the DOWNFILL_CRUST bit
			int downFillCrustBit = 0;
			if (in_triangleEmptyNormal.y > 0.0f)
			{
				downFillCrustBit = 1;
			}


			// step 2: trace the lines of the triangle into the grid.
			for (int x = 0; x < 3; x++)
			{
				rLines[x].runRasterTraceIntoGrid(in_massGridArrayRef, downFillCrustBit);
			}

			// step 3: use the rasterizedBlocks from each line, to create line pairs that are used to trace the interor of the RTriangle
			// into the MassGridArray. Each line pair should do X, Y, and Z scans.


		}
	private:
		RTriangleLine rLines[3];
};

#endif
