#pragma once

#ifndef RTRIANGLE_H
#define RTRIANGLE_H

#include "RTriangleLine.h"

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
	private:
		RTriangleLine rLines[3];
};

#endif
