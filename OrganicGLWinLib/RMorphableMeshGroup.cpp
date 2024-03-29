#include "stdafx.h"
#include "RMorphableMeshGroup.h"

void RMorphableMeshGroup::setDynamicBorderRef(DynamicBorderLineList* in_dynamicBorderLineListRef)
{
	dynamicBorderRef = in_dynamicBorderLineListRef;
}

void RMorphableMeshGroup::insertMeshIntoGroup(EnclaveKeyDef::EnclaveKey in_meshKey, RMorphableMesh in_mesh)
{
	keyedMorphables[in_meshKey] = in_mesh;
}

void RMorphableMeshGroup::flagLandlockedMeshes()
{
	std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> keysToRemove;
	auto removalScanBegin = keyedMorphables.begin();
	auto removalScanEnd = keyedMorphables.end();
	for (; removalScanBegin != removalScanEnd; removalScanBegin++)
	{
		EnclaveKeyDef::EnclaveKey currentMorphableMeshKey = removalScanBegin->first;	// the current key of the mesh we're looking at.

		// north key check
		bool northKeyFound = false;
		EnclaveKeyDef::EnclaveKey northKey(currentMorphableMeshKey);
		northKey.z -= 1;
		auto northKeyFinder = keyedMorphables.find(northKey);
		if (northKeyFinder != keyedMorphables.end())
		{
			northKeyFound = true;
		}

		// east key check
		bool eastKeyFound = false;
		EnclaveKeyDef::EnclaveKey eastKey(currentMorphableMeshKey);
		eastKey.x += 1;
		auto eastKeyFinder = keyedMorphables.find(eastKey);
		if (eastKeyFinder != keyedMorphables.end())
		{
			eastKeyFound = true;
		}

		// south key check
		bool southKeyFound = false;
		EnclaveKeyDef::EnclaveKey southKey(currentMorphableMeshKey);
		southKey.z += 1;
		auto southKeyFinder = keyedMorphables.find(southKey);
		if (southKeyFinder != keyedMorphables.end())
		{
			southKeyFound = true;
		}

		// west key check
		bool westKeyFound = false;
		EnclaveKeyDef::EnclaveKey westKey(currentMorphableMeshKey);
		westKey.x -= 1;
		auto westKeyFinder = keyedMorphables.find(westKey);
		if (westKeyFinder != keyedMorphables.end())
		{
			westKeyFound = true;
		}

		// above key check
		bool aboveKeyFound = false;
		EnclaveKeyDef::EnclaveKey aboveKey(currentMorphableMeshKey);
		aboveKey.y += 1;
		auto aboveKeyFinder = keyedMorphables.find(aboveKey);
		if (aboveKeyFinder != keyedMorphables.end())
		{
			aboveKeyFound = true;
		}

		// below key check
		bool belowKeyFound = false;
		EnclaveKeyDef::EnclaveKey belowKey(currentMorphableMeshKey);
		belowKey.y -= 1;
		auto belowKeyFinder = keyedMorphables.find(belowKey);
		if (belowKeyFinder != keyedMorphables.end())
		{
			belowKeyFound = true;
		}

		// if completely surrounded, flag it for removal.
		if
		(
			(northKeyFound == true)
			&&
			(eastKeyFound == true)
			&&
			(southKeyFound == true)
			&&
			(westKeyFound == true)
			&&
			(aboveKeyFound == true)
			&&
			(belowKeyFound == true)
		)
		{
			//std::cout << "!! RMorphableMesh identified by key (" << removalScanBegin->first.x << ", " << removalScanBegin->first.y << ", " << removalScanBegin->first.z << ")" << std::endl;
			keysToRemove.insert(removalScanBegin->first);
		}
	}

	// remove each element that needed to be deleted
	auto keysToRemoveBegin = keysToRemove.begin();
	auto keysToRemoveEnd = keysToRemove.end();
	for (; keysToRemoveBegin != keysToRemoveEnd; keysToRemoveBegin++)
	{
		//keyedMorphables.erase(*keysToRemoveBegin);
		keyedMorphables[*keysToRemoveBegin].updatePointState(RMorphableMeshState::LANDLOCKED);
	}
}

void RMorphableMeshGroup::generatePointArray(int in_slicePointArraySize)
{
	slicePointArraySize = in_slicePointArraySize;
	int maxPointSize = int(keyedMorphables.size()) * slicePointArraySize;	// 8 points per morphable
	meshGroupPointArray.createArray(maxPointSize);
}

