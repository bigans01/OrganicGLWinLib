#pragma once

#ifndef MSHADERCONTROLLER_H
#define MSHADERCONTROLLER_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include "MShaderBase.h"
#include "OrganicGLWinUtils.h"
#include "ComputeResolution.h"
#include "ImGuiButtonClickResult.h"
#include "ImGuiButtonPanelContainer.h"
#include "ImGuiSliderFloatPanelContainer.h"
#include "ImGuiInputTextPanelContainer.h"
#include "ShaderMachineFeedback.h"
#include "MBindingMap.h"

#include "MSBasicCompute.h"
#include "MSBasicGrayscale.h"

#include "MShaderCatalog.h"
#include "MShaderSelectionCycler.h"
#include "GLUniformRegistry.h"
#include <chrono>
#include "MGCIndex.h"
#include "MShaderHintIndexer.h"

/*

Description: This class serves as a new prototype for rendering, where the MShaderBase-derived classes stored in this class 
-- which similar to the ShaderMachine class -- may be chosen to run, or selected as the rendering mechanism, during runtime.
This functionality does not need to be exclusive to an OrganicSystem; it can be used in tools and other programs as well.

This class relies on various Message objects to be supplied to it, to assist in instantiation. A Message could contain the name of
a uniform value to set, the value to set for that uniform, the screen resolution to use, etc. The idea of all this is that the common
buffers, VAOs, uniforms, textures etc are instantiated and managed by this class itself, rather than having that be the 
responsibility of the MShaderBase-derived classes. By doing this, buffers, VAOs, textures, and other objects can be shared between ShaderMachines.
The deleting/cleanup of these objects should also be easier, as it can all be done in this class.

For instance, one could run a shader that uses compute shaders, or they could run a basic gray scale shader. However, each selected shader program(s)
must have an agreed-upon-format between each other. While this may not always be the most efficient approach, it should allow for ease of use and
programming, when developing new derivatives of the MShaderBase class. 

*/

class MShaderController
{
	public:
		MShaderController();
		~MShaderController();	// used to clean up the OpenGL Window, if it was created.

		bool switchMShader(std::string in_shaderToSwitchTo);			// directly switch to a shader, if it's available; return true if switch was successful.
		void attemptSwitchOnNextTick(std::string in_shaderToSwitchTo);  // attempts to switch to the MShader that is represented by the given string, during the next call to runTick().

		bool addMShaderToCatalog(std::string in_shaderToSwitchTo);	// attempt to add a shader to the catalog; return true if it was found and sucessfully added.


		template<typename FirstMessage, typename ...RemainingMessages> void setup(FirstMessage && firstMessage,
																				RemainingMessages && ...remainingMessages)
		{
			// Parse all the Messages.
			parseMessages(std::forward<FirstMessage>(firstMessage), std::forward<RemainingMessages>(remainingMessages)...);

			// Setup mandatory items (GLFW, GLEW, OpenGL window etc)
			initializeMandatoryItems();

			// Attempt to instantiate MShader objects, based on what's in the mShaderSetupQueue.
			createMShaders();

		}
		void setup() {}


		// Below: Use a series of Messages (or just a single one) to signal how the MShaderController should be set up. For example,
		// a Message for setting the screen resolution could be sent, along with a Message for setting the background color or certain matrix values.
		template<typename FirstMessage, typename ...RemainingMessages> void parseMessages(FirstMessage && firstMessage,
																						RemainingMessages && ...remainingMessages)
		{
			processMessage(std::forward<FirstMessage>(firstMessage));
			parseMessages(std::forward<RemainingMessages>(remainingMessages)...);
		}
		void parseMessages() {}

		// Information queue printing/fetching
		void writeOutInformationalMessages();	// write any informational Messages in the mShaderSetupQueue to std::cout.

		void insertNewGradient(Message in_gradientInsertionMessage);	// uses either a Message of the type MSHADER_SETUP_CYCLICAL_MGRADIENT
																		// or MSHADER_SETUP_FINITE_MGRADIENT to create an MGRADIENT and insert it into the 
																		// controllerMGCI (object of MGCIndex)

		void runTick();

