#include "stdafx.h"
#include "RTriangle.h"

RTriangle::RTriangle() 
{

};

RTriangle::RTriangle(RTriangleLine in_line0, RTriangleLine in_line1, RTriangleLine in_line2)
{
	rLines[0] = in_line0;
	rLines[1] = in_line1;
	rLines[2] = in_line2;

	// once lines are set up, initialize the dim registers (xDimRegister, yDimRegister, zDimRegister)
	buildRegisters();
};

void RTriangle::buildRegisters()
{
	// find the greatest difference in x,y,z

}

RTriangle::RTriangle(const RTriangle& in_triangleB)
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
}

void RTriangle::printRPoints()
{
	std::cout << "!! Printing points of RTriangle. " << std::endl;
	for (int x = 0; x < 3; x++)
	{
		std::cout << "point " << x << ": " << rLines[x].rLinePointA.x << ", " << rLines[x].rLinePointA.y << ", " << rLines[x].rLinePointA.z << std::endl;
	}
}

void RTriangle::traceRasterLines()
{
	for (int x = 0; x < 3; x++)
	{
		rLines[x].runRasterTrace();
	}
}

void RTriangle::traceRasterLinesIntoGrid(MassGridArray* in_massGridArrayRef, glm::vec3 in_triangleEmptyNormal)
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