#pragma once

#ifndef SHADERMACHINEBASE_H
#define SHADERMACHINEBASE_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
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
#include "ImGuiButtonPanelContainer.h"
#include "ImGuiButtonClickResult.h"
#include "ImGuiSliderFloatPanelContainer.h"
#include "ImGuiInputTextPanelContainer.h"
#include "ShaderMachineFeedback.h"
#include "SmartIntMap.h"
#include "TimeBasedWaveManager.h"
#include "TimeBasedWaveType.h"
#include "DynamicBufferManager.h"
#include "KeyPressTracker.h"
#include "OneOffMacros.h"
#include "GPUCoordinateMode.h"
#include "GPUWorldCoordinateProducer.h"
#include "IndependentUtils.h"

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
		
		virtual void flagCollectionGLDataForRemoval(EnclaveKeyDef::EnclaveKey in_keyForRemoval) = 0;
		virtual void insertCollectionGLDataIntoGearBuffer(std::string in_gearName, std::string in_gearBufferName, TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef) = 0;

		virtual void removeUnusedReplaceables() = 0;
		virtual void insertWorldLight(std::string in_stringedContainerName, int in_lightID, WorldLight in_worldLight) = 0;

		// miscellaneous state retrieval functions
		GPUCoordinateMode fetchMachineCoordMode();	// allows objects outside of an instance of this class to check the kinds of operations and style of vertex data 
													// that the ShaderMachine expects to receive, so that those programs/objects may send data correctly 
													// (i.e, OrganicCoreSystem would need this to determine what modifications it needs to make, if any, to vertex terrain data)

		// direct Gear program communication 
		void sendMessageToGLProgram(std::string in_programName, Message in_message);

		// uniform value retrieval functions
		float retrieveFloatUniform(std::string in_floatUniformName);

		// runtime buffer manipulation functions
		void createDynamicBufferAndSendToGear(std::string in_bufferName, std::string in_programName);
		void createDynamicBufferMultiDrawArrayJobAndSendToGear(std::string in_bufferName,
																std::string in_programName,
																GLint* in_startArray,
																GLsizei* in_vertexCount,
																int in_drawCount);
		void sendDataToDynamicBuffer(std::string in_bufferName, int in_byteSizeToWrite, GLfloat* in_dataArray);

	
		void sendMessageAndBufferDataToGLProgram(std::string in_programName, // Sends a Message + buffer data directly to a gear; will only get sent to the Gear if the Gear exists
										   Message in_message,			
										   int in_bufferSizeInBytes,	// Friendly Remember! 1 float = 4 bytes
										   GLfloat* in_dataArray);

		void deleteDynamicBuffer(std::string in_bufferName);	// uses the dynBufferManager to search for a list of gears that are affected by the 
																// buffer delete; these affected gears will get a Message sent to them, which they may
																// interpret to perform any necessary cleanup before the dynamically allocated buffer is deleted.

		// GL multi draw array functions
		void registerMultiDrawArrayJob(std::string in_drawJobName, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);	// registers and enables a new draw job.
		void disableMultiDrawArrayJob(std::string in_drawJobName);	// disables a multi draw array job (job must exist to be disabled)
		void enableMultiDrawArrayJob(std::string in_drawJobName);		// enables a multi draw array job (job must exist to be enabled)

		// GL instanced drawing functions
		void registerDrawElementsInstancedJob(std::string in_instancedJobName, int in_numberOfElements);

		// persistent buffer functions
		GLuint getPersistentBufferID(std::string in_bufferName);																	// gets the ID of the specified persistent buffer with the value of "in_bufferName"
		void sendDataToPersistentBuffer(std::string in_bufferName, int in_offset, int in_byteSizeToWrite, GLfloat* in_dataArray);	// send's data to " " " "; uses sub

		// buffer functions
		bool doesBufferExist(std::string in_bufferName);
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

		// wave manager functions
		void registerTBW(std::string in_tbwName, TimeBasedWaveType in_timeBasedWaveType);
		void registerTBWAndSendRequestToProgramGear(std::string in_tbwName, TimeBasedWaveType in_timeBasedWaveType, std::string in_programName);
		void deregisterTBWANdRemoveRequestFromProgramGear(std::string in_tbwName);

		 // "Terrain" vao data value retrieval functions for OrganicSystem
		int getVaoAttribMode();
		int getVaoAttribByteSize();
		void computeCameraDirectionAndPosition(bool in_imguiFocusedFlag);	// STEP 2: update the camera direction and position; 
																			// the position of the camera is changed based on input keystrokes, multiplied
																			// by the current "speed" modifier for the movement.

		void updateMatricesAndDelta();								// STEP 3

		// position and direction setting and retrieval
		void setPosition(glm::vec3 in_positionValue);
		glm::vec3 getPositionValue();
		glm::vec3 getDirectionValue();
		GLFWwindow* getWindow();

		TerrainMemoryTracker terrainMemoryTracker;		// built-in terrain memory tracker

		// imgui clicked buttons
		void checkForClickedButtons();						// checks for any button that was clicked; notifies the returning ShaderMachineFeedback instance for this tick whether or not
															// the mouse cursor was in a button panel.

		// input feedback processing, where input/keystrokes can be sent to named ImGui objects.
		void processInputFeedbackToImGuiObject(Message in_inputForObject);

		// toggled displayManagers that are set in OrganicGLManager's displayManager (OrganicCoreLib)
		void renderWorldLocation(float world_precise[3], int world_organicLoc[9]);	// renders a world location box on the screen. 
		void renderTargetedBlockLocation(int world_organicLoc[9]);					// renders the coordinates of a targeted block.
		void checkForTextInput();													// loads data into machineFeedback; allows programs using the shader to access this input data;
		void renderSliders();								// will render single float interactable sliders; notifies the returning ShaderMachineFeedback instance for this tick whether or not
															// the mouse cursor was in a slider panel.

		ShaderMachineFeedback retrieveShaderInputs();		// fetches (via std::move) any inputs that were given to the ShaderMachine, for use by other code (such as OrganicCoreLib etc)
		
		// set direction
		void setDirection(float in_x, float in_y, float in_z);
		KeyPressTracker keyTracker;	// set back to private member when done testing on 10/3/2021.

		void handleKeyPressInputs();	// analyzes the machineFeedback member to see if keyboard inputs were made 
										// to any existing ImGui elements (i.e, text input box, button clicked); if they were,
										// we do not want to modifiy/insert any existing keyPressCycles in the keyTracker,
										// because those inputs were focused on a specific ImGui element at the time; if we do not do this,
										// the ShaderMachineBase instance will incorrectly register input on the ImGui element AND
										// register/modify a keyCycle at the same time. This function should be called right before the call to 
		                                // retrieveShaderInputs().
		void resetFeedback();
