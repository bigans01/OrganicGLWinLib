#include "stdafx.h"
#include "Gear.h"

void Gear::useProgram()
{
	glUseProgram(programID);
}

void Gear::insertMultiDrawArrayJob(std::string in_jobName, GLMultiDrawArrayJob in_job)
{
	int currentKey = gearMultiDrawArrayJobMap.size();	// use this if it's a new job...
	int targetKeyValue = currentKey;


	auto doesJobExist = gearMultiDrawArrayJobLookup.find(in_jobName);
	if (doesJobExist != gearMultiDrawArrayJobLookup.end())
	{
		targetKeyValue = doesJobExist->second;
		gearMultiDrawArrayJobMap[targetKeyValue] = in_job;
		//gearMultiDrawArrayJobLookup[in_jobName] = targetKeyValue;
	}
	else
	{
		gearMultiDrawArrayJobMap[targetKeyValue] = in_job;
		gearMultiDrawArrayJobLookup[in_jobName] = targetKeyValue;
	}
	
}

void Gear::insertDrawElementsInstancedJob(std::string in_jobName, GLDrawElementsInstancedJob in_job)
{
	int currentKey = gearDrawElementsInstancedJobMap.size();	// use this if it's a new job...
	int targetKeyValue = currentKey;

	auto doesJobExist = gearDrawElementsInstancedJobLookup.find(in_jobName);
	if (doesJobExist != gearDrawElementsInstancedJobLookup.end())
	{
		targetKeyValue = doesJobExist->second;
		gearDrawElementsInstancedJobMap[targetKeyValue] = in_job;
	}
	else
	{
		gearDrawElementsInstancedJobMap[targetKeyValue] = in_job;
		gearDrawElementsInstancedJobLookup[in_jobName] = targetKeyValue;
	}

}

void Gear::registerNewPersistentBuffer(std::string in_bufferName, GLuint in_bufferID)
{
	int currentKey = gearPersistentBufferMap.size();
	gearPersistentBufferMap[currentKey] = in_bufferID;
	gearPersistentBufferLookup[in_bufferName] = currentKey;
}

void Gear::registerNewFBO(std::string in_fboName, GLuint in_bufferID)
{
	int currentKey = gearFboMap.size();
	gearFboMap[currentKey] = in_bufferID;
	gearFboLookup[in_fboName] = currentKey;
}

void Gear::registerNewTexture(std::string in_textureName, GLuint in_textureID)
{
	int currentKey = gearTextureMap.size();
	gearTextureMap[currentKey] = in_textureID;
	gearTextureLookup[in_textureName] = currentKey;
}

void Gear::registerNewBuffer(std::string in_bufferName, GLuint in_bufferID)
{
	int currentKey = gearBufferMap.size();
	gearBufferMap[currentKey] = in_bufferID;
	gearBufferLookup[in_bufferName] = currentKey;
}

GLuint Gear::getBufferID(std::string in_bufferName)
{
	GLuint returnGLuint;
	int lookupID = gearBufferLookup[in_bufferName];
	returnGLuint = gearBufferMap[lookupID];
	return returnGLuint;
}

GLuint Gear::getPersistentBufferID(std::string in_bufferName)
{
	GLuint returnGLuint;
	int lookupID = gearPersistentBufferLookup[in_bufferName];
	returnGLuint = gearPersistentBufferMap[lookupID];
	return returnGLuint;
}

GLuint Gear::getFBOID(std::string in_fboName)
{
	GLuint returnGLuint;
	int lookupID = gearFboLookup[in_fboName];
	returnGLuint = gearFboMap[lookupID];
	return returnGLuint;
}

void Gear::insertUniformRequest(GLDataType in_dataType, std::string in_uniformName)
{
	GLUniformRequest newRequest(in_dataType, in_uniformName);
	uniformRequests.push_back(newRequest);
}

void Gear::insertMultiDrawArrayJobRequest(std::string in_jobName)
{
	multiDrawArrayJobRequests.push_back(in_jobName);
}

std::vector<GLUniformRequest>* Gear::getUniformRequests()
{
	return &uniformRequests;
}

std::vector<std::string>* Gear::getMultiDrawArrayJobRequests()
{
	return &multiDrawArrayJobRequests;
}

std::vector<std::string>* Gear::getDrawElementsInstancedRequests()
{
	return &drawElementsInstancedRequests;
}

GLMultiDrawArrayJob Gear::getMultiDrawArrayJob(std::string in_jobName)
{
	GLMultiDrawArrayJob returnJob;
	int lookupID = gearMultiDrawArrayJobLookup[in_jobName];
	returnJob = gearMultiDrawArrayJobMap[lookupID];
	return returnJob;
}