#pragma once

#ifndef LOOKUPBYDIMREGISTER_H
#define LOOKUPBYDIMREGISTER_H

#include <unordered_set>
#include "EnclaveKeyDef.h"

class LookupByDimRegister
{
	public:
		LookupByDimRegister() {};
		LookupByDimRegister(int in_dimValue) :
			dimValue(in_dimValue)
		{};
		void insertLineSetRef(int in_lineID,
							  std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher>* in_lineSetRef)
		{
			lineSetRefs[in_lineID] = in_lineSetRef;
		}
	private:
		int dimValue = 0;	// x,y,z or coordinate value that represents the slice to scan. 
		                    // i.e, dimValue = 3 could mean:
							// scan y and z at x = 3
							// scan x and z at y = 3
							// scan x and y at z = 3						
		std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher>* lineSetRefs[3];
};

#endif