protected:
		friend class Gear;
		// misc
		GLFWwindow* window;									// pointer to openGL window
		int width, height;									// screen height, width

		GLUniformRegistry uniformRegistry;
		WorldLightContainerMap worldLights;
		ShaderMachineFeedback machineFeedback;				// gets any inputs that were received; it is loaded using a call to checkForTextInput().
		TimeBasedWaveManager waveManager;
		DynamicBufferManager dynBufferManager;
		GPUCoordinateMode machineCoordMode = GPUCoordinateMode::COORDINATE_MODE_UNDEFINED;	// determines how an instantiated ShaderMachine should have its gears interpret
																							// vertex data. ***THIS MUST** be manually set by the call to the virtual initialize() function in each class that is derived
																							// from this base. If it is not set, crashes/unhandled exceptions/errors are going to occur.

		GPUWorldCoordinate machineCurrentWorldCoord;	// the current location of the world camera, in a dvec3; should be utilized when machineCoordMode is using 
														// GPUCoordinateMode::COORDINATE_MODE_LOCAL.

		// Below: a pointer to the selected matrix/delta calculation function to use. ShaderMachine objects
		// will determine a select function to use, based on the value of machineCoordMode, by calling the 
		// setMachineCoordModeDependentSettings() function.
		void(ShaderMachineBase::*matrixAndDeltaSelectedFunctionPtr)() = nullptr;


		SmartIntMap<GLuint> bufferMap;									// (UPDATED 4/15/2021, to SmartIntMap) for typical buffers (non-persistent)
		SmartIntMap<GLuint> persistentBufferMap;						// (UPDATED 4/15/2021, to SmartIntMap) map that stores IDs of persistent buffers
		SmartIntMap<GLuint> programMap;									// (UPDATED 4/15/2021, to SmartIntMap) " for program IDs
		SmartIntMap<GLuint> fboMap;										// (UPDATED 4/15/2021, to SmartIntMap) " for frame buffer objects
		SmartIntMap<GLuint> textureMap;									// (UPDATED 4/15/2021, to SmartIntMap)  " for textures
		SmartIntMap<GLMultiDrawArrayJob> multiDrawArrayJobMap;			// (UPDATED 4/15/2021, to SmartIntMap)
		SmartIntMap<std::unique_ptr<Gear>> gearTrain;					// (UPDATED 4/15/2021, to SmartIntMap) map that stores individual OpenGL programs (aka, "Gears"). GearTrain is borrowed from an engineering term.
		SmartIntMap<GLDrawElementsInstancedJob> drawElementsInstancedJobMap;	// (UPDATED 4/15/2021, to SmartIntMap)
		SmartIntMap<AtlasMap> atlasMapMap;

		//std::map<int, AtlasMap> atlasMapMap;
		//std::map<int, GLuint> bufferMap;									// for typical buffers (non-persistent)


		std::unordered_map<std::string, int> bufferLookup;
		std::unordered_map<std::string, int> persistentBufferLookup;					// used to look up buffer IDs
		std::unordered_map<std::string, int> programLookup;
		std::unordered_map<std::string, int> fboLookup;
		std::unordered_map<std::string, int> textureLookup;
		std::unordered_map<std::string, int> multiDrawArrayJobLookup;
		std::unordered_map<std::string, int> drawElementsInstancedJobLookup;
		std::unordered_map<std::string, int> atlasMapLookup;

		// itinerary maps -- the int value in each of these should correspond to a valid gear ID, for the key in the gearTrain map.
		std::unordered_map<std::string, int> multiDrawArrayItineraries;
		std::unordered_map<std::string, int> tbwDestinationItineraries;

		// "Terrain" vao data values
		int vaoAttribMode;			// the VAO attrib mode for rendering
		int vaoAttribByteSize;		// the size, in bytes, of the combined VAO

		// time
		static double lastTime;
		double currentTime;
		float deltaTime;

		// world movement keys
		int moveForwardKey = GLFW_KEY_W;
		int strafeRightKey = GLFW_KEY_D;
		int strafeLeftKey = GLFW_KEY_A;
		int moveBackwardKey = GLFW_KEY_S;

		bool wasFocusedPreviousFrame = false;

		// buffer sizes
		int terrainBufferSize = 0;	// set when the ShaderMachine is initialized

		// mouse control 
		bool cameraBoundToMousePointer = true;	


		// for the camera
		glm::vec3 position = glm::vec3(30, 0, 5);									// Initial position of camera : on +30x, 0y, +5z
		glm::dvec3 smWorldPosition = glm::dvec3(30, 0, 5);
		glm::vec3 direction;														// direction camera is facing
		glm::vec3 up;
		glm::mat4 projection;														// (temporary) OpenGL projection matrix
		glm::mat4 view;																// (temporary) OpenGL view matrix
		glm::mat4 model;															// (temporary) OpenGL model matrix
		glm::mat4 MVP;

		//float horizontalAngle = 0.0f;												// Initial horizontal angle : toward -Z
		//float horizontalAngle = 3.14f * .75f;										// Initial horizontal angle : toward -Z
		float horizontalAngle = (3.14f * .75f) + 3.14f;
																					// 3.14 = 0,0, 1
																					// 3.14 * .75f = .7, 0, -.7
																					// (3.14 * .75) + 3.14 = 
		float verticalAngle = 0.0f;													// Initial vertical angle : none
		float initialFoV = 45.0f;													// Initial Field of view
		float speed = 10.0f;															// 3 units / second
		float mouseSpeed = 0.005f;

		// imgui objects
		ImGuiButtonPanelContainer buttonPanelContainer;
		ImGuiSliderFloatPanelContainer sliderPanelContainer;
		ImGuiInputTextPanelContainer inputPanelContainer;

		// functions dependent on the value of machineCoordMode
		void setMachineCoordModeDependentSettings();	// This *MUST* be called in the initialize() function of each ShaderMachine, 
														// AFTER machineCoordMode has been set by each ShaderMachine,
														// or OpenGL/the program will crash while accessing a null pointer.

		void runMatrixAndDeltaAbsoluteComputations();	// Uses logic required when vertex data is in absolute coordinates

		void runMatrixAndDeltaLocalComputations();		// Uses logic required when vertex data is in blueprint-localized coordinates.

		// gearTrain related functions
		void runGearTrain();
		void swapAndPoll();
		
		//void sendGearUniforms();
		void sendGearUniforms();
		void sendDrawJobs();	
		void sendMultiDrawArrayJobRequests(int in_gearID, Gear* in_gearRef);		// send multi draw array requests, if there are any, to the Gear
		void sendDrawElementsInstancedRequests(int in_gearID, Gear* in_gearRef);	// send draw elements instanced requests, if there are any, to the Gear

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

		// wave manager functions
		void updateWaveUniforms();

		// draw job disabling functions

		// program creation
		void createProgram(std::string in_programName);
		void createComputeProgram(std::string in_programName);

		// control toggling
		void toggleCameraBoundToMousePointer();

		// set direction
		//void setDirection(float in_x, float in_y, float in_z);
		
		GLuint* getTextureRef(std::string in_textureName);
		GLMultiDrawArrayJob* getMultiDrawArrayJobRef(std::string in_jobName);
		GLMultiDrawArrayJob getMultiDrawArrayJob(std::string in_jobName);
		GLDrawElementsInstancedJob getDrawElementsInstancedJob(std::string in_jobName);

		// test, call back function.
		static void keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
		void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static void mouseScrollCallbackWrapper(GLFWwindow* window, double xoffset, double yoffset);
		void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		static void mouseButtonCallbackWrapper(GLFWwindow* window, int button, int action, int mods);
		void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
private:
		struct GearFindResult
		{
			GearFindResult() {};
			GearFindResult(bool in_wasResultFound, int in_foundGearIndex, Gear* in_foundGearRef) :
				wasResultFound(in_wasResultFound),
				foundGearIndex(in_foundGearIndex),
				foundGear(in_foundGearRef)
			{};
			bool wasResultFound = false;
			int foundGearIndex = 0;
			Gear* foundGear = nullptr;
		};
		GearFindResult findGear(std::string in_programName);

};

#endif;