void RMorphableMeshGroup::generatePoints()
{
	auto keyedMorphablesBegin = keyedMorphables.begin();
	auto keyedMorphablesEnd = keyedMorphables.end();
	for (; keyedMorphablesBegin != keyedMorphablesEnd; keyedMorphablesBegin++)
	{
		RMorphableMeshCubeAreaDefiner currentAreaDefiner = keyedMorphablesBegin->second.getAreaDefiner();
		auto pointsToSearch = currentAreaDefiner.getPointRequests();
		RCollisionPoint* collisionPointRefs[8];
		for (int x = 0; x < 8; x++)
		{
			ECBPolyPoint glmConverted(pointsToSearch.points[x].x, 
									pointsToSearch.points[x].y, 
									pointsToSearch.points[x].z);
			ECBPPOrientationResults currentPointOrientation = IndependentUtils::getDynamicPointOrientation(glmConverted, dynamicBorderRef);
			RCollisionPoint* currentIndexPointRef = meshGroupPointArray.attemptPointInsert(pointsToSearch.points[x], currentPointOrientation);
			collisionPointRefs[x] = currentIndexPointRef;
		}
		RMorphableMeshCorners currentMeshCorners;
		currentMeshCorners.setCorners(collisionPointRefs[0],
			collisionPointRefs[1],
			collisionPointRefs[2],
			collisionPointRefs[3],
			collisionPointRefs[4],
			collisionPointRefs[5],
			collisionPointRefs[6],
			collisionPointRefs[7]);
		keyedMorphablesBegin->second.setMeshCorners(currentMeshCorners);
	}
}

void RMorphableMeshGroup::flagImmutablePoints()
{
	auto keyedMorphablesBegin = keyedMorphables.begin();
	auto keyedMorphablesEnd = keyedMorphables.end();
	for (; keyedMorphablesBegin != keyedMorphablesEnd; keyedMorphablesBegin++)
	{
		keyedMorphablesBegin->second.updatePointUsageCounts();
	}
}

void RMorphableMeshGroup::updatePointsWithinMass(MassGridArray* in_massGridArrayRef, RPointToGridTranslator* in_translatorRef)
{
	meshGroupPointArray.updatePointsFoundWithinMass(in_massGridArrayRef, in_translatorRef);
}

Operable3DEnclaveKeySet RMorphableMeshGroup::scanForSolidBlocks(MassGridArray* in_massGridArrayRef)
{
	// need to return an unordered_map of EnclaveKeys (maybe need a new class, like OperableIntSet?) (2/13/2022)
	Operable3DEnclaveKeySet landlockedKeys;

	auto keyedMorphablesBegin = keyedMorphables.begin();
	auto keyedMorphablesEnd = keyedMorphables.end();
	for (; keyedMorphablesBegin != keyedMorphablesEnd; keyedMorphablesBegin++)
	{
		// only LANDLOCKED blocks will ever be considered solid; so those are the only ones we need to look at.
		if (keyedMorphablesBegin->second.getMeshState() == RMorphableMeshState::LANDLOCKED)
		{
			bool wasTraceBitFound = in_massGridArrayRef->wasFlagDiscoveredInArea(keyedMorphablesBegin->second.getScanArea(), MassCellBitFlags::TRACE_BIT);
			// if there weren't any cells that had the TRACE_BIT set, it means that the LANDLOCKED mesh 
			// never got "pierced" by a MOVED RCollisionPoint that was traced. Which means, it is truly landlocked.
			//
			// However, if there were tracing bits found, we'll assume it's not LANDLOCKED -- but we will have to verify this later in the call to
			// the ORE function, setPendingRMatterSolids (see OrganicIndependents lib commit on 12/5/2022 for more details).
			if (wasTraceBitFound == false)
			{
				landlockedKeys += keyedMorphablesBegin->first;
			}
		}
	}

	return landlockedKeys;
}

