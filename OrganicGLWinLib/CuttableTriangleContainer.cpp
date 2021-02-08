#include "stdafx.h"
#include "CuttableTriangleContainer.h"

void CuttableTriangleContainer::buildFirstCuttableTriangle(STriangle in_firstTriangle)
{
	CuttableTriangle primeTriangle(in_firstTriangle);
	cuttableTriangleMap[0] = primeTriangle;
}

void CuttableTriangleContainer::insertNewCuttableTriangle(STriangle in_sTriangle)
{
	int currentMapSize = int(cuttableTriangleMap.size());
	CuttableTriangle newTriangle(in_sTriangle);
	cuttableTriangleMap[currentMapSize] = newTriangle;
}

void CuttableTriangleContainer::rebuildCuttableTriangleMapFromContainer(STriangleOutputContainer* in_sTriangleOutputContainerRef)
{
	cuttableTriangleMap.clear();	// clear out the map.
	auto outputContainerBegin = in_sTriangleOutputContainerRef->sTriangleOutputVector.begin();
	auto outputContainerEnd = in_sTriangleOutputContainerRef->sTriangleOutputVector.end();
	int currentIndex = 0;
	for (; outputContainerBegin != outputContainerEnd; outputContainerBegin++)
	{
		CuttableTriangle newTriangle(*outputContainerBegin);
		cuttableTriangleMap[currentIndex++] = newTriangle;
	}
}