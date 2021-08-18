#include "stdafx.h"
#include "RMorphableAreaScanner.h"

void RMorphableAreaScanner::addSPolyToGrid(SPoly in_sPolyToAdd)
{
	massGrid.addGridRPoly(in_sPolyToAdd);
}

void RMorphableAreaScanner::handleGenericDO(DebugOption in_debugOption)
{
	switch (in_debugOption)
	{
		case DebugOption::RPOLY_RTRIANGLE_TRACING: { massGrid.setLoggerDebugLevel(PolyDebugLevel::DEBUG); break;}
	}
}

void RMorphableAreaScanner::buildGridMassShell()
{
	auto shellBuildStart = std::chrono::high_resolution_clock::now();
	std::cout << "##################### Build shell START" << std::endl;
	massGrid.buildShell();
	std::cout << "##################### Build shell END" << std::endl;
	auto shellBuildEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> comparisonIterationsElapsed = shellBuildEnd - shellBuildStart;
	std::cout << "Shell build time: " << comparisonIterationsElapsed.count() << std::endl;
}

void RMorphableAreaScanner::fillGridMass()
{
	massGrid.fillMass();
}

MassGridArray* RMorphableAreaScanner::fetchMassGridArrayRef()
{
	return &massGrid.dataArray;
}

void RMorphableAreaScanner::scanGridMass()
{
	// Generate the dynamic border line list, that all mesh groups will need to have a reference to.

	// Phase 1: determine which morphable mesh areas will be used, and put them into the ungrouped meshes map.
	for (int x = 0; x < meshesPerDimension; x++)
	{
		for (int y = 0; y < meshesPerDimension; y++)
		{
			for (int z = 0; z < meshesPerDimension; z++)
			{
				// calculate the center point of the current candidate mesh; it's a point equal to (the current key * morphableMeshDimension ) + (morphableMeshDimension / 2)
				float currentMeshCenterX = (x * morphableMeshDimension) + (morphableMeshDimension / 2);
				float currentMeshCenterY = (y * morphableMeshDimension) + (morphableMeshDimension / 2);
				float currentMeshCenterZ = (z * morphableMeshDimension) + (morphableMeshDimension / 2);
				glm::vec3 currentMeshCenter(currentMeshCenterX, currentMeshCenterY, currentMeshCenterZ);
				RMorphableMesh currentCandidateMesh(2, currentMeshCenter, morphableMeshDimension, &gridTranslator, &massGrid);

				
				RMorphableMeshCubeAreaDefiner currentMeshAreaScanner = currentCandidateMesh.getAreaDefiner();
				EnclaveKeyDef::EnclaveKey convertedPointA = massGrid.convertSPolyPointToBlockCoord(currentMeshAreaScanner.definingPointA);
				EnclaveKeyDef::EnclaveKey convertedPointB = massGrid.convertSPolyPointToBlockCoord(currentMeshAreaScanner.definingPointB);
				MassGridArrayCellScanArea scanArea(convertedPointA, convertedPointB);

				
				bool wasMassFound = massGrid.wasMassFoundInAreaScan(scanArea);
				if (wasMassFound == true)
				{
					PolyLogger tempMassLogger;
					tempMassLogger.setDebugLevel(scannerDebugOptions.returnDebugLevelIfFound(DebugOption::RMORPHABLEAREASCANNER_PRINT_MESHES_CONTAINING_MASS));
					//std::cout << "!! Notice: morphable mesh at mesh coord " << x << ", " << y << ", " << z << " contains mass. " << std::endl;
					tempMassLogger.log("(RMorphableAreaScanner): !! Notice: morphable mesh at mesh coord ", x, ", ", y, ", ", z, " contains mass. ", "\n");
					
					EnclaveKeyDef::EnclaveKey currentMeshKey(x, y, z);
					ungroupedMeshes[currentMeshKey] = currentCandidateMesh;
				}
				
			}
		}
	}

	// Phase 2: run the code for building each group of keys.
	if (!ungroupedMeshes.empty())
	{
		PolyLogger meshGroupBuildingLogger;
		meshGroupBuildingLogger.setDebugLevel(scannerDebugOptions.returnDebugLevelIfFound(DebugOption::RMORPHABLEAREASCANNER_MESH_GROUP_CONSTRUCTION));
		meshGroupBuildingLogger.log("(RMorphableAreaScanner): current size of ungrouped mesh, before move: ", ungroupedMeshes.size(), "\n");

		// move the very first ungrouped mesh into the candidateMesh map; because we must start somewhere.
		auto firstUngroupedMesh = ungroupedMeshes.begin();
		EnclaveKeyDef::EnclaveKey firstKey = firstUngroupedMesh->first;
		currentMeshGroup[firstKey] = std::move(firstUngroupedMesh->second);
		ungroupedMeshes.erase(firstKey);

		meshGroupBuildingLogger.log("(RMorphableAreaScanner): current size of ungrouped mesh, after move: ", ungroupedMeshes.size(), "\n");
		meshGroupBuildingLogger.log("(RMorphableAreaScanner): current size of currentMeshGroup: ", currentMeshGroup.size(), "\n");

		int remainingMeshesToHandle = ungroupedMeshes.size();
		int currentMeshGroupID = 0;
		for (int x = 0; x < remainingMeshesToHandle; x++)
		{
			bool wasMeshToStickToFound = false;

			// compare each mesh to all meshes in the currentMeshGroup; 
			EnclaveKeyDef::EnclaveKey foundKey;
			EnclaveKeyDef::EnclaveKey candidateKey;
			auto remainingMeshesBegin = ungroupedMeshes.begin();
			auto remainingMeshesEnd = ungroupedMeshes.end();
			for (; remainingMeshesBegin != remainingMeshesEnd; remainingMeshesBegin++)
			{
				auto currentMeshGroupMeshesBegin = currentMeshGroup.begin();
				auto currentMeshGroupMeshesEnd = currentMeshGroup.end();
				for (; currentMeshGroupMeshesBegin != currentMeshGroupMeshesEnd; currentMeshGroupMeshesBegin++)
				{
					candidateKey = remainingMeshesBegin->first;
					EnclaveKeyDef::EnclaveKey currentMeshGroupMeshKey = currentMeshGroupMeshesBegin->first;
					
					bool neighborsFound = checkIfKeysAreNeighbors(candidateKey, currentMeshGroupMeshKey);
					if (neighborsFound == true)
					{
						foundKey = candidateKey;
						wasMeshToStickToFound = true;
						meshGroupBuildingLogger.log("(RMorphableAreaScanner): >>>> Mesh to stick was found; flagging for move. Key(", foundKey.x, ", ", foundKey.y, ", ", foundKey.z, "\n");
						goto NEXTACTION;
					}
				}
			}

			// take action, based on the result
			NEXTACTION:
			if (wasMeshToStickToFound == true)
			{
				currentMeshGroup[foundKey] = ungroupedMeshes[foundKey];
				ungroupedMeshes.erase(foundKey);
			}
			else if (wasMeshToStickToFound == false)
			{
				meshGroupBuildingLogger.log("(RMorphableAreaScanner): No stick found, for key: ", candidateKey.x, ", ", candidateKey.y, ", ", candidateKey.z, "\n");
				meshGroupMap[currentMeshGroupID++].keyedMorphables = currentMeshGroup;		// move over the current mesh group, because it didn't match to anything
				currentMeshGroup.clear();													// clear it out

				auto nextMesh = ungroupedMeshes.begin();									// grab the first mesh in the remaining candidate pool; put it as the start of the new group.
				currentMeshGroup[nextMesh->first] = nextMesh->second;
					
				ungroupedMeshes.erase(nextMesh->first);				// erase the mesh we copied over fromt he ungroupedMesh

			}
		}
		meshGroupMap[currentMeshGroupID++].keyedMorphables = currentMeshGroup;	// move over the final group
	}
	
	// OPTIONAL: for debug output only.
	if (scannerDebugOptions.containsOption(DebugOption::RMORPHABLEAREASCANNER_PRINT_RMORPHABLEMESHGROUP_MESH_KEYS))
	{
		PolyLogger meshKeyOutputLogger;
		meshKeyOutputLogger.setDebugLevel(PolyDebugLevel::DEBUG);
		auto meshGroupsBegin = meshGroupMap.begin();
		auto meshGroupsEnd = meshGroupMap.end();
		for (; meshGroupsBegin != meshGroupsEnd; meshGroupsBegin++)
		{
			//std::cout << "Printing out contents for mesh group with ID " << meshGroupsBegin->first << ": " << std::endl;
			meshKeyOutputLogger.log("(RMorphableAreaScanner): Printing out EnclaveKey values for RMorphableMesh in each RMorphableMeshGroup...", "\n");
			auto meshGroupEntriesBegin = meshGroupsBegin->second.keyedMorphables.begin();
			auto meshGroupEntriesEnd = meshGroupsBegin->second.keyedMorphables.end();
			for (; meshGroupEntriesBegin != meshGroupEntriesEnd; meshGroupEntriesBegin++)
			{
				//std::cout << "Mesh with key (" << meshGroupEntriesBegin->first.x << ", " << meshGroupEntriesBegin->first.y << ", " << meshGroupEntriesBegin->first.z << ") " << std::endl;
				meshKeyOutputLogger.log("(RMorphableAreaScanner): Key value found -> (", meshGroupEntriesBegin->first.x, ", ", meshGroupEntriesBegin->first.y, ", ", meshGroupEntriesBegin->first.z, ") ", "\n");
			}
		}
	}

	// OPTION 1: ----------------------------------------------------------
	// Phase 3: for each RMorphableMeshGroup, generate its points and hollow out its interiors, aka non-visible points.
	/*
	auto hollowingOutBegin = meshGroupMap.begin();
	auto hollowingOutEnd = meshGroupMap.end();
	for (; hollowingOutBegin != hollowingOutEnd; hollowingOutBegin++)
	{
		hollowingOutBegin->second.setDynamicBorderRef(&scannerDynamicBorderLineList);	
		hollowingOutBegin->second.generatePointArray(pointsPerSlicePointArray);				// size of array is equal to the number of RMorphableMeshes * 8
		//hollowingOutBegin->second.generatePoints();					// generate all possible points
		//hollowingOutBegin->second.updatePointLandlockStats();		// determine which points in the mesh are landlocked.
		//hollowingOutBegin->second.flagLandlockedMeshes();	// remember, meshes that are completely surrounded on all 6 sides are considered to be "landlocked"
		//hollowingOutBegin->second.generateRProductFacesInRemainingMeshes();
		//hollowingOutBegin->second.printLandlockedPoints();
	}

	// Phase 4: run suction for each mesh group
	auto suctionBegin = meshGroupMap.begin();
	auto suctionEnd = meshGroupMap.end();
	for (; suctionBegin != suctionEnd; suctionBegin++)
	{
		suctionBegin->second.buildMeshByXScan(massGrid.fetchDataArrayRef(), morphableMeshDimension, pointsPerSlicePointArray, &scannerPointToTriangleMapper);
	}
	*/

	// OPTION 2: ---------------------------------------------------------

	auto originalMeshRunBegin = meshGroupMap.begin();
	auto origianlMeshRunEnd = meshGroupMap.end();
	for (; originalMeshRunBegin != origianlMeshRunEnd; originalMeshRunBegin++)
	{
		originalMeshRunBegin->second.insertMorphableMeshDebugOptions(scannerStoredMeshDebugOptions);
		originalMeshRunBegin->second.setDynamicBorderRef(&scannerDynamicBorderLineList);
		originalMeshRunBegin->second.generatePointArray(8);
		originalMeshRunBegin->second.generatePoints();							// generate all possible points
		originalMeshRunBegin->second.updatePointLandlockStats();				// determine which points in the mesh are landlocked.
		originalMeshRunBegin->second.updatePointsWithinMass(massGrid.fetchDataArrayRef(), &gridTranslator);
		originalMeshRunBegin->second.flagLandlockedMeshes();					// remember, meshes that are completely surrounded on all 6 sides are considered to be "landlocked"
		originalMeshRunBegin->second.generateRProductFacesInRemainingMeshes();	// build the exposed faces in each mesh, and build the pTriangles in those faces.
		//originalMeshRunBegin->second.printLandlockedPoints();
	}
	auto originalSuctionBegin = meshGroupMap.begin();
	auto originalSuctionEnd = meshGroupMap.end();
	for (; originalSuctionBegin != originalSuctionEnd; originalSuctionBegin++)
	{
		originalSuctionBegin->second.buildMeshByXScanV2(massGrid.fetchDataArrayRef(), 
													    morphableMeshDimension, 
														pointsPerSlicePointArray, 
														&scannerPointToTriangleMapper, 
														massGrid.getRCubeDimLength(), 
														massGrid.getTileDimWeightRatio(),
														scannerCellsPerDimension
														);
	}

	acquireProducedSolutions();
}