Operable3DEnclaveKeySet	RMorphableMeshGroup::scanForSolidBlocksWithFilling(MassGridArray* in_massGridArrayRef)
{
	// need to return an unordered_map of EnclaveKeys (maybe need a new class, like OperableIntSet?) (2/13/2022)
	Operable3DEnclaveKeySet foundWholeBlocksWithFilling;

	auto keyedMorphablesBegin = keyedMorphables.begin();
	auto keyedMorphablesEnd = keyedMorphables.end();
	for (; keyedMorphablesBegin != keyedMorphablesEnd; keyedMorphablesBegin++)
	{
		// only LANDLOCKED blocks will ever be considered solid; so those are the only ones we need to look at.
		if (keyedMorphablesBegin->second.getMeshState() == RMorphableMeshState::LANDLOCKED)
		{
			bool wasInnerMassFound = in_massGridArrayRef->wasFlagDiscoveredInArea(keyedMorphablesBegin->second.getScanArea(), MassCellBitFlags::INNER_MASS);
			// if there weren't any cells that had the TRACE_BIT set, it means that the LANDLOCKED mesh 
			// never got "pierced" by a MOVED RCollisionPoint that was traced. Which means, it is truly landlocked.
			if (wasInnerMassFound == true)
			{
				foundWholeBlocksWithFilling += keyedMorphablesBegin->first;
			}
		}
	}

	return foundWholeBlocksWithFilling;
}

bool RMorphableMeshGroup::doesGroupContainKey(EnclaveKeyDef::EnclaveKey in_enclaveKey)
{
	bool wasFound;
	auto keyFinder = keyedMorphables.find(in_enclaveKey);
	if (keyFinder != keyedMorphables.end())
	{
		wasFound = true;
	}
	return wasFound;
}

void RMorphableMeshGroup::printLandlockedPoints()
{
	for (int x = 0; x < meshGroupPointArray.arraySize; x++)
	{
		if (meshGroupPointArray.collisionPoints[x].isPointLandlocked() == true)
		{
			auto currentPoint = meshGroupPointArray.collisionPoints[x].originalValue;
			std::cout << "!! Notice, point " << currentPoint.x << ", " << currentPoint.y << ", " << currentPoint.z << " is landlocked. " << std::endl;
		}
	}
}

void RMorphableMeshGroup::generateRProductFacesInRemainingMeshes()
{
	auto generationRunBegin = keyedMorphables.begin();
	auto generationRunEnd = keyedMorphables.end();
	for (; generationRunBegin != generationRunEnd; generationRunBegin++)
	{
		PolyLogger currentKeyGenerationLogger;
		currentKeyGenerationLogger.setDebugLevel(rMorphableMeshDebugOptions[generationRunBegin->first].returnDebugLevelIfFound(DebugOption::RMORPHABLEMESHGROUP_SPECIFIC_MESH_FACE_GENERATION));
		bool debugValueCheck = currentKeyGenerationLogger.isLoggingSet();
		if (generationRunBegin->second.getMeshState() == RMorphableMeshState::EXPOSED)
		{
			currentKeyGenerationLogger.log("(RMorphableMeshGroup): Generating associated faces and PTriangles for RMorphableMesh with key: ", generationRunBegin->first.x, ", ", generationRunBegin->first.y, ", ", generationRunBegin->first.z, "\n");
			generationRunBegin->second.generateRProductFaces(debugValueCheck);
			currentKeyGenerationLogger.waitForDebugInput();
		}
		
		// if the value of getMeshState() is LANDLOCKED, we won't produce anything.
		else if (generationRunBegin->second.getMeshState() == RMorphableMeshState::LANDLOCKED)
		{
			currentKeyGenerationLogger.log("(RMorphableMeshGroup): Mesh with key ", generationRunBegin->first.x, ", ", generationRunBegin->first.y, ", ", generationRunBegin->first.z, " is landlocked. Ignoring face production. ", "\n");
		}
	}
}

void RMorphableMeshGroup::insertMorphableMeshDebugOptions(std::unordered_map<EnclaveKeyDef::EnclaveKey, DebugOptionSet, EnclaveKeyDef::KeyHasher> in_rMorphableMeshDebugOptions)
{
	rMorphableMeshDebugOptions = in_rMorphableMeshDebugOptions;
}

void RMorphableMeshGroup::determineBestPointCount(int in_potentialPointCount)
{
	setPointCount = in_potentialPointCount;
}

bool RMorphableMeshGroup::checkForMorphableMeshDebugOption(EnclaveKeyDef::EnclaveKey in_morphableMeshKey, DebugOption in_optionToCheck)
{
	bool wasOptionFound = false;
	auto keyFinder = rMorphableMeshDebugOptions.find(in_morphableMeshKey);
	if (keyFinder != rMorphableMeshDebugOptions.end())
	{
		return keyFinder->second.containsOption(in_optionToCheck);
	}
	return wasOptionFound;
}

