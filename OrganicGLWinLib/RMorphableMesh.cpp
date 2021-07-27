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

void RMorphableMesh::updatePointState(RMorphableMeshState in_pointState)
{
	pointState = in_pointState;
}

void RMorphableMesh::generateRProductFaces()
{
	// check north face.
	bool isNorthUsable = meshCorners.checkIfFaceIsUsable(ECBPPOrientations::CORNER_UPPERNW,
													     ECBPPOrientations::CORNER_UPPERNE,
														 ECBPPOrientations::CORNER_LOWERNE,
														 ECBPPOrientations::CORNER_LOWERNW);
	if (isNorthUsable == true)
	{
		RProductFaceRootPoints northRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_UPPERNW,
																			ECBPPOrientations::CORNER_UPPERNE,
																			ECBPPOrientations::CORNER_LOWERNE,
																			ECBPPOrientations::CORNER_LOWERNW);
		RProductFace northProductFace(northRootPoints);
		rProductFaceMap[ECBPPOrientations::NORTHFACE] = northProductFace;
	}

	// check east face.
	bool isEastUsable = meshCorners.checkIfFaceIsUsable(ECBPPOrientations::CORNER_UPPERNE,
														ECBPPOrientations::CORNER_UPPERSE,
														ECBPPOrientations::CORNER_LOWERSE,
														ECBPPOrientations::CORNER_LOWERNE);
	if (isEastUsable == true)
	{
		RProductFaceRootPoints eastRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_UPPERNE,
																			ECBPPOrientations::CORNER_UPPERSE,
																			ECBPPOrientations::CORNER_LOWERSE,
																			ECBPPOrientations::CORNER_LOWERNE);
		RProductFace eastProductFace(eastRootPoints);
		rProductFaceMap[ECBPPOrientations::EASTFACE] = eastProductFace;
	}

	// check south face
	bool isSouthUsable = meshCorners.checkIfFaceIsUsable(ECBPPOrientations::CORNER_UPPERSE,
														ECBPPOrientations::CORNER_UPPERSW,
														ECBPPOrientations::CORNER_LOWERSW,
														ECBPPOrientations::CORNER_LOWERSE);
	if (isSouthUsable == true)
	{
		RProductFaceRootPoints southRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_UPPERSE,
																			ECBPPOrientations::CORNER_UPPERSW,
																			ECBPPOrientations::CORNER_LOWERSW,
																			ECBPPOrientations::CORNER_LOWERSE);
		RProductFace southProductFace(southRootPoints);
		rProductFaceMap[ECBPPOrientations::SOUTHFACE] = southProductFace;
	}
}

void RMorphableMesh::runSuctionByXSlice()
{
	RMorphableMeshCornerArray fetchedArray = meshCorners.getPointArray();
	for (int x = 0; x < 8; x++)
	{
		RCollisionPoint* currentPointRef = fetchedArray.pointRefArray[x];
		glm::vec3 currentRefedPointOriginalValue = currentPointRef->originalValue;
		EnclaveKeyDef::EnclaveKey translatedPoint = translatorRef->translatePointToGrid(currentRefedPointOriginalValue);
		MassGridSearchResult searchResult = gridRef->fetchCell(translatedPoint);
		std::string massValue = "EMPTY";
		std::string wasFound = "No";
		std::string wasLandlocked = "No";
		int totalFlagsFound = 0;
		if (currentPointRef->getCollisionPointState() == RCollisionPointState::IMMUTABLE)
		{
			wasLandlocked = "Yes";
		}
		if (searchResult.wasSearchKeyValid == true)
		{
			wasFound = "Yes";
			totalFlagsFound = searchResult.cellRef->getNumberOfFlagsSet();
			if 
			(
				(searchResult.cellRef->isFlagSet(MassCellBitFlags::INNER_MASS))
				||
				(searchResult.cellRef->isFlagSet(MassCellBitFlags::CRUST_MASS))
				||
				(searchResult.cellRef->isFlagSet(MassCellBitFlags::LINE_MASS))
			)
			{
				massValue = "HAS_MASS";
			}

		}
		
		std::cout << ":: Point at array index " << x << "( " << currentRefedPointOriginalValue.x << "," 
															 << currentRefedPointOriginalValue.y << ", " << 
																currentRefedPointOriginalValue.z << ") translates to array value: " << translatedPoint.x << ", " << translatedPoint.y << ", " << translatedPoint.z 
														    << "), wasFound = " << wasFound 
													        << ", and has a " 
															<< massValue << " mass; totalFlagsFound = " << totalFlagsFound << "; landlocked = " << wasLandlocked << std::endl;
	}
}