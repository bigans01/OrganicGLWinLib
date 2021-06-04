#include "stdafx.h"
#include "RPoly.h"

void RPoly::addRTriangle(RTriangle in_rTriangleToAdd)
{
	rTriangleMap[numberOfRTriangles++] = in_rTriangleToAdd;
}

void RPoly::printRPoints()
{
	for (int x = 0; x < numberOfRBorderLines; x++)
	{
		std::cout << "RPoint " << x << ": " << rBorderLines[x].pointA.x << ", " << rBorderLines[x].pointA.y << ", " << rBorderLines[x].pointA.z << std::endl;
	}
}