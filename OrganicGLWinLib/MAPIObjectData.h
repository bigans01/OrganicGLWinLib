#pragma once

#ifndef MAPIObjectData_H
#define MAPIObjectData_H

#include "MAPIObjectType.h"
#include <string>
#include "Message.h"

/*
* 
* Description: an MAPIObjectData is meant to serve as a piece of data for a resource that has been generated in OpenGL / GUI APIs, such
* as buffers, VAOS, framebuffers, textures, and anything that is allocated in memory. Each MAPIObjectData must have an associated MAPIObjectType, so
* it must be set in the non-default constructor. 
* 
* The bindingName is the unique binding name; it must hold this property for the map/container it will be stored in (i.e, unique name for the map 
* that contains MAPIObjectData objects of the MAPIObjectType::BUFFER)
* 
* The bindingMessage should contain any metadata, such as the buffer ID or texture ID associated with the binding; it is an optional piece of
* data that isn't mandatory


*/

class MAPIObjectData
{
	public:
		MAPIObjectData() {};	// default constructor, used mainly for maps.
		MAPIObjectData(MAPIObjectType in_bindingType, std::string in_bindingName, Message in_bindingMessage) :
			bindingType(in_bindingType),
			bindingName(in_bindingName),
			bindingMessage(in_bindingMessage)
		{};

		MAPIObjectType getBindingType() { return bindingType; }
		std::string getBindingName() { return bindingName; }
		Message getBindingMessage() { return bindingMessage; }

	private:
		MAPIObjectType bindingType = MAPIObjectType::UNSET;	// default value is UNSET; determines if the binding type is for a texture, buffer, VAO, etc
														// must be set by non-default constructor.

		std::string bindingName = "";	// the required name of the binding
		Message bindingMessage;			// the Message that contains metadata about the binding.
};

#endif
