#pragma once

#ifndef MAPIObjectRequest_H
#define MAPIObjectRequest_H

#include "MAPIObjectType.h"
#include "MAPIObjectMapKeyType.h"
#include <string>

/*
* 
* Description: This class contains information for a desired API resource object that an MShader-derived class needs; each MShader child
* has a vector of this type, in the bindingRequests member. This member should be utilized during shader setup, and can be fetched with a call to 
* MShader::fetchMShaderBindingRequests. They can then be processed individually, in an attempt to generate each API object, or ignore the request
* if it already exists.

*/

class MAPIObjectRequest
{
	public:
		MAPIObjectRequest() {};	// default constructor required for copy into a vector, etc

		// Below: constructor for string types.
		MAPIObjectRequest(MAPIObjectType in_requestBindingType, 
						  std::string in_bindingName) :	
			requestBindingType(in_requestBindingType),
			requestBindingString(in_bindingName)
		{
			requestMapKeyType = MAPIObjectMapKeyType::STRING_KEYTYPE;
		};

		// Below: constructor for 3d key types.
		MAPIObjectRequest(MAPIObjectType in_requestBindingType,
			EnclaveKeyDef::EnclaveKey in_keyEnclaveValue,
			std::string in_requestBindingKeyMapName) :
				requestBindingType(in_requestBindingType),
				requestBindingKey(in_keyEnclaveValue),
				requestBindingKeyMapName(in_requestBindingKeyMapName)
		{
			requestMapKeyType = MAPIObjectMapKeyType::ENCLAVE_KEYTYPE;
		}


		MAPIObjectMapKeyType getBindingMapKeyType() { return requestMapKeyType;  }
		MAPIObjectType getBindingRequestType() { return requestBindingType;  }
		std::string getBindingRequestName() { return requestBindingString; }

	private:
		MAPIObjectMapKeyType requestMapKeyType = MAPIObjectMapKeyType::UNSET_KEYTYPE;
		MAPIObjectType requestBindingType = MAPIObjectType::UNSET;	// default value is UNSET; determines if the binding type is for a texture, buffer, VAO, etc
		// must be set by non-default constructor.

		std::string requestBindingString = "";				// when the key value type is string, the string is stored here.

		EnclaveKeyDef::EnclaveKey requestBindingKey;		// when the key value is a 3d key, that key value is stored here.
		std::string requestBindingKeyMapName = "";			// when the key value is a 3d key, this specifies the name of the 3d key map
};

#endif
