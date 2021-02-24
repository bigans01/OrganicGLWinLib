#include "stdafx.h"
#include "CuttingTriangleManager.h"

void CuttingTriangleManager::insertCuttingTriangle(STriangle in_sTriangle)
{
	int currentIndex = int(cuttingTriangles.size());
	CuttingTriangle newCuttingTriangle(in_sTriangle);
	cuttingTriangles[currentIndex] = newCuttingTriangle;
}

void CuttingTriangleManager::printCuttingTriangles()
{
	auto cuttingTrianglesBegin = cuttingTriangles.begin();
	auto cuttingTrianglesEnd = cuttingTriangles.end();
	for (; cuttingTrianglesBegin != cuttingTrianglesEnd; cuttingTrianglesBegin++)
	{
		cuttingTrianglesBegin->second.printPoints();
	}
}