void RMorphableMeshGroup::buildMeshByXScan(MassGridArray* in_massGridArrayRef, float in_sliceThickness, int in_pointsPerSliceArray, RCollisionPointToPTriangleMapContainer* in_pointToTriangleMapContainerRef)
{
	// find the lowest/highest x values, by using min and max
	int minX = 1000;	// should always start high 
	int maxX = 0;		// should always start low
	

	// print values
	//std::cout << ":::: buildMeshByXScan, minX: " << minX << std::endl;
	//std::cout << ":::: buildMeshByXScan, maxX: " << maxX << std::endl;
	
	auto keyedMorphablesBegin = keyedMorphables.begin();
	auto keyedMorphablesEnd = keyedMorphables.end();
	for (; keyedMorphablesBegin != keyedMorphablesEnd; keyedMorphablesBegin++)
	{
		minX = std::min(minX, keyedMorphablesBegin->first.x);
		maxX = std::max(maxX, keyedMorphablesBegin->first.x);
	}

	// for when there is more than one slice
	if (minX != maxX)
	{
		// iterate from min to max; slicing goes from west (x = 0) towards east (x > 0)

		// First pass: build slices, without initializing their arrays.
		for (int currentXSliceIndex = minX; currentXSliceIndex < maxX + 1; currentXSliceIndex++)	// maxX + 1 because we must include  maxX.
		{

			std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> currentXSliceIndexSet;
			auto currentKeyedScanForXBegin = keyedMorphables.begin();
			auto currentKeyedScanForXEnd = keyedMorphables.end();
			for (; currentKeyedScanForXBegin != currentKeyedScanForXEnd; currentKeyedScanForXBegin++)
			{
				if (currentKeyedScanForXBegin->first.x == currentXSliceIndex)
				{
					currentXSliceIndexSet.insert(currentKeyedScanForXBegin->first);
				}
			}

			// west slice goes first:
			if (currentXSliceIndex == minX)
			{
				sliceMap[currentXSliceIndex].reset(new RAdditiveXSliceWestEnd());
				sliceMap[currentXSliceIndex]->initialize(RAdditiveSliceType::X_SLICE_WEST_END, 
													in_massGridArrayRef, 
													&meshGroupPointArray, 
													in_sliceThickness,
													currentXSliceIndex, 
													&keyedMorphables,
													std::move(currentXSliceIndexSet), 
													dynamicBorderRef, in_pointToTriangleMapContainerRef);
			}

			// east slice goes last:
			else if (currentXSliceIndex == maxX)
			{
				sliceMap[currentXSliceIndex].reset(new RAdditiveXSliceEastEnd());
				sliceMap[currentXSliceIndex]->initialize(RAdditiveSliceType::X_SLICE_EAST_END, 
													in_massGridArrayRef, 
													&meshGroupPointArray,
													in_sliceThickness,
													currentXSliceIndex, 
													&keyedMorphables,
													std::move(currentXSliceIndexSet),
													dynamicBorderRef, in_pointToTriangleMapContainerRef);
			}

			// all other slices
			else
			{
				sliceMap[currentXSliceIndex].reset(new RAdditiveXSlice());
				sliceMap[currentXSliceIndex]->initialize(RAdditiveSliceType::X_SLICE, 
													in_massGridArrayRef, 
													&meshGroupPointArray, 
													in_sliceThickness,
													currentXSliceIndex, 
													&keyedMorphables,
													std::move(currentXSliceIndexSet),
													dynamicBorderRef, in_pointToTriangleMapContainerRef);
			}


			/*
			// run "suction" on each morphable mesh.
			auto xSliceSetBegin = currentXSliceIndexSet.begin();
			auto xSliceSetEnd = currentXSliceIndexSet.end();
			for (; xSliceSetBegin != xSliceSetEnd; xSliceSetBegin++)
			{
				std::cout << "Running suction for mesh at key: " << xSliceSetBegin->x << ", " << xSliceSetBegin->y << ", " << xSliceSetBegin->z << std::endl;
				keyedMorphables[*xSliceSetBegin].runSuctionByXSlice();

				int suctionWait = 3;
				std::cin >> suctionWait;
			}
			*/

		}


		// Step 2: find the best point count, by analyzing all slices and their keyed morphables; the resulting value is stored into setPointCount.
		determineBestPointCount(in_pointsPerSliceArray);

		// Step 3 (Second Pass): set the "best" value for array size in each slice, and initialize the point sets.
		auto pass2Begin = sliceMap.begin();
		auto pass2End = sliceMap.end();
		for (; pass2Begin != pass2End; pass2Begin++)
		{
			pass2Begin->second->initializeSetArrays(setPointCount);
			pass2Begin->second->buildInitialPointSets();
			pass2Begin->second->runInitialPointAdjustments();
		}


		// Step 4 (Third Pass): copy set refs from previous slices.
		// do the following for all but the first slice; that current slice must inherit the point B set from the previous slice.
		for (int currentXSliceIndex = minX; currentXSliceIndex < maxX + 1; currentXSliceIndex++)	// maxX + 1 because we must include  maxX.
		{
			if (currentXSliceIndex != minX)
			{
				RAdditiveSliceBase* previousSliceRef = sliceMap[currentXSliceIndex - 1].get();
				sliceMap[currentXSliceIndex]->copySetBRefsFromPreviousSlice(previousSliceRef);
			}
		}

		// test output: print the points in each slice's two sets.
		auto sliceMapBegin = sliceMap.begin();
		auto sliceMapEnd = sliceMap.end();
		for (; sliceMapBegin != sliceMapEnd; sliceMapBegin++)
		{
			std::cout << "~~~~~~~ Printing out points for slice at index " << sliceMapBegin->first << ": " << std::endl;
			sliceMapBegin->second.get()->printAllSetPoints();
		}



		// produce the PTriangles for each slice.
		auto pTriangleProductionBegin = sliceMap.begin();
		auto pTriangleProductionEnd = sliceMap.end();
		for (; pTriangleProductionBegin != pTriangleProductionEnd; pTriangleProductionBegin++)
		{
			pTriangleProductionBegin->second.get()->buildPTriangles(false);
		}
	}

	// there is only one slice.
	else if (minX == maxX)
	{

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> currentXSliceIndexSet;
		auto currentKeyedScanForXBegin = keyedMorphables.begin();
		auto currentKeyedScanForXEnd = keyedMorphables.end();
		for (; currentKeyedScanForXBegin != currentKeyedScanForXEnd; currentKeyedScanForXBegin++)
		{
			if (currentKeyedScanForXBegin->first.x == minX)
			{
				currentXSliceIndexSet.insert(currentKeyedScanForXBegin->first);
			}
		}

		sliceMap[0].reset(new RAdditiveXSliceStandalone());
		sliceMap[0]->initialize(RAdditiveSliceType::X_SLICE_STANDALONE, 
								in_massGridArrayRef, 
								&meshGroupPointArray,
								in_sliceThickness,
								0, 
								&keyedMorphables,
								std::move(currentXSliceIndexSet),
								dynamicBorderRef, in_pointToTriangleMapContainerRef);
		determineBestPointCount(in_pointsPerSliceArray);
		sliceMap[0]->initializeSetArrays(setPointCount);
		sliceMap[0]->buildInitialPointSets();
	}
}

