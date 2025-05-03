#pragma once

#ifndef MAPIOBJECTMETADATA_H
#define MAPIOBJECTMETADATA_H

#include "MAPIObjectType.h"
#include "MAPIObjectRequest.h"
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

		// Use when copying data from an instance of MAPIObjectRequest;
		// Make sure the MAPIObjectRequest being used is properly instantiated and set before attempting to use this
		// constructor.
		//
		// Used by MShaderController::insertDynamicBufferData, when needed to build based off
		// a newly inserted MAPIObjectRequest.
		MAPIObjectMetadata(MAPIObjectRequest in_requestToCopy)
		{
			mdType = in_requestToCopy.getBindingRequestType();
			switch (in_requestToCopy.getBindingMapKeyType())
			{
				case MAPIObjectMapKeyType::STRING_KEYTYPE:
				{
					mdMapKeyType = MAPIObjectMapKeyType::STRING_KEYTYPE;
					mdName = in_requestToCopy.getBindingRequestName();
					break;
				}

				case MAPIObjectMapKeyType::ENCLAVE_KEYTYPE:
				{
					mdMapKeyType = MAPIObjectMapKeyType::ENCLAVE_KEYTYPE;
					mdKeyValue = in_requestToCopy.getBindingEnclaveKey();
					mdKeyMapName = in_requestToCopy.getBindingRequestKeyMapName();
					break;
				}
			}
			
		}

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

		std::string buildMetadataInfoString()
		{
			std::string returnString = "";

			std::string objectTypeString = "";
			switch (mdType)
			{
				case MAPIObjectType::UNSET: { objectTypeString = "MAPIObjectType::UNSET"; break; }
				case MAPIObjectType::BUFFER: { objectTypeString = "MAPIObjectType::BUFFER"; break; }
				case MAPIObjectType::TEXTURE: { objectTypeString = "MAPIObjectType::TEXTURE"; break; }
				case MAPIObjectType::VAO: { objectTypeString = "MAPIObjectType::VAO"; break; }
				case MAPIObjectType::FBO: { objectTypeString = "MAPIObjectType::FBO"; break; }
			}

			switch (mdMapKeyType)
			{
				case MAPIObjectMapKeyType::UNSET_KEYTYPE:
				{
					returnString = "No data set for this MAPIObjectMetadata instance.";
					break;
				}

				case MAPIObjectMapKeyType::STRING_KEYTYPE:
				{
					std::string keytypeString = "Map type: MAPIObjectMapKeyType::STRING_KEYTYPE";
					std::string stringedKey = "String key value: " + mdName;
					returnString = "Metadata info: | " + keytypeString + " | Object type: " + objectTypeString + " | " + stringedKey;
					break;
				}

				case MAPIObjectMapKeyType::ENCLAVE_KEYTYPE:
				{
					std::string keytypeString = "Map type: MAPIObjectMapKeyType::ENCLAVE_KEYTYPE";
					std::string key3dValue = "3d key value: " + mdKeyValue.getKeyString();
					std::string keyMapNameString = "3d map name: " + mdKeyMapName;
					returnString = "Metadata info: | " + keytypeString + " | Object type: " + objectTypeString + " | " + key3dValue + " | " + keyMapNameString;
					break;
				}
			}

			return returnString;
		};

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
