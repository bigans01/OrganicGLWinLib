#pragma once

#ifndef MAPIObjectData_H
#define MAPIObjectData_H

#include "MAPIObjectType.h"
#include <string>
#include "Message.h"
#include "MAPIObjectMapKeyType.h"

/*
* 
* Description: an MAPIObjectData is meant to serve as a piece of data for a resource that has been generated in OpenGL / GUI APIs, such
* as buffers, VAOS, framebuffers, textures, and anything that is allocated in memory. Each MAPIObjectData must have an associated MAPIObjectType, so
* it must be set in the non-default constructor. 
* 
* The dataBindingName is the unique binding name; it must hold this property for the map/container it will be stored in (i.e, unique name for the map 
* that contains MAPIObjectData objects of the MAPIObjectType::BUFFER)
* 
* The dataBindingMessage should contain any metadata, such as the buffer ID or texture ID associated with the binding; it is an optional piece of
* data that isn't mandatory


*/

class MAPIObjectData
{
	public:
		MAPIObjectData() {};	// default constructor, used mainly for maps.

		// Below: constructor for string types.
		MAPIObjectData(MAPIObjectType in_dataBindingType, 
					   std::string in_dataBindingName, 
			           Message in_dataBindingMessage) :
			dataBindingType(in_dataBindingType),
			dataBindingName(in_dataBindingName),
			dataBindingMessage(in_dataBindingMessage)
		{
			dataMapKeyType = MAPIObjectMapKeyType::STRING_KEYTYPE;
		};

		// Below: constructor for 3d key types.
		MAPIObjectData(MAPIObjectType in_dataBindingType,
			          EnclaveKeyDef::EnclaveKey in_keyEnclaveValue,
			          std::string in_requestBindingKeyMapName,
			          Message in_dataBindingMessage) :
			dataBindingType(in_dataBindingType),
			dataBindingKey(in_keyEnclaveValue),
			dataBindingKeyMapName(in_requestBindingKeyMapName),
			dataBindingMessage(in_dataBindingMessage)
		{
			dataMapKeyType = MAPIObjectMapKeyType::ENCLAVE_KEYTYPE;
		};



		MAPIObjectMapKeyType getDataMapKeyType() { return dataMapKeyType; } 
		MAPIObjectType getDataBindingType() { return dataBindingType; }
		std::string getDataBindingName() { return dataBindingName; }
		Message getDataBindingMessage() { return dataBindingMessage; }

	private:
		MAPIObjectMapKeyType dataMapKeyType = MAPIObjectMapKeyType::UNSET_KEYTYPE;
		MAPIObjectType dataBindingType = MAPIObjectType::UNSET;	// default value is UNSET; determines if the binding type is for a texture, buffer, VAO, etc
														// must be set by non-default constructor.

		std::string dataBindingName = "";	// the required name of the binding
		Message dataBindingMessage;			// the Message that contains metadata about the binding.

		EnclaveKeyDef::EnclaveKey dataBindingKey;		// when the key value is a 3d key, that key value is stored here.
		std::string dataBindingKeyMapName = "";			// when the key value is a 3d key, this specifies the name of the 3d key map
};

#endif