void RMorphableAreaScanner::acquireProducedSolutions()
{
	// for each mesh group, have it produce its solutions.
	auto solutionProductionBegin = meshGroupMap.begin();
	auto solutionProductionEnd = meshGroupMap.end();
	for (; solutionProductionBegin != solutionProductionEnd; solutionProductionBegin++)
	{
		solutionProductionBegin->second.produceSolution();
	}
}

bool RMorphableAreaScanner::checkIfKeysAreNeighbors(EnclaveKeyDef::EnclaveKey in_keyA, EnclaveKeyDef::EnclaveKey in_keyB)
{
	bool areKeysNeighbors = false;

	bool validX = false;
	bool validY = false;
	bool validZ = false;

	if
	(
		(abs(in_keyA.x - in_keyB.x) == 1)
		&&
		(abs(in_keyA.y - in_keyB.y) == 0)
		&&
		(abs(in_keyA.z - in_keyB.z) == 0)
	)
	{
		validX = true;
	}

	if
	(
		(abs(in_keyA.x - in_keyB.x) == 0)
		&&
		(abs(in_keyA.y - in_keyB.y) == 1)
		&&
		(abs(in_keyA.z - in_keyB.z) == 0)
	)
	{

		validY = true;
	}

	if
	(
		(abs(in_keyA.x - in_keyB.x) == 0)
		&&
		(abs(in_keyA.y - in_keyB.y) == 0)
		&&
		(abs(in_keyA.z - in_keyB.z) == 1)
	)
	{

		validZ = true;
	}

	if
	(
		(validX == true)
		||
		(validY == true)
		||
		(validZ == true)
	)
	{
		areKeysNeighbors = true;
	}
	return areKeysNeighbors;
}

