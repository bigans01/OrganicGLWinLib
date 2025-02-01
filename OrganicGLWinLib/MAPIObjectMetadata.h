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
		MAPIObjectMetadata(MAPIObjectType in_mdType, std::string in_mdName):
			mdType(in_mdType),
			mdName(in_mdName)
		{ }
		MAPIObjectType mdType = MAPIObjectType::UNSET;
		std::string mdName = "";
};

#endif
