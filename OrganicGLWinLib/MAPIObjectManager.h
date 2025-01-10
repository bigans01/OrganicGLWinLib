#pragma once

#ifndef MAPIObjectManager_H
#define MAPIObjectManager_H

#include "MAPIObjectData.h"
#include "OrganicGLWinUtils.h"

/*
* 
* Description: the purpose of this class is store and record API objects used by OpenGL, or a similiar GPU API,
* such as buffer IDs that are being utilized. For instance, when glBindBuffer is called, the value gets assigned to an
* integer (GLint) somewhere; the value of that integer can be stored in this class, to be fetched later.
* 
* Resources used by a MShaderController object should be managed here -- in other words, their creation and deletion should be
* handled by this class (i.e, when a buffer is created, it is also deleted by this class). Delegating the responsibility of creation/deletion
* of resources to a single class will help assist in proper resource management, by keeping the creation/deletion to a single area/class.

*/


class MAPIObjectManager
{
	public:
		MAPIObjectManager() {};
		~MAPIObjectManager();
		bool doesBindingExist(MAPIObjectType in_bindingType, std::string in_bindingName);	// returns true if the given binding was found; will not
																						// create a new binding in the map, though.

		bool attemptBindingInsert(MAPIObjectData in_bindingToInsert);	// insert a new binding, as long as it didn't exist already; the bool return value 
																// should be true if it did insert a new binding, and false if it existed already.

		int fetchBinding(MAPIObjectType in_bindingType, std::string in_bindingName);	// given a type and name of the binding, attempt to 
																						// return a value from the appropriate binding map 
																						// (i.e, bufferResourceMap for MAPIObjectType::BUFFER).
																						// The return value will be the int that represents the assigned ID 
																						// that the GUI API gave the object when it was allocated.
	private:

		void cleanupResources();	// should be called on destructor of this class; deletes any remaining allocated resoruces in the resource maps.

		// used with MAPIObjectType::BUFFER
		struct MBufferBinding
		{
			MBufferBinding() {};
			MBufferBinding(MAPIObjectData in_bufferBinding)
			{
				bufferBindingName = in_bufferBinding.getBindingName();
				OrganicGLWinUtils::createBuffer(&bufferId);
				std::cout << "MBufferBinding: created buffer with ID: " << bufferId << std::endl;
			};

			void deleteBufferResource() 
			{ 
				std::cout << "!! deleting buffer resource, with ID: " << bufferId << std::endl;
				int deleteVal = 3;
				std::cin >> deleteVal;

				OrganicGLWinUtils::deleteBuffer(&bufferId); 
			} ;

			std::string bufferBindingName = "";
			GLuint bufferId = 0;
			Message bufferBindingMessage;


			std::string getBindingName()
			{
				return bufferBindingName;
			}
		};

		// maps for various binding structs
		std::unordered_map<std::string, MBufferBinding> bufferResourceMap;
};

#endif