void RMorphableMeshGroup::buildMeshByXScanV2(MassGridArray* in_massGridArrayRef, 
											float in_sliceThickness, 
											int in_pointsPerSliceArray, 
											RCollisionPointToPTriangleMapContainer* in_pointToTriangleMapContainerRef,
											float in_cubeDimLength,
											float in_tileDimWeightRatio,
											int in_tilesPerDim
											)
{
	// find the lowest/highest x values, by using min and max
	int minX = 1000;	// should always start high 
	int maxX = 0;		// should always start low


	// print values
	//std::cout << ":::: buildMeshByXScan, minX: " << minX << std::endl;
	//std::cout << ":::: buildMeshByXScan, maxX: " << maxX << std::endl;
	auto keyedMorphablesBegin = keyedMorphables.begin();
	auto keyedMorphablesEnd = keyedMorphables.end();
	for (; keyedMorphablesBegin != keyedMorphablesEnd; keyedMorphablesBegin++)
	{
		minX = std::min(minX, keyedMorphablesBegin->first.x);
		maxX = std::max(maxX, keyedMorphablesBegin->first.x);
	}

	for (int currentXSliceIndex = minX; currentXSliceIndex < maxX + 1; currentXSliceIndex++)	// maxX + 1 because we must include  maxX.
	{
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> currentXSliceIndexSet;
		auto currentKeyedScanForXBegin = keyedMorphables.begin();
		auto currentKeyedScanForXEnd = keyedMorphables.end();
		for (; currentKeyedScanForXBegin != currentKeyedScanForXEnd; currentKeyedScanForXBegin++)
		{
			if (currentKeyedScanForXBegin->first.x == currentXSliceIndex)
			{
				currentXSliceIndexSet.insert(currentKeyedScanForXBegin->first);
			}
		}

		//sliceMap[currentXSliceIndex].reset(new RAdditiveXSlice());

		
		// run "suction" on each morphable mesh.
		auto xSliceSetBegin = currentXSliceIndexSet.begin();
		auto xSliceSetEnd = currentXSliceIndexSet.end();
		for (; xSliceSetBegin != xSliceSetEnd; xSliceSetBegin++)
		{
			PolyLogger currentKeySuctionLogger;
			currentKeySuctionLogger.setDebugLevel(rMorphableMeshDebugOptions[*xSliceSetBegin].returnDebugLevelIfFound(DebugOption::RMORPHABLEMESHGROUP_SPECIFIC_MESH_SUCTION));
			if (keyedMorphables[*xSliceSetBegin].getMeshState() == RMorphableMeshState::EXPOSED)
			{
				//std::cout << "Running suction for mesh at key: " << xSliceSetBegin->x << ", " << xSliceSetBegin->y << ", " << xSliceSetBegin->z << std::endl;
				currentKeySuctionLogger.log("(RMorphableMeshGroup): Running suction for mesh at key: ", xSliceSetBegin->x, ", ", xSliceSetBegin->y, ", ", xSliceSetBegin->z, "\n");		

				
				keyedMorphables[*xSliceSetBegin].runSuctionByXSlice(in_cubeDimLength,
						in_tileDimWeightRatio,
						in_tilesPerDim,
						in_massGridArrayRef,
						&currentKeySuctionLogger);				
					
			}
			else
			{
				//std::cout << "!! Mesh with key " << xSliceSetBegin->x << ", " << xSliceSetBegin->y << ", " << xSliceSetBegin->z << "is landlocked; will not run suction. " << std::endl;
				currentKeySuctionLogger.log("(RMorphableMeshGroup): !! Mesh with key ", xSliceSetBegin->x, ", ", xSliceSetBegin->y, ", ", xSliceSetBegin->z, "is landlocked; will not run suction. \n");
			}
			//int suctionWait = 3;
			//std::cin >> suctionWait;
			currentKeySuctionLogger.waitForDebugInput();
		}
			

	}
}

