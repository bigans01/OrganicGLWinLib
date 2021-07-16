#include "stdafx.h"
#include "RMorphableMesh.h"

RMorphableMeshCubeAreaDefiner RMorphableMesh::getAreaDefiner()
{
	return morphAreaDefinition;
}

void RMorphableMesh::setMeshCorners(RMorphableMeshCorners in_meshCorners)
{
	meshCorners = in_meshCorners;
}

void RMorphableMesh::printCornerPoints()
{
	meshCorners.printCornerData();
}

void RMorphableMesh::updatePointUsageCounts()
{
	meshCorners.updateUsageCounts();
}