	private:
		void processShaderChangeRequests();	//	1.	check if there is a request to switch to a new MShader; if there is a request,
											//		ensure that the MShader exists in the catalog.
											//
											//	2.  If the MShader does exist, proceed to checking for transitional hints in the 
											//		controllerHintIndexer (class MShaderHintIndexer). This class should return a set of 
											//		MShaderHintEnum values; use a case/switch statement to check the values to decide how to process.
											//		the MShaderHintEnum values should be used to create gradients.

		void setupSharedComponents();	// buffers, VAOs etc that can be shared between MShaderBase 
		                                // derived classes in the catalog should be set up here

		void instantiateAndSetupShader(std::string in_shaderToInstantiate);		// insert a new shader; check it's requests and grant them.
		void processMessage(Message in_messageToRead);	// this function has a bunch of case statements that can be processed against the MessageType values of 
														// Message objects that come from the call to setup().

		void initializeMandatoryItems();	// setup GLFW, GLEW, and GLFWWindow, as well as other basic state info.
		void createMShaders();

		void updateMVPVariables();
		
		void updateAndapplyGradients(float in_ms);	// update all grandients, and fetch their updated Messages, so
													// that the values in the controllerValueRegistry are updated appropriately.

		// Set functions, used in call to setup
		Message setComputeResolution(Message in_messageToRead);

		// Informational queue
		std::queue<Message> mShaderInfoQueue;	// a queue that should store informational Message objects of the type MSHADER_INFO;
												// should be used to print out various metadata/informational items, when the MShaderController is operating.

		std::queue<Message> mShaderSetupQueue;	// Message objects that will be used to instantiate MShaderBase-derived classes should go here; 
												// This queue should only be read-from/utilized after the objects that the MShaders rely on have all been setup;
												// in other words, it should be one of the last functions called during
												

		// Various flags that determine what has been set
		bool isResolutionSet = false;	// set to true when an actual resolution has been set for the MShaderBase-derived classes;
		bool isGLFWWindowActive = false;	// needs to be set to true when the call to OrganicGLWinUtils::checkWindowValidity in MShaderController::initializeMandatoryItems
											// returns true.

		bool wasInitialTimeCalculated = false;	// used by calculatePassedTime(), to check if that function has been called at least once (used to set the initial value of lastTimeStamp)

		int mainScreenWidth = 0;
		int mainScreenHeight = 0;
		int mainComputeDim = 0;		// would be set by a Message of the type MSHADER_SET_COMPUTE_RESOLUTION
		GLFWwindow* mainWindowPtr = nullptr;	// set by the call to initializeMandatoryItems

		// ***************************************************** Shader-related objects ************************************************************
		MShaderCatalog catalog;		// contains and manages all usable MShaders
		MShaderSelectionCycler mShaderCycler;	// keeps track of which MShader is currently selected, as well as the last one that was used.
		std::string nextShaderToSwitchTo = "";	// set by a call to 
		bool shaderSwitchAttemptFlag = false;	// set to true whenever nextShaderToSwitchTo gets set, and set to false when it gets cleared.
		

		// ***************************************************** Time objects and functions*********************************************************
		std::chrono::steady_clock::time_point lastTimeStamp;		// the timestamp set in the previous call to calculatePassedTime
		std::chrono::steady_clock::time_point currentTimeStamp;		// the current timestamp value
		float millisecondsSinceLastTimestamp = 0.0f;	// the time difference between currentTimeStamp and lastTimeStamp; should be updated every tick.
		void calculatePassedTime();	// update currentTimeStamp, millisecondsSinceLastTimestamp, and set lastTimeStamp to the value of currentTimeStamp after that
									// calculation is done.

		// ***************************************************** Gradient control ******************************************************************
		MGCIndex controllerMGCI;	// stores and manages gradients that can be used by MShaderController

		// ************************************************** Hint management ******************************************************************
		MShaderHintIndexer controllerHintIndexer;	// contains and manages hints that can be used by MShaderController

		// ***************************************************** Shareable objects *****************************************************************
		// imgui objects, intended to be referenced via pointers in MShaderBase-derived classes
		ImGuiButtonPanelContainer controllerButtonPanelContainer;
		ImGuiSliderFloatPanelContainer controllerSliderPanelContainer;
		ImGuiInputTextPanelContainer controllerInputPanelContainer;
		ShaderMachineFeedback controllerMachineFeedback;
		MBindingMap controllerBindings;				// binding map for use across different objects
		GLUniformRegistry controllerValueRegistry;	// should contain all uniforms, values that would be shared between MShaders and this instance

};

#endif
