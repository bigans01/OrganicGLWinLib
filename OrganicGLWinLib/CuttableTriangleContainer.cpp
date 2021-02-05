#include "stdafx.h"
#include "CuttableTriangleContainer.h"

void CuttableTriangleContainer::buildFirstCuttableTriangle(STriangle in_firstTriangle)
{
	cuttableTriangleMap[0].buildCuttableTriangle(in_firstTriangle);
}

void CuttableTriangleContainer::insertNewCuttableTriangle(STriangle in_sTriangle)
{
	int currentMapSize = cuttableTriangleMap.size();
	cuttableTriangleMap[currentMapSize].buildCuttableTriangle(in_sTriangle);
}

void CuttableTriangleContainer::rebuildCuttableTriangleMapFromContainer(STriangleOutputContainer* in_sTriangleOutputContainerRef)
{
	cuttableTriangleMap.clear();	// clear out the map.
	auto outputContainerBegin = in_sTriangleOutputContainerRef->sTriangleOutputVector.begin();
	auto outputContainerEnd = in_sTriangleOutputContainerRef->sTriangleOutputVector.end();
	int currentIndex = 0;
	for (; outputContainerBegin != outputContainerEnd; outputContainerBegin++)
	{
		cuttableTriangleMap[currentIndex++].buildCuttableTriangle(*outputContainerBegin);
	}
}