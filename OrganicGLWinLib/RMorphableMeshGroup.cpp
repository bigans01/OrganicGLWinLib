#include "stdafx.h"
#include "RMorphableMeshGroup.h"

void RMorphableMeshGroup::insertMeshIntoGroup(EnclaveKeyDef::EnclaveKey in_meshKey, RMorphableMesh in_mesh)
{
	keyedMorphables[in_meshKey] = in_mesh;
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
			//pointsToSearch.
		}
	}
}

void RMorphableMeshGroup::hollowOutInterior()
{

}