#include "stdafx.h"
#include "RMorphableAreaScanner.h"

void RMorphableAreaScanner::addSPolyToGrid(SPoly in_sPolyToAdd)
{
	massGrid.addGridRPoly(in_sPolyToAdd);
}

void RMorphableAreaScanner::buildGridMassShell()
{
	massGrid.buildShell();
}

void RMorphableAreaScanner::fillGridMass()
{
	massGrid.fillMass();
}

void RMorphableAreaScanner::scanGridMass()
{
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
				RMorphableMesh currentCandidateMesh(2, currentMeshCenter, morphableMeshDimension);

				
				RMorphableMeshCubeAreaDefiner currentMeshAreaScanner = currentCandidateMesh.getAreaDefiner();
				EnclaveKeyDef::EnclaveKey convertedPointA = massGrid.convertSPolyPointToBlockCoord(currentMeshAreaScanner.definingPointA);
				EnclaveKeyDef::EnclaveKey convertedPointB = massGrid.convertSPolyPointToBlockCoord(currentMeshAreaScanner.definingPointB);
				MassGridArrayCellScanArea scanArea(convertedPointA, convertedPointB);

				
				bool wasMassFound = massGrid.wasMassFoundInAreaScan(scanArea);
				if (wasMassFound == true)
				{
					std::cout << "!! Notice: morphable mesh at mesh coord " << x << ", " << y << ", " << z << " contains mass. " << std::endl;
					EnclaveKeyDef::EnclaveKey currentMeshKey(x, y, z);
					ungroupedMeshes[currentMeshKey] = currentCandidateMesh;
				}
				
			}
		}
	}

	// Phase 2: run the code for building each group of keys.
	if (!ungroupedMeshes.empty())
	{
		std::cout << "current size of ungrouped mesh, before move: " << ungroupedMeshes.size() << std::endl;

		// move the very first ungrouped mesh into the candidateMesh map; because we must start somewhere.
		auto firstUngroupedMesh = ungroupedMeshes.begin();
		EnclaveKeyDef::EnclaveKey firstKey = firstUngroupedMesh->first;
		currentMeshGroup[firstKey] = std::move(firstUngroupedMesh->second);
		ungroupedMeshes.erase(firstKey);

		std::cout << "current size of ungrouped mesh, after move: " << ungroupedMeshes.size() << std::endl;
		std::cout << "current size of currentMeshGroup: " << currentMeshGroup.size() << std::endl;

		int remainingMeshesToHandle = ungroupedMeshes.size();
		int currentMeshGroupID = 0;
		for (int x = 0; x < remainingMeshesToHandle; x++)
		{
			bool wasMeshToStickToFound = false;

			// compare each mesh to all meshes in the currentMeshGroup; 
			EnclaveKeyDef::EnclaveKey foundKey;
			auto remainingMeshesBegin = ungroupedMeshes.begin();
			auto remainingMeshesEnd = ungroupedMeshes.end();
			for (; remainingMeshesBegin != remainingMeshesEnd; remainingMeshesBegin++)
			{
				auto currentMeshGroupMeshesBegin = currentMeshGroup.begin();
				auto currentMeshGroupMeshesEnd = currentMeshGroup.end();
				for (; currentMeshGroupMeshesBegin != currentMeshGroupMeshesEnd; currentMeshGroupMeshesBegin++)
				{
					EnclaveKeyDef::EnclaveKey candidateKey = remainingMeshesBegin->first;
					EnclaveKeyDef::EnclaveKey currentMeshGroupMeshKey = currentMeshGroupMeshesBegin->first;
					
					std::cout << "Comparing keys: keyA (" << candidateKey.x << ", " << candidateKey.y << ", " << candidateKey.z
						<< ") | keyB: (" << currentMeshGroupMeshKey.x << ", " << currentMeshGroupMeshKey.y << ", " << currentMeshGroupMeshKey.z << std::endl;

					bool neighborsFound = checkIfKeysAreNeighbors(candidateKey, currentMeshGroupMeshKey);
					if (neighborsFound == true)
					{
						foundKey = candidateKey;
						wasMeshToStickToFound = true;
						std::cout << ">>>> Mesh to stick was found; flagging for move. Key (" << foundKey.x << ", " << foundKey.y << ", " << foundKey.z << ") " << std::endl;
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
				std::cout << "!! No stick found. " << std::endl;
				meshGroupMap[currentMeshGroupID++].keyedMorphables = currentMeshGroup;		// move over the current mesh group, because it didn't match to anything
				currentMeshGroup.clear();													// clear it out

				auto nextMesh = ungroupedMeshes.begin();									// grab the first mesh in the remaining candidate pool; put it as the start of the new group.
				currentMeshGroup[nextMesh->first] = nextMesh->second;
					
				ungroupedMeshes.erase(nextMesh->first);				// erase the mesh we copied over fromt he ungroupedMesh

			}
		}
		meshGroupMap[currentMeshGroupID++].keyedMorphables = currentMeshGroup;	// move over the final group
	}
	

	auto meshGroupsBegin = meshGroupMap.begin();
	auto meshGroupsEnd = meshGroupMap.end();
	for (; meshGroupsBegin != meshGroupsEnd; meshGroupsBegin++)
	{
		std::cout << "Printing out contents for mesh group with ID " << meshGroupsBegin->first << ": " << std::endl;
		auto meshGroupEntriesBegin = meshGroupsBegin->second.keyedMorphables.begin();
		auto meshGroupEntriesEnd = meshGroupsBegin->second.keyedMorphables.end();
		for (; meshGroupEntriesBegin != meshGroupEntriesEnd; meshGroupEntriesBegin++)
		{
			std::cout << "Mesh with key (" << meshGroupEntriesBegin->first.x << ", " << meshGroupEntriesBegin->first.y << ", " << meshGroupEntriesBegin->first.z << ") " << std::endl;
		}
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