SPoly RMorphableAreaScanner::convertPTriangleToSPoly(PTriangle in_pTriangle)
{
	SPoly returnSPoly;
	
	returnSPoly.setEmptyNormal(in_pTriangle.getEmptyNormal());
	returnSPoly.setSPolyMaterial(in_pTriangle.getMaterialID());

	STriangle currentSTriangle(in_pTriangle.collisionPointRefArray[0]->currentValue, 
							in_pTriangle.collisionPointRefArray[1]->currentValue, 
							in_pTriangle.collisionPointRefArray[2]->currentValue);
	currentSTriangle.setSTriangleMaterialID(in_pTriangle.getMaterialID());
	returnSPoly.addTriangle(currentSTriangle);
	
	return returnSPoly;
}

std::vector<SPoly> RMorphableAreaScanner::produceSPolysFromPTriangleMeshes()
{
	std::vector<SPoly> returnVector;
	auto currentMeshGroupBegin = meshGroupMap.begin();
	auto currentMeshGroupEnd = meshGroupMap.end();
	for (; currentMeshGroupBegin != currentMeshGroupEnd; currentMeshGroupBegin++)
	{
		auto currentMeshGroupPTrianglesBegin = currentMeshGroupBegin->second.groupMesh.meshPTriangles.begin();
		auto currentMeshGroupPTrianglesEnd = currentMeshGroupBegin->second.groupMesh.meshPTriangles.end();
		for (; currentMeshGroupPTrianglesBegin != currentMeshGroupPTrianglesEnd; currentMeshGroupPTrianglesBegin++)
		{
			SPoly currentConvertedSPoly = convertPTriangleToSPoly(currentMeshGroupPTrianglesBegin->second);
			returnVector.push_back(currentConvertedSPoly);
		}
	}
	return returnVector;
}