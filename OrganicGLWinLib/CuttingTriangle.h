#pragma once

#ifndef CUTTINGTRIANGLE_H
#define CUTTINGTRIANGLE_H

#include "STriangle.h"
#include "CuttingTriangleLine.h"

class CuttingTriangle
{
	public:
		CuttingTriangle() {};
		CuttingTriangle(STriangle in_sTriangle)
		{
			for (int x = 0; x < 3; x++)
			{
				CuttingTriangleLine newLine(in_sTriangle.triangleLines[x]);
				cuttingLines[x] = newLine;
			}
		};
	private:
		friend class STriangleCutter;
		friend class CuttableTriangle;
		friend class CuttingTriangleManager;
		
		CuttingTriangleLine cuttingLines[3];
		void reset();
};

#endif
