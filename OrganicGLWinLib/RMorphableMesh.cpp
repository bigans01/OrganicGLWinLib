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

void RMorphableMesh::insertPTrianglesIntoMesh(PTriangleMesh* in_pTriangleMeshRef)
{
	auto existingFacesBegin = rProductFaceMap.begin();
	auto existingFacesEnd = rProductFaceMap.end();
	for (; existingFacesBegin != existingFacesEnd; existingFacesBegin++)
	{
		existingFacesBegin->second.copyPTrianglesIntoMesh(in_pTriangleMeshRef);
	}
}

void RMorphableMesh::printPTrianglePoints()
{
	auto facesBegin = rProductFaceMap.begin();
	auto facesEnd = rProductFaceMap.end();
	for (; facesBegin != facesEnd; facesBegin++)
	{
		facesBegin->second.printPTrianglePoints();
	}
}

void RMorphableMesh::generateRProductFaces(bool in_debugOutputValue)
{
	// check north face.
	bool isNorthUsable = meshCorners.checkIfFaceIsUsable(ECBPPOrientations::CORNER_UPPERNW,
													     ECBPPOrientations::CORNER_UPPERNE,
														 ECBPPOrientations::CORNER_LOWERNE,
														 ECBPPOrientations::CORNER_LOWERNW);
	if (isNorthUsable == true)
	{
		if (in_debugOutputValue == true)
		{
			std::cout << "Generating NORTHFACE..." << std::endl;
		}
		RProductFaceRootPoints northRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_UPPERNW,
																			ECBPPOrientations::CORNER_UPPERNE,
																			ECBPPOrientations::CORNER_LOWERNE,
																			ECBPPOrientations::CORNER_LOWERNW);
		RProductFace northProductFace(northRootPoints);
		glm::vec3 northFaceTargetEmptyNormal(0, 0, -1.0f);
		rProductFaceMap[ECBPPOrientations::NORTHFACE] = northProductFace;
		rProductFaceMap[ECBPPOrientations::NORTHFACE].generateFacePTriangles(northFaceTargetEmptyNormal, in_debugOutputValue);
	}
	
	// check east face.
	bool isEastUsable = meshCorners.checkIfFaceIsUsable(ECBPPOrientations::CORNER_UPPERNE,
														ECBPPOrientations::CORNER_UPPERSE,
														ECBPPOrientations::CORNER_LOWERSE,
														ECBPPOrientations::CORNER_LOWERNE);
	if (isEastUsable == true)
	{
		if (in_debugOutputValue == true)
		{
			std::cout << "Generating EASTFACE..." << std::endl;
		}
		RProductFaceRootPoints eastRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_UPPERNE,
																			ECBPPOrientations::CORNER_UPPERSE,
																			ECBPPOrientations::CORNER_LOWERSE,
																			ECBPPOrientations::CORNER_LOWERNE);
		RProductFace eastProductFace(eastRootPoints);
		glm::vec3 eastFaceTargetEmptyNormal(1.0f, 0, 0);
		rProductFaceMap[ECBPPOrientations::EASTFACE] = eastProductFace;
		rProductFaceMap[ECBPPOrientations::EASTFACE].generateFacePTriangles(eastFaceTargetEmptyNormal, in_debugOutputValue);
	}
	
	// check south face
	bool isSouthUsable = meshCorners.checkIfFaceIsUsable(ECBPPOrientations::CORNER_UPPERSE,
														ECBPPOrientations::CORNER_UPPERSW,
														ECBPPOrientations::CORNER_LOWERSW,
														ECBPPOrientations::CORNER_LOWERSE);
	if (isSouthUsable == true)
	{
		if (in_debugOutputValue == true)
		{
			std::cout << "Generating SOUTHFACE..." << std::endl;
		}
		RProductFaceRootPoints southRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_UPPERSE,
																			ECBPPOrientations::CORNER_UPPERSW,
																			ECBPPOrientations::CORNER_LOWERSW,
																			ECBPPOrientations::CORNER_LOWERSE);
		RProductFace southProductFace(southRootPoints);
		glm::vec3 southFaceTargetEmptyNormal(0, 0, 1.0f);
		rProductFaceMap[ECBPPOrientations::SOUTHFACE] = southProductFace;
		rProductFaceMap[ECBPPOrientations::SOUTHFACE].generateFacePTriangles(southFaceTargetEmptyNormal, in_debugOutputValue);
	}
	
	// check west face
	bool isWestUsable = meshCorners.checkIfFaceIsUsable(ECBPPOrientations::CORNER_UPPERSW,
														ECBPPOrientations::CORNER_UPPERNW,
														ECBPPOrientations::CORNER_LOWERNW,
														ECBPPOrientations::CORNER_LOWERSW);
	if (isWestUsable == true)
	{
		if (in_debugOutputValue == true)
		{
			std::cout << "Generating WESTFACE..." << std::endl;
		}
		RProductFaceRootPoints westRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_UPPERSW,
																			ECBPPOrientations::CORNER_UPPERNW,
																			ECBPPOrientations::CORNER_LOWERNW,
																			ECBPPOrientations::CORNER_LOWERSW);
		RProductFace westProductFace(westRootPoints);
		glm::vec3 westFaceTargetEmptyNormal(-1.0f, 0, 0);
		rProductFaceMap[ECBPPOrientations::WESTFACE] = westProductFace;
		rProductFaceMap[ECBPPOrientations::WESTFACE].generateFacePTriangles(westFaceTargetEmptyNormal, in_debugOutputValue);
	}

	// check top face
	bool isTopUsable = meshCorners.checkIfFaceIsUsable(ECBPPOrientations::CORNER_UPPERSW,
														ECBPPOrientations::CORNER_UPPERSE,
														ECBPPOrientations::CORNER_UPPERNE,
														ECBPPOrientations::CORNER_UPPERNW);
	if (isTopUsable == true)
	{
		if (in_debugOutputValue == true)
		{
			std::cout << "Generating TOPFACE..." << std::endl;
		}
		RProductFaceRootPoints topRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_UPPERSW,
																			ECBPPOrientations::CORNER_UPPERSE,
																			ECBPPOrientations::CORNER_UPPERNE,
																			ECBPPOrientations::CORNER_UPPERNW);
		RProductFace topProductFace(topRootPoints);
		glm::vec3 topFaceTargetEmptyNormal(0, 1.0f, 0);
		rProductFaceMap[ECBPPOrientations::TOPFACE] = topProductFace;
		rProductFaceMap[ECBPPOrientations::TOPFACE].generateFacePTriangles(topFaceTargetEmptyNormal, in_debugOutputValue);
	}
	
	// check bottom face
	bool isBottomUsable = meshCorners.checkIfFaceIsUsable(ECBPPOrientations::CORNER_LOWERNW,
														ECBPPOrientations::CORNER_LOWERNE,
														ECBPPOrientations::CORNER_LOWERSE,
														ECBPPOrientations::CORNER_LOWERSW);
	if (isBottomUsable == true)
	{
		if (in_debugOutputValue == true)
		{
			std::cout << "Generating BOTTOMFACE..." << std::endl;
		}
		RProductFaceRootPoints bottomRootPoints = meshCorners.fetchRootPoints(ECBPPOrientations::CORNER_LOWERNW,
																			ECBPPOrientations::CORNER_LOWERNE,
																			ECBPPOrientations::CORNER_LOWERSE,
																			ECBPPOrientations::CORNER_LOWERSW);
		RProductFace bottomProductFace(bottomRootPoints);
		glm::vec3 bottomFaceTargetEmptyNormal(0, -1.0f, 0);
		rProductFaceMap[ECBPPOrientations::BOTTOMFACE] = bottomProductFace;
		rProductFaceMap[ECBPPOrientations::BOTTOMFACE].generateFacePTriangles(bottomFaceTargetEmptyNormal, in_debugOutputValue);
	}
}

