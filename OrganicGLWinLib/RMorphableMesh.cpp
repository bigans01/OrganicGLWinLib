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

void RMorphableMesh::runSuctionByXSlice()
{
	RMorphableMeshCornerArray fetchedArray = meshCorners.getPointArray();
	for (int x = 0; x < 8; x++)
	{
		RCollisionPoint* currentPointRef = fetchedArray.pointRefArray[x];
		glm::vec3 currentRefedPointOriginalValue = currentPointRef->originalValue;
		EnclaveKeyDef::EnclaveKey translatedPoint = translatorRef->translatePointToGrid(currentRefedPointOriginalValue);
		
		std::cout << ":: Point at array index " << x << "( " << currentRefedPointOriginalValue.x << "," 
															 << currentRefedPointOriginalValue.y << ", " << 
																currentRefedPointOriginalValue.z << ") translates to array value: " << translatedPoint.x << ", " << translatedPoint.y << ", " << translatedPoint.z << std::endl;
	}
}