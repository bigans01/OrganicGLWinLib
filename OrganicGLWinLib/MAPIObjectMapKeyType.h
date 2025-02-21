#pragma once

#ifndef MAPIOBJECTMAPKEYTYPE_H
#define MAPIOBJECTMAPKEYTYPE_H

/*
* Description: This class is used to describe the type of map that a MAPIObject should be inserted/put into.
* For example, ENCLAVE_KEYTYPE indicates that it should go into a std::unordered_map<EnclaveKeyDef::EnclaveKey, int, EnclaveKeyDef::KeyHasher>
* object.
* 

*/

enum class MAPIObjectMapKeyType
{
	UNSET_KEYTYPE,		// default value
	ENCLAVE_KEYTYPE,
	STRING_KEYTYPE
};

#endif
