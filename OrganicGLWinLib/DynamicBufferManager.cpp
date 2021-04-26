#include "stdafx.h"
#include "DynamicBufferManager.h"

GLuint DynamicBufferManager::attemptCreateOfDynamicBufferForGear(std::string in_string, int in_programGearID)
{
	GLuint bufferID = 0;	// the ID of the generated buffer
	auto existingStringFinder = dynamicBufferLookup.find(in_string);
	if (existingStringFinder == dynamicBufferLookup.end())	// it wasn't found, create it
	{
		// create the new buffer, call gen buffers on it.
		GLuint indexToUse = bufferMap.getNextAvailableKey();
		bufferMap[indexToUse] = 0;
		OrganicGLWinUtils::createBuffer(&bufferMap[indexToUse]);

		// insert the lookup value into the lookup table map.
		dynamicBufferLookup[in_string] = indexToUse;
		bufferID = bufferMap[indexToUse];	// get the buffer ID at the index with value of indexToUse
	}
	else if (existingStringFinder != dynamicBufferLookup.end())		// it exists; just return the existing buffer ID.
	{
		bufferID = dynamicBufferLookup[in_string];
	}

	// before returning, be sure to update the set of gear IDs that the program was sent to.
	bufferDestinationGears[in_string] += in_programGearID;

	return bufferID;
}

void DynamicBufferManager::attemptDeleteOfDynamicBuffer(std::string in_string)
{
	auto existingStringFinder = dynamicBufferLookup.find(in_string);
	if (existingStringFinder != dynamicBufferLookup.end())	// it was found; lets delete it.
	{
		//OrganicGLWinUtils::deleteBuffer(dynamicBufferLookup[in_string]);
		int bufferIDToRetrieve = dynamicBufferLookup[in_string];
		//int bufferID = bufferMap[bufferIDToRetrieve];
		//OrganicGLWinUtils::deleteBuffer(bufferID);

		OrganicGLWinUtils::deleteBuffer(&bufferMap[bufferIDToRetrieve]);
		dynamicBufferLookup.erase(in_string);
	}
}

void DynamicBufferManager::insertDataIntoDynBuffer(std::string in_bufferName, int in_byteSizeToWrite, GLfloat* in_dataArray)
{
	auto existingStringFinder = dynamicBufferLookup.find(in_bufferName);
	if (existingStringFinder != dynamicBufferLookup.end())	// it was found; lets delete it.
	{
		int bufferMapKey = dynamicBufferLookup[in_bufferName];
		int bufferIDToUse = bufferMap[bufferMapKey];
		glBindBuffer(GL_ARRAY_BUFFER, bufferIDToUse);				// bind to the specified buffer
		//std::cout << "Inserted data into dynamic buffer with ID: " << bufferIDToUse << std::endl;
		//glBufferSubData(GL_ARRAY_BUFFER, in_offset, in_byteSizeToWrite, in_dataArray);		// send the data to the buffer
		//glBufferData(GL_ARRAY_BUFFER, 6 * 7 * sizeof(float), quadData, GL_STATIC_DRAW);		// populate the data
		glBufferData(GL_ARRAY_BUFFER, in_byteSizeToWrite, in_dataArray, GL_STATIC_DRAW);
	}
}

void DynamicBufferManager::createDynamicMultiDrawArrayJob(std::string in_bufferName, GLint* in_startArray, GLsizei* in_vertexCount, int in_drawCount)
{
	// create the multi draw array job.
	GLMultiDrawArrayJob newJob;
	newJob.updateDrawArrayData(0, in_startArray, in_vertexCount, in_drawCount);

	auto existingJobFinder = multiDrawArrayJobLookup.find(in_bufferName);
	if (existingJobFinder == multiDrawArrayJobLookup.end())	// it doesn't exist 
	{
		int indexToUse = multiDrawArrayJobMap.getNextAvailableKey();
		multiDrawArrayJobMap[indexToUse] = newJob;
		multiDrawArrayJobLookup[in_bufferName] = indexToUse;
	}
	else if (existingJobFinder != multiDrawArrayJobLookup.end())
	{
		int indexToUpdate = multiDrawArrayJobLookup[in_bufferName];
		multiDrawArrayJobMap[indexToUpdate] = newJob;
	}
}

GLMultiDrawArrayJob DynamicBufferManager::fetchDynamicMultiDrawArrayJob(std::string in_bufferName)
{
	GLMultiDrawArrayJob returnJob;
	auto existingJobFinder = multiDrawArrayJobLookup.find(in_bufferName);
	if (existingJobFinder != multiDrawArrayJobLookup.end())
	{
		int indexToUse = multiDrawArrayJobLookup[in_bufferName];
		returnJob = multiDrawArrayJobMap[indexToUse];
	}
	return returnJob;
}

OperableIntSet DynamicBufferManager::getBufferDestinationGears(std::string in_bufferName)
{
	return bufferDestinationGears[in_bufferName];
}
