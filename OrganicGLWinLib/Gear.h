#pragma once

#ifndef GEAR_H
#define GEAR_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "GLUniformRegistry.h"
#include "GLUniformRequest.h"
#include "GLDataType.h"
#include "GLMultiDrawArrayJob.h"
#include "GLDrawElementsInstancedJob.h"
#include "SmartIntMap.h"
#include "GLProgramIDIndex.h"
#include "TerrainJobResults.h"

class ShaderMachineBase;
class Gear
{
	public:
		// misc, inherited by all
		GLFWwindow* window;			// pointer to openGL window
		int width, height;			// screen height, width
		GLuint programID;			// optional, but used in most cases: the ID of the shader program this Gear uses, when using a single shader machine.

		GLProgramIDIndex programIndex;		// stores multiple program IDs; the value a Gear uses for the programID will also be fetched from here.

		// virtual functions
		virtual void initializeMachineShader(int in_width, int in_height, GLuint in_programID, GLFWwindow* in_windowRef, ShaderMachineBase* in_shaderMachineBasePtr) = 0;
		virtual void render() = 0;
		virtual void passGLuintValue(std::string in_identifier, GLuint in_gluInt) = 0;
		virtual void executeGearFunction(std::string in_identifier) = 0;
		virtual void printData() = 0;
		virtual void interpretMessage(Message in_message) = 0;

		virtual void sendTerrainDataToGear(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) = 0;
		virtual void sendMessageAndBufferDataToGear(Message in_messageToSend, int in_arraySize, GLfloat* in_arrayRef) = 0;
		virtual void removeTerrainDataFromGear(EnclaveKeyDef::EnclaveKey in_keyToRemove) = 0;

		void insertMultiDrawArrayJob(std::string in_jobName, GLMultiDrawArrayJob in_job);
		void removeMultiDrawArrayJob(std::string in_jobName);
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
			else if constexpr (in_dType == GLDataType::INT)
			{
				gearUniformRegistry.insertInt(in_uniformName, in_data);
			}
			else if constexpr (in_dType == GLDataType::KEY3D)
			{
				gearUniformRegistry.insertEnclaveKey(in_uniformName, in_data);
			}
			
		}
		
		std::vector<GLUniformRequest>* getUniformRequests();
		std::vector<std::string>* getMultiDrawArrayJobRequests();
		std::vector<std::string>* getDrawElementsInstancedRequests();
		void insertUniformRequest(GLDataType in_dataType, std::string in_uniformName);
		void deleteUniformRequest(std::string in_uniformName);
	protected:
		void useProgram();
		std::vector<GLUniformRequest> uniformRequests;
		std::vector<std::string> multiDrawArrayJobRequests;
		std::vector<std::string> drawElementsInstancedRequests;
		ShaderMachineBase* gearMachinePtr = nullptr;

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
		void insertMultiDrawArrayJobRequest(std::string in_jobName);
		GLMultiDrawArrayJob getMultiDrawArrayJob(std::string in_jobName);
		GLDrawElementsInstancedJob getDrawElementsInstancedJob(std::string in_jobName);

		GLuint getBufferID(std::string in_bufferName);
		GLuint getPersistentBufferID(std::string in_bufferName);
		GLuint getFBOID(std::string in_fboName);

		SmartIntMap<std::unique_ptr<Gear>>* fetchGearTrainMapRef();
		ShaderMachineBase* fetchGearMachinePtr();
		GLMultiDrawArrayJob gearBaseFetchDynamicMultiDrawArrayJobCopy(std::string in_bufferName);
};

#endif
