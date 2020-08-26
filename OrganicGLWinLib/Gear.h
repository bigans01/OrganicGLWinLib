#pragma once

#ifndef GEAR_H
#define GEAR_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <unordered_map>
#include <iostream>
#include <vector>
#include "GLUniformRegistry.h"
#include "GLUniformRequest.h"
#include "GLDataType.h"
#include "GLMultiDrawArrayJob.h"
#include "GLDrawElementsInstancedJob.h"

class Gear
{
	public:
		// misc, inherited by all
		GLFWwindow* window;									// pointer to openGL window
		int width, height;									// screen height, width
		GLuint programID;									// the ID of the shader program this Gear uses

		// virtual functions
		virtual void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef) = 0;
		virtual void render() = 0;
		virtual void passGLuintValue(std::string in_identifier, GLuint in_gluInt) = 0;
		virtual void executeGearFunction(std::string in_identifier) = 0;
		virtual void printData() = 0;
		void insertMultiDrawArrayJob(std::string in_jobName, GLMultiDrawArrayJob in_job);
		void insertDrawElementsInstancedJob(std::string in_jobName, GLDrawElementsInstancedJob in_job);
		
		template <GLDataType in_dType, typename PassType> void sendUniform(std::string in_uniformName, PassType in_data)
		{
			if constexpr (in_dType == GLDataType::VEC2)
			{
				gearUniformRegistry.insertVec2(in_uniformName, in_data);
				//std::cout << "!!! VEC2 inserted!" << std::endl;
			}
			else if constexpr (in_dType == GLDataType::VEC3)
			{
				gearUniformRegistry.insertVec3(in_uniformName, in_data);
				//std::cout << "!!! VEC3 inserted!" << std::endl;
			}
			else if constexpr (in_dType == GLDataType::MAT3)
			{
				gearUniformRegistry.insertMat3(in_uniformName, in_data);
			}

			else if constexpr (in_dType == GLDataType::MAT4)
			{
				gearUniformRegistry.insertMat4(in_uniformName, in_data);
				//std::cout << "!!! MAT4 inserted!" << std::endl;
			}

			else if constexpr (in_dType == GLDataType::FLOAT)
			{
				gearUniformRegistry.insertFloat(in_uniformName, in_data);
				//std::cout << "!!! FLOAT inserted!" << std::endl;
			}
			
		}
		
		std::vector<GLUniformRequest>* getUniformRequests();
		std::vector<std::string>* getMultiDrawArrayJobRequests();
		std::vector<std::string>* getDrawElementsInstancedRequests();

	protected:
		void useProgram();
		std::vector<GLUniformRequest> uniformRequests;
		std::vector<std::string> multiDrawArrayJobRequests;
		std::vector<std::string> drawElementsInstancedRequests;

		std::map<int, GLuint> gearBufferMap;									// for typical buffers (non-persistent)
		std::map<int, GLuint> gearPersistentBufferMap;							// map that stores IDs of persistent buffers
		std::map<int, GLuint> gearFboMap;										// " for frame buffer objects
		std::map<int, GLuint> gearTextureMap;
		std::map<int, GLMultiDrawArrayJob> gearMultiDrawArrayJobMap;
		std::map<int, GLDrawElementsInstancedJob> gearDrawElementsInstancedJobMap;

		std::unordered_map<std::string, int> gearBufferLookup;
		std::unordered_map<std::string, int> gearPersistentBufferLookup;					// used to look up buffer IDs
		std::unordered_map<std::string, int> gearFboLookup;
		std::unordered_map<std::string, int> gearTextureLookup;
		std::unordered_map<std::string, int> gearMultiDrawArrayJobLookup;
		std::unordered_map<std::string, int> gearDrawElementsInstancedJobLookup;

		GLUniformRegistry gearUniformRegistry;

		void registerNewBuffer(std::string in_bufferName, GLuint in_bufferID);							// insert a typical, non-persistent buffer
		void registerNewPersistentBuffer(std::string in_bufferName, GLuint in_bufferID);		// insert a persistent buffer
		void registerNewFBO(std::string in_fboName, GLuint in_bufferID);									// insert a FBO
		void registerNewTexture(std::string in_textureName, GLuint in_textureID);
		void insertUniformRequest(GLDataType in_dataType, std::string in_uniformName);
		void insertMultiDrawArrayJobRequest(std::string in_jobName);
		GLMultiDrawArrayJob getMultiDrawArrayJob(std::string in_jobName);
		GLDrawElementsInstancedJob getDrawElementsInstancedJob(std::string in_jobName);

		GLuint getBufferID(std::string in_bufferName);
		GLuint getPersistentBufferID(std::string in_bufferName);
		GLuint getFBOID(std::string in_fboName);

};

#endif
