#pragma once

#ifndef MAPIOBJECTMETADATA_H
#define MAPIOBJECTMETADATA_H

#include "MAPIObjectType.h"
#include <string>

/*
* 
* Description: this class simply stores a MAPIObjectType and std::string that represents
* the name of the type of object to search for.

*/

class MAPIObjectMetadata
{
	public:
		MAPIObjectMetadata() {};

		// Use when simply searching for strings 
		MAPIObjectMetadata(MAPIObjectType in_mdType, 
						  std::string in_mdName):
			mdType(in_mdType),
			mdName(in_mdName)
		{ 
			mdMapKeyType = MAPIObjectMapKeyType::STRING_KEYTYPE;
		}

		// Use when searching for a 3d key in a specific named map
		MAPIObjectMetadata(MAPIObjectType in_mdType, 
						  EnclaveKeyDef::EnclaveKey in_keyToSearchFor,
						  std::string in_keyToSearchMapName) :
			mdType(in_mdType),
			mdKeyValue(in_keyToSearchFor),
			mdKeyMapName(in_keyToSearchMapName)
		{
			mdMapKeyType = MAPIObjectMapKeyType::ENCLAVE_KEYTYPE;
		}

		MAPIObjectMapKeyType fetchMdMapKeyType() { return mdMapKeyType;  }
		MAPIObjectType fetchMdObjectType() { return mdType; }
		std::string fetchMdName() { return mdName; }
		EnclaveKeyDef::EnclaveKey fetchMdKeyValue() { return mdKeyValue; }
		std::string fetchMdKeyMapName() { return mdKeyMapName; }

	private:
		MAPIObjectMapKeyType mdMapKeyType = MAPIObjectMapKeyType::UNSET_KEYTYPE;	// determines the kind of container/map
																					// that this data should go into (i.e, a string-keyed map, or a 3d-keyed map)
		MAPIObjectType mdType = MAPIObjectType::UNSET;	// is it a buffer, etc
		std::string mdName = "";	// used when the data is meant to be used with MAPIObjectMapKeyType::STRING_KEYTYPE.

		// The below members are relevant only when mdMapKeyType is
		// MAPIObjectMapKeyType::ENCLAVE_KEYTYPE
		EnclaveKeyDef::EnclaveKey mdKeyValue;
		std::string mdKeyMapName = "";
};

#endif
