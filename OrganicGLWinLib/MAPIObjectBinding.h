#pragma once

#ifndef MAPIOBJECTBINDING_H
#define MAPIOBJECTBINDING_H

#include "MAPIObjectType.h"
#include "MAPIObjectMapKeyType.h"
#include <string>
#include <iostream>

/*

Description: This class is designed to store the binding value, associated with the data that defines the binding.
The non-default constructors of this class are utilized by MAPIObjectManager::fetchAllBindings, which creates a vector
of objects of this class type. The intent of this class is to have a single class that encapsulates all the data needed 
when individual MShader objects need to scan for available objects that are in play/use; objects of this class should be able 
to store all that data.

*/

class MAPIObjectBinding
{
	public:
		MAPIObjectBinding() {};

		// Below: constructor for string types
		MAPIObjectBinding(MAPIObjectType in_requestBindingType,
						std::string in_bindingName,
						int in_bindingValue):
			bindingType(in_requestBindingType),
			bindingString(in_bindingName),
			bindingValue(in_bindingValue)
		{
			bindingMapKeyType = MAPIObjectMapKeyType::STRING_KEYTYPE;
		};

		// Below: constructor for 3d key types.
		MAPIObjectBinding(MAPIObjectType in_requestBindingType,
						EnclaveKeyDef::EnclaveKey in_keyEnclaveValue,
						std::string in_requestBindingKeyMapName,
						int in_bindingValue) :
			bindingType(in_requestBindingType),
			bindingMapKey(in_keyEnclaveValue),
			bindingKeyMapName(in_requestBindingKeyMapName),
			bindingValue(in_bindingValue)
		{
			bindingMapKeyType = MAPIObjectMapKeyType::ENCLAVE_KEYTYPE;
		}

		MAPIObjectMapKeyType getBindingMapKeyType() { return bindingMapKeyType; }
		MAPIObjectType getBindingRequestType() { return bindingType; }

		// Below: This is used for string types. See usage in MAPIObjectManager::handleMAPIObjectRequest.
		std::string getBindingRequestName() { return bindingString; }

		// Below: These are for 3d key types.
		std::string getBindingRequestKeyMapName() { return bindingKeyMapName; }
		EnclaveKeyDef::EnclaveKey getBindingEnclaveKey() { return bindingMapKey; }

		int getBindingValue() { return bindingValue; }

		void printBindingData()
		{
			switch (bindingMapKeyType)
			{
				case MAPIObjectMapKeyType::STRING_KEYTYPE:
				{
					std::cout << "Binding map key type: STRING | ";
					std::cout << "String value: " << bindingString << " | ";
					std::cout << "Binding value: " << bindingValue << std::endl;

					break;
				}

				case MAPIObjectMapKeyType::ENCLAVE_KEYTYPE:
				{
					std::cout << "Binding map key type: ENCLAVE_KEY | ";
					std::cout << "Enclave Key named map value: " << bindingKeyMapName << " | ";
					std::cout << "Enclave Key value: ";
					bindingMapKey.printKey();
					std::cout << " | ";
					std::cout << "Binding value: " << bindingValue << std::endl;

					break;
				}
			}
		}

	private:
		MAPIObjectMapKeyType bindingMapKeyType = MAPIObjectMapKeyType::UNSET_KEYTYPE;
		MAPIObjectType bindingType = MAPIObjectType::UNSET;	// default value is UNSET; determines if the binding type is for a texture, buffer, VAO, etc
		// must be set by non-default constructor.

		std::string bindingString = "";				// when the key value type is string, the string is stored here.

		EnclaveKeyDef::EnclaveKey bindingMapKey;		// when the key value is a 3d key, that key value is stored here.
		std::string bindingKeyMapName = "";			// when the key value is a 3d key, this specifies the name of the 3d key map

		int bindingValue = 0;
};

#endif