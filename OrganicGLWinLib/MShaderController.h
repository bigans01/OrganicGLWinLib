#pragma once

#ifndef MSHADERCONTROLLER_H
#define MSHADERCONTROLLER_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include "MShaderBase.h"
#include "OrganicGLWinUtils.h"
#include "ComputeResolution.h"

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
		bool switchMShader(std::string in_shaderToSwitchTo);		// switch to a shader, if it's available; return true if switch was successful.
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

	private:
		void setupSharedComponents();	// buffers, VAOs etc that can be shared between MShaderBase 
		                                // derived classes in the catalog should be set up here

		void instantiateAndSetupShader(std::string in_shaderToInstantiate);		// insert a new shader; check it's requests and grant them.
		void processMessage(Message in_messageToRead);	// this function has a bunch of case statements that can be processed against the MessageType values of 
														// Message objects that come from the call to setup().

		void initializeMandatoryItems();	// setup GLFW, GLEW, and GLFWWindow, as well as other basic state info.
		void createMShaders();

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

		int mainScreenWidth = 0;
		int mainScreenHeight = 0;
		int mainComputeDim = 0;		// would be set by a Message of the type MSHADER_SET_COMPUTE_RESOLUTION
		GLFWwindow* mainWindowPtr = nullptr;	// set by the call to initializeMandatoryItems


		std::unordered_map<std::string, MShaderBase> catalog;	// should contain all MShaderBase-derived classes that should be used

		// Below: a struct that maps strings to OpenGL binding IDs; can be used by any OpenGL object that 
		// utilizes bindings -- VAO, buffers, textures. It's purpose is to universally store all these IDs 
		// in maps that can be shared across multiple MShaderBase instances.
		struct BindingMap
		{
			BindingMap() {};

			bool doesBindingExist(std::string in_bindingToFind);	// returns true if the given binding was found; will not
																	// create a new binding in the map, though.

			void insertBinding(std::string in_bindingName, int in_bindingValue);	// insert a new binding; this will auto overwrite an existing binding 
																					// (be careful)

			std::unordered_map<std::string, int> bindings;

		};

};

#endif
