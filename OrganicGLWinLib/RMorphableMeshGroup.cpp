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

void RMorphableMeshGroup::removeInteriorLandlockedMeshes()
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
		keyedMorphables.erase(*keysToRemoveBegin);
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
		for (int x = 0; x < 8; x++)
		{
			ECBPolyPoint glmConverted(pointsToSearch.points[x].x, 
									pointsToSearch.points[x].y, 
									pointsToSearch.points[x].z);
			ECBPPOrientationResults currentPointOrientation = IndependentUtils::getDynamicPointOrientation(glmConverted, dynamicBorderRef);
			RCollisionPoint* currentIndexPointRef = meshGroupPointArray.attemptPointInsert(pointsToSearch.points[x], currentPointOrientation);
		}
	}
}
