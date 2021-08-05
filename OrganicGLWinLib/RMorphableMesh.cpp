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

RMorphableMeshState RMorphableMesh::getMeshState()
{
	return pointState;
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
		std::cout << "Generating NORTHFACE..." << std::endl;
		RProductFaceRootPoints northRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_UPPERNW,
																			ECBPPOrientations::CORNER_UPPERNE,
																			ECBPPOrientations::CORNER_LOWERNE,
																			ECBPPOrientations::CORNER_LOWERNW);
		RProductFace northProductFace(northRootPoints);
		glm::vec3 northFaceTargetEmptyNormal(0, 0, -1.0f);
		rProductFaceMap[ECBPPOrientations::NORTHFACE] = northProductFace;
		rProductFaceMap[ECBPPOrientations::NORTHFACE].generateFacePTriangles(northFaceTargetEmptyNormal);
	}

	// check east face.
	bool isEastUsable = meshCorners.checkIfFaceIsUsable(ECBPPOrientations::CORNER_UPPERNE,
														ECBPPOrientations::CORNER_UPPERSE,
														ECBPPOrientations::CORNER_LOWERSE,
														ECBPPOrientations::CORNER_LOWERNE);
	if (isEastUsable == true)
	{
		std::cout << "Generating EASTFACE..." << std::endl;
		RProductFaceRootPoints eastRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_UPPERNE,
																			ECBPPOrientations::CORNER_UPPERSE,
																			ECBPPOrientations::CORNER_LOWERSE,
																			ECBPPOrientations::CORNER_LOWERNE);
		RProductFace eastProductFace(eastRootPoints);
		glm::vec3 eastFaceTargetEmptyNormal(1.0f, 0, 0);
		rProductFaceMap[ECBPPOrientations::EASTFACE] = eastProductFace;
		rProductFaceMap[ECBPPOrientations::EASTFACE].generateFacePTriangles(eastFaceTargetEmptyNormal);
	}

	// check south face
	bool isSouthUsable = meshCorners.checkIfFaceIsUsable(ECBPPOrientations::CORNER_UPPERSE,
														ECBPPOrientations::CORNER_UPPERSW,
														ECBPPOrientations::CORNER_LOWERSW,
														ECBPPOrientations::CORNER_LOWERSE);
	if (isSouthUsable == true)
	{
		std::cout << "Generating SOUTHFACE..." << std::endl;
		RProductFaceRootPoints southRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_UPPERSE,
																			ECBPPOrientations::CORNER_UPPERSW,
																			ECBPPOrientations::CORNER_LOWERSW,
																			ECBPPOrientations::CORNER_LOWERSE);
		RProductFace southProductFace(southRootPoints);
		glm::vec3 southFaceTargetEmptyNormal(0, 0, 1.0f);
		rProductFaceMap[ECBPPOrientations::SOUTHFACE] = southProductFace;
		rProductFaceMap[ECBPPOrientations::SOUTHFACE].generateFacePTriangles(southFaceTargetEmptyNormal);
	}

	// check west face
	bool isWestUsable = meshCorners.checkIfFaceIsUsable(ECBPPOrientations::CORNER_UPPERSW,
														ECBPPOrientations::CORNER_UPPERNW,
														ECBPPOrientations::CORNER_LOWERNW,
														ECBPPOrientations::CORNER_LOWERSW);
	if (isWestUsable == true)
	{
		std::cout << "Generating WESTFACE..." << std::endl;
		RProductFaceRootPoints westRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_UPPERSW,
																			ECBPPOrientations::CORNER_UPPERNW,
																			ECBPPOrientations::CORNER_LOWERNW,
																			ECBPPOrientations::CORNER_LOWERSW);
		RProductFace westProductFace(westRootPoints);
		glm::vec3 westFaceTargetEmptyNormal(-1.0f, 0, 0);
		rProductFaceMap[ECBPPOrientations::WESTFACE] = westProductFace;
		rProductFaceMap[ECBPPOrientations::WESTFACE].generateFacePTriangles(westFaceTargetEmptyNormal);
	}

	// check top face
	bool isTopUsable = meshCorners.checkIfFaceIsUsable(ECBPPOrientations::CORNER_UPPERSW,
														ECBPPOrientations::CORNER_UPPERSE,
														ECBPPOrientations::CORNER_UPPERNE,
														ECBPPOrientations::CORNER_UPPERNW);
	if (isTopUsable == true)
	{
		std::cout << "Generating TOPFACE..." << std::endl;
		RProductFaceRootPoints topRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_UPPERSW,
																			ECBPPOrientations::CORNER_UPPERSE,
																			ECBPPOrientations::CORNER_UPPERNE,
																			ECBPPOrientations::CORNER_UPPERNW);
		RProductFace topProductFace(topRootPoints);
		glm::vec3 topFaceTargetEmptyNormal(0, 1.0f, 0);
		rProductFaceMap[ECBPPOrientations::TOPFACE] = topProductFace;
		rProductFaceMap[ECBPPOrientations::TOPFACE].generateFacePTriangles(topFaceTargetEmptyNormal);
	}

	// check bottom face
	bool isBottomUsable = meshCorners.checkIfFaceIsUsable(ECBPPOrientations::CORNER_LOWERNW,
														ECBPPOrientations::CORNER_LOWERNE,
														ECBPPOrientations::CORNER_LOWERSE,
														ECBPPOrientations::CORNER_LOWERSW);
	if (isBottomUsable == true)
	{
		std::cout << "Generating BOTTOMFACE..." << std::endl;
		RProductFaceRootPoints bottomRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_LOWERNW,
																			ECBPPOrientations::CORNER_LOWERNE,
																			ECBPPOrientations::CORNER_LOWERSE,
																			ECBPPOrientations::CORNER_LOWERSW);
		RProductFace bottomProductFace(bottomRootPoints);
		glm::vec3 bottomFaceTargetEmptyNormal(0, -1.0f, 0);
		rProductFaceMap[ECBPPOrientations::BOTTOMFACE] = bottomProductFace;
		rProductFaceMap[ECBPPOrientations::BOTTOMFACE].generateFacePTriangles(bottomFaceTargetEmptyNormal);
	}
}

void RMorphableMesh::runSuctionByXSlice(float in_cubeDimLength, float in_tileDimWeightRatio, int in_tilesPerDim)
{
	RMorphableMeshCornerArray fetchedArray = meshCorners.getPointArray();

	// the following loop is for diagnostic testing only
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

	// attempt to run the point modifier on all 8 points of this mesh
	RCollisionPointModifier pointModifier(&fetchedArray, translatorRef, in_cubeDimLength, in_tileDimWeightRatio, in_tilesPerDim);
}