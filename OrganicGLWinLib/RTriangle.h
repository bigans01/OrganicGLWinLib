#pragma once

#ifndef RTRIANGLE_H
#define RTRIANGLE_H

#include "RTriangleLine.h"
#include "MassGridArray.h"
#include "LookupByDimRegister.h"
#include <mutex>

class RTriangle
{
	public:
		RTriangle();
		RTriangle(RTriangleLine in_line0, RTriangleLine in_line1, RTriangleLine in_line2);
		RTriangle(const RTriangle& in_triangleB);
		RTriangle& operator=(const RTriangle& in_triangleB)
		{
			for (int x = 0; x < 3; x++)
			{
				rLines[x] = in_triangleB.rLines[x];
			}

			areRegistersSet = in_triangleB.areRegistersSet;
			if (areRegistersSet == true)
			{
				xDimRegisterSize = in_triangleB.xDimRegisterSize;
				yDimRegisterSize = in_triangleB.yDimRegisterSize;
				zDimRegisterSize = in_triangleB.zDimRegisterSize;

				// copy data for the X dim
				xDimRegister.reset(new LookupByDimRegister[xDimRegisterSize]);
				for (int x = 0; x < xDimRegisterSize; x++)
				{
					xDimRegister[x] = in_triangleB.xDimRegister[x];
				}

				// "" Y dim 
				yDimRegister.reset(new LookupByDimRegister[yDimRegisterSize]);
				for (int x = 0; x < yDimRegisterSize; x++)
				{
					yDimRegister[x] = in_triangleB.yDimRegister[x];
				}

				// "" Z dim
				zDimRegister.reset(new LookupByDimRegister[zDimRegisterSize]);
				for (int x = 0; x < zDimRegisterSize; x++)
				{
					zDimRegister[x] = in_triangleB.zDimRegister[x];
				}
			}
			return *this;
		}

		void printRPoints();
		void traceRasterLines();
		void traceRasterLinesIntoGrid(MassGridArray* in_massGridArrayRef, glm::vec3 in_triangleEmptyNormal);
	private:
		RTriangleLine rLines[3];
		std::unique_ptr<LookupByDimRegister[]> xDimRegister;
		std::unique_ptr<LookupByDimRegister[]> yDimRegister;
		std::unique_ptr<LookupByDimRegister[]> zDimRegister;
		int xDimRegisterSize, yDimRegisterSize, zDimRegisterSize = 0;
		bool areRegistersSet = false;

		void buildRegisters();
};

#endif
