#pragma once

#ifndef MShaderController_H
#define MShaderController_H

#include "MShaderBase.h"

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
		bool switchOShader(std::string in_shaderToSwitchTo);		// switch to a shader, if it's available; return true if switch was successful.
		bool addOShaderToCatalog(std::string in_shaderToSwitchTo);	// attempt to add a shader to the catalog; return true if it was found and sucessfully added.

		// Below: Use a series of Messages (or just a single one) to signal how the MShaderController should be set up. For example,
		// a Message for setting the screen resolution could be sent, along with a Message for setting the background color or certain matrix values.
		template<typename FirstMessage, typename ...RemainingMessages> void setup(
																				  FirstMessage && firstMessage,
			                                                                     RemainingMessages && ...remainingMessages)
		{
			processMessage(std::forward<FirstMessage>(firstMessage));
			setup(std::forward<RemainingMessages>(remainingMessages)...);
		}
		void setup() {}

	private:
		void setupSharedComponents();	// buffers, VAOs etc that can be shared between MShaderBase 
		                                // derived classes in the catalog should be set up here

		void instantiateAndSetupShader(std::string in_shaderToInstantiate);		// insert a new shader; check it's requests and grant them.
		void processMessage(Message in_messageToRead);	// this function has a bunch of case statements that can be processed against the MessageType values of 
														// Message objects that come from the call to setup().

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
