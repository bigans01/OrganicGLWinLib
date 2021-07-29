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
			std::cout << "!! RMorphableMesh identified by key (" << removalScanBegin->first.x << ", " << removalScanBegin->first.y << ", " << removalScanBegin->first.z << ")" << std::endl;
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

void RMorphableMeshGroup::generatePointArray()
{
	int maxPointSize = int(keyedMorphables.size()) * 8;	// 8 points per morphable
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

void RMorphableMeshGroup::updatePointLandlockStats()
{
	auto keyedMorphablesBegin = keyedMorphables.begin();
	auto keyedMorphablesEnd = keyedMorphables.end();
	for (; keyedMorphablesBegin != keyedMorphablesEnd; keyedMorphablesBegin++)
	{
		keyedMorphablesBegin->second.updatePointUsageCounts();
	}
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
		generationRunBegin->second.generateRProductFaces();
	}
}

void RMorphableMeshGroup::buildMeshByXScan(MassGridArray* in_massGridArrayRef, float in_sliceThickness)
{
	// find the lowest/highest x values, by using min and max
	int minX = 1000;	// should always start high 
	int maxX = 0;		// should always start low
	auto keyedMorphablesBegin = keyedMorphables.begin();
	auto keyedMorphablesEnd = keyedMorphables.end();
	for (; keyedMorphablesBegin != keyedMorphablesEnd; keyedMorphablesBegin++)
	{
		minX = std::min(minX, keyedMorphablesBegin->first.x);
		maxX = std::max(maxX, keyedMorphablesBegin->first.x);
	}

	// print values
	std::cout << ":::: buildMeshByXScan, minX: " << minX << std::endl;
	std::cout << ":::: buildMeshByXScan, maxX: " << maxX << std::endl;

	// for when there is more than one slice
	if (minX != maxX)
	{
		// iterate from min to max; slicing goes from west (x = 0) towards east (x > 0)
		for (int currentXSlice = minX; currentXSlice < maxX + 1; currentXSlice++)	// maxX + 1 because we must include  maxX.
		{
			std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> currentXSliceSet;
			auto currentKeyedScanForXBegin = keyedMorphables.begin();
			auto currentKeyedScanForXEnd = keyedMorphables.end();
			for (; currentKeyedScanForXBegin != currentKeyedScanForXEnd; currentKeyedScanForXBegin++)
			{
				if (currentKeyedScanForXBegin->first.x == currentXSlice)
				{
					currentXSliceSet.insert(currentKeyedScanForXBegin->first);
				}
			}

			// west slice goes first:
			if (currentXSlice == minX)
			{
				sliceMap[currentXSlice].reset(new RAdditiveXSliceWestEnd());
				sliceMap[currentXSlice]->initialize(RAdditiveSliceType::X_SLICE_WEST_END, 
													8, 
													in_massGridArrayRef, 
													&meshGroupPointArray, 
													in_sliceThickness);
				sliceMap[currentXSlice]->buildPointSets();
			}

			// east slice goes last:
			else if (currentXSlice == maxX)
			{
				sliceMap[currentXSlice].reset(new RAdditiveXSliceEastEnd());
				sliceMap[currentXSlice]->initialize(RAdditiveSliceType::X_SLICE_EAST_END, 
													8, 
													in_massGridArrayRef, 
													&meshGroupPointArray,
													in_sliceThickness);
				sliceMap[currentXSlice]->buildPointSets();
			}

			// all other slices
			else
			{
				sliceMap[currentXSlice].reset(new RAdditiveXSlice());
				sliceMap[currentXSlice]->initialize(RAdditiveSliceType::X_SLICE, 
													8, 
													in_massGridArrayRef, 
													&meshGroupPointArray, 
													in_sliceThickness);
				sliceMap[currentXSlice]->buildPointSets();
			}
			// set the RAdditiveSliceBase(s)

			/*
			// run "suction" on each morphable mesh.
			auto xSliceSetBegin = currentXSliceSet.begin();
			auto xSliceSetEnd = currentXSliceSet.end();
			for (; xSliceSetBegin != xSliceSetEnd; xSliceSetBegin++)
			{
				std::cout << "Running suction for mesh at key: " << xSliceSetBegin->x << ", " << xSliceSetBegin->y << ", " << xSliceSetBegin->z << std::endl;
				keyedMorphables[*xSliceSetBegin].runSuctionByXSlice();

				int suctionWait = 3;
				std::cin >> suctionWait;
			}
			*/

			// do the following for all but the first slice; that current slice must inherit the point B set from the previous slice.
			if (currentXSlice != minX)
			{
				RAdditiveSliceBase* previousSliceRef = sliceMap[currentXSlice - 1].get();
				sliceMap[currentXSlice]->copySetBRefsFromPreviousSlice(previousSliceRef);
			}
		}
	}

	// there is only one slice.
	else if (minX == maxX)
	{
		sliceMap[0].reset(new RAdditiveSliceStandalone());
		sliceMap[0]->initialize(RAdditiveSliceType::X_SLICE_STANDALONE, 
								8, 
								in_massGridArrayRef, 
								&meshGroupPointArray,
								in_sliceThickness);
		sliceMap[0]->buildPointSets();
	}
}