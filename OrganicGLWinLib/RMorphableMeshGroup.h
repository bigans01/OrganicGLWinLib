#pragma once

#ifndef RMORPHABLEMESHGROUP_H
#define RMORPHABLEMESHGROUP_H

#include "EnclaveKeyDef.h"
#include "RMorphableMesh.h"
#include <unordered_map>
#include "RCollisionPointArray.h"
#include "RCollisionPoint.h"

class RMorphableMeshGroup
{
	public:
		void insertMeshIntoGroup(EnclaveKeyDef::EnclaveKey in_meshKey, RMorphableMesh in_mesh);
		std::unordered_map<EnclaveKeyDef::EnclaveKey, RMorphableMesh, EnclaveKeyDef::KeyHasher> keyedMorphables;
	private:
		RCollisionPointArray meshGroupPointArray;
};

#endif