void RMorphableMeshGroup::produceSolution(MassGridArray* in_massGridArrayRef,
	float in_tileGridWidth,
	int in_numberOfTilesPerDimension)
{
	int faceCount = 0;
	auto faceScanBegin = keyedMorphables.begin();
	auto faceScanEnd = keyedMorphables.end();
	for (; faceScanBegin != faceScanEnd; faceScanBegin++)
	{
		//EnclaveKeyDef::EnclaveKey currentKey = faceScanBegin->first;
		//if (currentKey.equals(2, 0, 0))
		//{

			if (faceScanBegin->second.getMeshState() == RMorphableMeshState::EXPOSED)
			{
				faceScanBegin->second.insertPTrianglesIntoMesh(&groupMesh);
			}
		//}
	}

	groupMesh.rebuildPTriangleNormals();
	//groupMesh.calculatePTriangleMaterials();
	groupMesh.samplePTriangleMaterials(in_massGridArrayRef, in_tileGridWidth, in_numberOfTilesPerDimension);

	std::cout << "!!!!######################### Number of PTriangles in this solution: " << groupMesh.getMeshPTriangleCount() << std::endl;
}

void RMorphableMeshGroup::calibratePTriangleMeshPoints(RPointToGridTranslator* in_translatorRef, MassZoneBoxType in_boxTypeForCalibrator)
{
	PTriangleMeshPointCalibrator calibrator(&meshGroupPointArray, in_translatorRef, in_boxTypeForCalibrator);
	groupMesh.rebuildPTriangleNormals();	// should always rebuild normals after any modification to points.
}

void RMorphableMeshGroup::determineGroupMeshBoundaries(MassZoneBoxType in_boundaryType)
{
	int numberOfDiscoveredBoundaryPTriangles = groupMesh.determinePTriangleBoundaries(in_boundaryType);
	std::cout << ">>> Number of PTriangles flagged as being on boundary: " << numberOfDiscoveredBoundaryPTriangles << std::endl;
}