void RMorphableMesh::runSuctionByXSlice(float in_cubeDimLength, 
										float in_tileDimWeightRatio, 
										int in_tilesPerDim, 
										MassGridArray* in_massGridArrayRef,
										PolyLogger* in_suctionPolyLoggerRef)
{
	RMorphableMeshCornerArray fetchedArray = meshCorners.getPointArray();

	// the following loop is for diagnostic testing only
	for (int x = 0; x < 8; x++)
	{
		RCollisionPoint* currentPointRef = fetchedArray.pointRefArray[x];
		glm::vec3 currentRefedPointOriginalValue = currentPointRef->originalValue;
		glm::vec3 currentRefedPointCurrentValue = currentPointRef->currentValue;
		EnclaveKeyDef::EnclaveKey translatedPoint = translatorRef->translatePointToGrid(currentRefedPointOriginalValue);
		MassGridSearchResult searchResult = gridRef->fetchCell(translatedPoint);
		std::string massValue = "EMPTY";
		std::string wasFound = "No";
		std::string wasLandlocked = "No";
		int totalFlagsFound = 0;
		if (currentPointRef->getCollisionPointState() == RCollisionPointState::IMMUTABLE)
		{
			wasLandlocked = "IMMUTABLE";
		}
		if (currentPointRef->getCollisionPointState() == RCollisionPointState::MOVED)
		{
			wasLandlocked = "MOVED";
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
		
		/*
		std::cout << ":: Point at array index " << x << "having original value of ( " << currentRefedPointOriginalValue.x << "," 
															 << currentRefedPointOriginalValue.y << ", " << 
																currentRefedPointOriginalValue.z << ") translates to array value: (" << translatedPoint.x << ", " << translatedPoint.y << ", " << translatedPoint.z 
															<< " and has a current value of (" <<  currentRefedPointCurrentValue.x << ", " 
															<< currentRefedPointCurrentValue.y << ", "
															<< currentRefedPointCurrentValue.z 
														    << "), wasFound = " << wasFound 
													        << ", and has a " 
															<< massValue << " mass; totalFlagsFound = " << totalFlagsFound << "; state = " << wasLandlocked << std::endl;
		*/
		in_suctionPolyLoggerRef->log("(RMorphableMesh) :: Point at array index ", x, " having original value of ( ", currentRefedPointOriginalValue.x, ", ",
			currentRefedPointOriginalValue.y, ", ",
			currentRefedPointOriginalValue.z, ") translated to array value: (", translatedPoint.x, ", ", translatedPoint.y, ", ", translatedPoint.z,
			" and has a current value of (", currentRefedPointCurrentValue.x, ", ",
			currentRefedPointCurrentValue.y, ", ",
			currentRefedPointCurrentValue.z,
			"), wasFound = ", wasFound,
			", and has a ",
			massValue, " mass; totalFlagsFound =  ", totalFlagsFound, "; state = ", wasLandlocked, "\n");

	}

	// attempt to run the point modifier on all 8 points of this mesh
	
	RCollisionPointModifier pointModifier(&fetchedArray, 
										translatorRef, 
										in_cubeDimLength, 
										in_tileDimWeightRatio, 
										in_tilesPerDim, 
										in_massGridArrayRef, 
										in_suctionPolyLoggerRef);
										
}