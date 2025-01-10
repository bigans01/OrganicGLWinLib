#pragma once

#ifndef MAPIObjectRequest_H
#define MAPIObjectRequest_H

#include "MAPIObjectType.h"
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

		// Below: the typical constructor that should be used 
		MAPIObjectRequest(MAPIObjectType in_bindingType, std::string in_bindingName) :	
			bindingType(in_bindingType),
			bindingName(in_bindingName)
		{};

		MAPIObjectType getBindingRequestType() { return bindingType;  }
		std::string getBindingRequestName() { return bindingName; }

	private:
		MAPIObjectType bindingType = MAPIObjectType::UNSET;	// default value is UNSET; determines if the binding type is for a texture, buffer, VAO, etc
		// must be set by non-default constructor.

		std::string bindingName = "";	// the required name of the binding
};

#endif
