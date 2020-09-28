#pragma once

#ifndef SHADERMACHINEBASE_H
#define SHADERMACHINEBASE_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <unordered_map>
#include "Gear.h"
#include "OrganicGLWinUtils.h"
#include "GLUniformRegistry.h"
#include "AtlasMap.h"
#include "AtlasPropertiesGL.h"
#include "GLMultiDrawArrayJob.h"
#include "GLDrawElementsInstancedJob.h"
#include "SimpleUnique.h"
#include "TerrainJobResults.h"
#include "TerrainMemoryTracker.h"
#include "WorldLight.h"
#include "WorldLightContainerMap.h"

class ShaderMachineBase
{
public:
		// virtual functions
		virtual void initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize) = 0;   // STEP 1
		virtual void setupTextureAtlases() = 0;	// STEP 2
		virtual void runAllShadersAndSwap() = 0;
		virtual void runAllShadersNoSwap() = 0;
		virtual void shutdownGL() = 0;
		virtual void multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections) = 0;		// STEP 4
		virtual void printDataForGears() = 0;
		virtual void insertCollectionGLData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) = 0;
		virtual void removeUnusedReplaceables() = 0;
		virtual void insertWorldLight(std::string in_stringedContainerName, int in_lightID, WorldLight in_worldLight) = 0;

		void registerDrawJob(std::string in_drawJobName, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);
		void registerDrawElementsInstancedJob(std::string in_instancedJobName, int in_numberOfElements);

		// persistent buffer functions
		GLuint getPersistentBufferID(std::string in_bufferName);																	// gets the ID of the specified persistent buffer with the value of "in_bufferName"
		void sendDataToPersistentBuffer(std::string in_bufferName, int in_offset, int in_byteSizeToWrite, GLfloat* in_dataArray);	// send's data to " " " "; uses sub

		// buffer functions
		GLuint getBufferID(std::string in_bufferName);
		void sendDataToBuffer(std::string in_bufferName, int in_byteSizeToWrite, GLfloat* in_dataArray);						// does a full copy, rebuilding the buffer
		void sendMat4DataToBuffer(std::string in_bufferName, int in_byteSizeToWrite, glm::mat4* in_dataArray);					// full copy "" 
		void sendDataToSSBOBuffer(std::string in_bufferName, int in_byteSizeToWrite, GLfloat* in_dataArray);					// full copy ""

		void sendDataToSSBOBufferSub(std::string in_bufferName, int in_offset, int in_byteSizeToWrite, GLfloat* in_dataArray);	// uses sub data, no full copy

		// FBO functions
		GLuint getFBOID(std::string in_fboName);

		// texture functions
		GLuint getTextureID(std::string in_textureName);
		GLuint& getTextureLValueRef(std::string in_textureName);

		// atlas functions
		AtlasMap* getAtlasMapRef(std::string in_atlasName);

		// terrain deferred functions
		GLuint* getTerrainBufferRef();
		GLuint* getTerrainSwapRef();
		GLuint getTerrainBufferID();
		GLuint getTerrainSwapID();

		// size fetching functions
		int getBufferSize();

		 // "Terrain" vao data value retrieval functions for OrganicSystem
		int getVaoAttribMode();
		int getVaoAttribByteSize();
		void computeMatricesFromInputs();			// STEP 2 
		void updateMatricesAndDelta();				// STEP 3

		glm::vec3* getPosition();
		glm::vec3* getDirection();
		GLFWwindow* getWindow();

		TerrainMemoryTracker terrainMemoryTracker;		// built-in terrain memory tracker

