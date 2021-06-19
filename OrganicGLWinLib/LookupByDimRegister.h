#pragma once

#ifndef LOOKUPBYDIMREGISTER_H
#define LOOKUPBYDIMREGISTER_H

#include <unordered_set>
#include "EnclaveKeyDef.h"

class LookupByDimRegister
{
	public:
		void insertLineSetRef(int in_lineID,
							  std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher>* in_lineSetRef)
		{
			lineSetRefs[in_lineID] = in_lineSetRef;
		}
	private:
		std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher>* lineSetRefs[3];
};

#endif