#pragma once

#ifndef DYNAMICBUFFERMANAGER_H
#define DYNAMICBUFFERMANAGER_H

#include "SmartIntMap.h"
#include <unordered_map>
#include <iostream>
#include "OrganicGLWinUtils.h"
#include "OperableIntSet.h"

class DynamicBufferManager
{
	public:
		GLuint attemptCreateOfDynamicBufferForGear(std::string in_string, int in_programGearID);
		void attemptDeleteOfDynamicBuffer(std::string in_string);
		std::unordered_map<std::string, OperableIntSet> getBufferDestinationGears();
	private:
		std::unordered_map<std::string, GLuint> dynamicBufferLookup;
		SmartIntMap<GLuint> bufferMap;
		std::unordered_map<std::string, OperableIntSet> bufferDestinationGears;
};

#endif
