#pragma once

#ifndef DYNAMICBUFFERMANAGER_H
#define DYNAMICBUFFERMANAGER_H

#include "SmartIntMap.h"
#include <unordered_map>
#include <iostream>
#include "OrganicGLWinUtils.h"
#include "OperableIntSet.h"
#include "GLMultiDrawArrayJob.h"

class DynamicBufferManager
{
	public:
		GLuint attemptCreateOfDynamicBufferForGear(std::string in_string, int in_programGearID);
		void attemptDeleteOfDynamicBuffer(std::string in_string);
		void insertDataIntoDynBuffer(std::string in_bufferName, int in_byteSizeToWrite, GLfloat* in_dataArray);
		void createDynamicMultiDrawArrayJob(std::string in_bufferName, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);
		GLMultiDrawArrayJob fetchDynamicMultiDrawArrayJob(std::string in_bufferName);
		OperableIntSet getBufferDestinationGears(std::string in_bufferName);
	private:
		// buffer related members
		std::unordered_map<std::string, GLuint> dynamicBufferLookup;
		SmartIntMap<GLuint> bufferMap;
		std::unordered_map<std::string, OperableIntSet> bufferDestinationGears;

		// draw job related members
		std::unordered_map<std::string, int> multiDrawArrayJobLookup;
		SmartIntMap<GLMultiDrawArrayJob> multiDrawArrayJobMap;
};

#endif