protected:
		// misc
		GLFWwindow* window;									// pointer to openGL window
		int width, height;									// screen height, width

		GLUniformRegistry uniformRegistry;
		WorldLightContainerMap worldLights;

		std::map<int, GLuint> bufferMap;									// for typical buffers (non-persistent)
		std::map<int, GLuint> persistentBufferMap;							// map that stores IDs of persistent buffers
		std::map<int, GLuint> programMap;									// " for program IDs
		std::map<int, GLuint> fboMap;										// " for frame buffer objects
		std::map<int, GLuint> textureMap;									// " for textures
		std::map<int, std::unique_ptr<Gear>> gearTrain;						// map that stores individual OpenGL programs (aka, "Gears"). GearTrain is borrowed from an engineering term.
		std::map<int, GLMultiDrawArrayJob> multiDrawArrayJobMap;
		std::map<int, GLDrawElementsInstancedJob> drawElementsInstancedJobMap;
		std::map<int, AtlasMap> atlasMapMap;

		std::unordered_map<std::string, int> bufferLookup;
		std::unordered_map<std::string, int> persistentBufferLookup;					// used to look up buffer IDs
		std::unordered_map<std::string, int> programLookup;
		std::unordered_map<std::string, int> fboLookup;
		std::unordered_map<std::string, int> textureLookup;
		std::unordered_map<std::string, int> multiDrawArrayJobLookup;
		std::unordered_map<std::string, int> drawElementsInstancedJobLookup;
		std::unordered_map<std::string, int> atlasMapLookup;

		// "Terrain" vao data values
		int vaoAttribMode;			// the VAO attrib mode for rendering
		int vaoAttribByteSize;		// the size, in bytes, of the combined VAO

		// time
		static double lastTime;
		double currentTime;
		float deltaTime;

		// buffer sizes
		int terrainBufferSize = 0;	// set when the ShaderMachine is initialized

		// mouse control 
		bool cameraBoundToMousePointer = true;	


		// for the camera
		glm::vec3 position = glm::vec3(30, 0, 5);									// Initial position of camera : on +30x, 0y, +5z
		glm::vec3 direction;														// direction camera is facing
		glm::vec3 up;
		glm::mat4 projection;														// (temporary) OpenGL projection matrix
		glm::mat4 view;																// (temporary) OpenGL view matrix
		glm::mat4 model;															// (temporary) OpenGL model matrix
		glm::mat4 MVP;
		float horizontalAngle = 3.14f;												// Initial horizontal angle : toward -Z
		float verticalAngle = 0.0f;													// Initial vertical angle : none
		float initialFoV = 45.0f;													// Initial Field of view
		float speed = 10.0f;															// 3 units / second
		float mouseSpeed = 0.005f;

		// gearTrain related functions
		void runGearTrain();
		void swapAndPoll();
		
		//void sendGearUniforms();
		void sendGearUniforms();
		void sendDrawJobs();	
		void sendMultiDrawArrayJobRequests(Gear* in_gearRef);		// send multi draw array requests, if there are any, to the Gear
		void sendDrawElementsInstancedRequests(Gear* in_gearRef);	// send draw elements instanced requests, if there are any, to the Gear

		// buffer creation functions
		void insertNewPersistentBuffer(std::string in_bufferName, int in_size);		// insert a persistent buffer
		void insertNewFBO(std::string in_fboName);									// insert a FBO
		void insertNewBuffer(std::string in_bufferName);							// insert a typical, non-persistent buffer
		void insertNewTexture(std::string in_textureName);
		void insertAndBuildNewAtlas(std::string in_atlasFolderName, GLuint* in_atlasTextureRef, float* in_atlasTileWidth, float* in_atlasWidth);	// for everything before the lighting compute machine
		void insertAndBuildNewAtlasToSpecifiedTextureChannel(GLenum in_texUnit, std::string in_atlasFolderName, GLuint* in_atlasTextureRef, float* in_atlasTileWidth, float* in_atlasWidth);
		
		// draw job functions
		void insertNewMultiDrawArrayJob(std::string in_jobName, GLMultiDrawArrayJob in_job);
		void insertNewDrawElementsInstancedJob(std::string in_jobName, GLDrawElementsInstancedJob in_job);

		// program creation
		void createProgram(std::string in_programName);
		void createComputeProgram(std::string in_programName);

		// control toggling
		void toggleCameraBoundToMousePointer();
		
		GLuint* getTextureRef(std::string in_textureName);
		GLMultiDrawArrayJob* getMultiDrawArrayJobRef(std::string in_jobName);
		GLMultiDrawArrayJob getMultiDrawArrayJob(std::string in_jobName);
		GLDrawElementsInstancedJob getDrawElementsInstancedJob(std::string in_jobName);

};

#endif;
