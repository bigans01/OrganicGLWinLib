#pragma once

#ifndef MAPIObjectManager_H
#define MAPIObjectManager_H

#include "MAPIObjectData.h"
#include "MAPIObjectRequest.h"
#include "OrganicGLWinUtils.h"
#include "MAPIObjectMetadata.h"

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
		bool doesBindingExist(MAPIObjectMetadata in_objectMeta);	// returns true if the given binding was found; will not
																						// create a new binding in the map, though.


		Message handleMAPIObjectRequest(MAPIObjectRequest in_objectRequest);	//	Get the meta data from the MAPIObjectRequest, and analyze it to attempt to insert a new binding, 
																				//	as long as it didn't exist already; This function will create a corresponding MAPIObjectType that is built
																				//	around the MAPIObjectType specified in in_objectRequest. That object will be used in the call to 
																				//  attemptBindingInsert, which will return a Message containing specifics about what happened during the 
																				//  attempted insert.
	

		int fetchBinding(MAPIObjectMetadata in_objectMeta);	// given a type and name of the binding, attempt to 
															// return a value from the appropriate binding map 
															// (i.e, bufferResourceMap for MAPIObjectType::BUFFER).
															// The return value will be the int that represents the assigned ID 
															// that the GUI API gave the object when it was allocated.
															// Return value will be -1 if no object is found.

		bool deleteBinding(MAPIObjectMetadata in_objectMeta);
	private:

		void cleanupResources();	// should be called on destructor of this class; deletes any remaining allocated resoruces in the resource maps.
		Message attemptBindingInsert(MAPIObjectData in_bindingToInsert);	// used by handleMAPIObjectRequest to insert new API-based objects into their corresponding map/container.
																			// The return value of this is a Message that contains if the insertiona failed/succeeded, and what relevant reasons, if any.

		// used with MAPIObjectType::BUFFER, when inserting into the string-keyed map, bufferResourceMap. 
		// This simply calls glGenBuffers, via createBuffer below.
		// No special error checking should be required here, as we are simply generating a buffer.
		struct StringedMBufferBinding
		{
			public:
				StringedMBufferBinding() {};
				StringedMBufferBinding(MAPIObjectData in_bufferBinding)
				{
					bufferBindingName = in_bufferBinding.getDataBindingName();
					OrganicGLWinUtils::createBuffer(&bufferId);
					std::cout << "StringedMBufferBinding: created buffer with ID: " << bufferId << std::endl;
				}

				void deleteBufferResource() 
				{ 
					OrganicGLWinUtils::deleteBuffer(&bufferId); 
				}

				std::string getStringedDataBindingName()
				{
					return bufferBindingName;
				}

				GLuint getBufferId()
				{
					return bufferId;
				}
			private:
				std::string bufferBindingName = "";
				GLuint bufferId = 0;
				Message bufferBindingMessage;	// optional, potentially unused (give it a tuning pass sometime after 1/11/2025)


		};

		struct ThreeDKeyedMBufferBinding
		{
			public:
				ThreeDKeyedMBufferBinding() {};
				ThreeDKeyedMBufferBinding(MAPIObjectData in_threeDBindingData)
				{
					threeDKeyedMapName = in_threeDBindingData.getDataBindingThreeDKeyMapName();
					threeDKeyValue = in_threeDBindingData.getDataBindingThreeDKey();
					OrganicGLWinUtils::createBuffer(&threeDKeyedBufferId);
					std::cout << "ThreeDKeyedMBufferBinding: created buffer with ID: " << threeDKeyedBufferId << std::endl;
				}

				void deleteThreeDBufferResource()
				{
					OrganicGLWinUtils::deleteBuffer(&threeDKeyedBufferId);
				}

				GLuint getThreeDKeyedBufferId()
				{
					return threeDKeyedBufferId;
				}

			private:
				std::string threeDKeyedMapName = "";
				EnclaveKeyDef::EnclaveKey threeDKeyValue;
				GLuint threeDKeyedBufferId = 0;
				Message threeDKeyBindingMessage;

		};

		struct ThreeDKeyedBufferBindingMap
		{
			public:
				void insertThreeDBinding(EnclaveKeyDef::EnclaveKey in_threeDValue, ThreeDKeyedMBufferBinding in_threeDBindingData)
				{
					threeDBindings[in_threeDValue] = in_threeDBindingData;
					std::cout << "!!! inserted into threeDBindings..." << std::endl;
				}
				void deleteThreeDBinding(EnclaveKeyDef::EnclaveKey in_threeDValue)
				{
					threeDBindings.erase(in_threeDValue);
				}

				bool isMapEmpty()
				{
					return threeDBindings.empty();
				}

				GLuint fetchThreeDBindingId(EnclaveKeyDef::EnclaveKey in_threeDValue)
				{
					GLuint threeDBufferId = -1;
					auto threeDFinder = threeDBindings.find(in_threeDValue);
					if (threeDFinder != threeDBindings.end())
					{
						threeDBufferId = threeDFinder->second.getThreeDKeyedBufferId();
					}
					return threeDBufferId;
				}

				bool doesThreeDBindingExist(EnclaveKeyDef::EnclaveKey in_threeDValue)
				{
					bool doesBindingExist = false;
					auto threeDFinder = threeDBindings.find(in_threeDValue);
					if (threeDFinder != threeDBindings.end())
					{
						doesBindingExist = true;
					}
					return doesBindingExist;
				}

			//private:
				std::unordered_map<EnclaveKeyDef::EnclaveKey, ThreeDKeyedMBufferBinding, EnclaveKeyDef::KeyHasher> threeDBindings;

		};

		// used with MAPIObjectType::TEXTURE, when inserting into the string-keyed map, textureResourceMap. 
		// This simply calls glGenTextures directly (no calls to OrganicGLWinUtils)
		// No special error checking required here.
		struct StringedMTextureBinding
		{
			public:
				StringedMTextureBinding() {};
				StringedMTextureBinding(MAPIObjectData in_textureBinding)
				{
					textureBindingName = in_textureBinding.getDataBindingName();
					glGenTextures(1, &textureId);
					std::cout << "StringedMTextureBinding: created texture with ID: " << textureId << std::endl;
				}

				void deleteTextureResource()
				{
					glDeleteTextures(1, &textureId);
				}

				std::string getStringedDataBindingName()
				{
					return textureBindingName;
				}

				GLuint getTextureId()
				{
					return textureId;
				}
			private:
				std::string textureBindingName = "";
				GLuint textureId = 0;
				Message textureBindingMessage;

		};

		// used with MAPIObjectType::FBO, designed for OpenGL frame buffer objects.
		// Data should go into the string-keyed fboResourceMap.
		struct StringedMFBOBinding
		{
			public:
				StringedMFBOBinding() {};
				StringedMFBOBinding(MAPIObjectData in_textureBinding)
				{
					fboBindingName = in_textureBinding.getDataBindingName();
					glGenFramebuffers(1, &fboBindingId);
				}

				// NOTE: make sure all textures and other objects attached to the FBO are also deleted! 
				// (order of deletion will have to be tested -- textures first? or FBO then textures?)
				void deleteFBOResource()
				{
					glDeleteFramebuffers(1, &fboBindingId);
				}

				GLuint getFBOId()
				{
					return fboBindingId;
				}

			private: 
				std::string fboBindingName = "";
				GLuint fboBindingId = 0;
				Message fboBindingMessage;
		};


		// maps for string-keyed bindings
		std::unordered_map<std::string, StringedMBufferBinding> bufferResourceMap;
		std::unordered_map<std::string, StringedMTextureBinding> textureResourceMap;
		std::unordered_map<std::string, StringedMFBOBinding> fboResourceMap;

		// maps for 3d-keyed bindings
		std::unordered_map<std::string, ThreeDKeyedBufferBindingMap> stringedThreeDBufferMaps;	// a string-keyed map of ThreeDKeyedBufferBindingMap; 
																								// allows for the capability to store multiple 3d-keyed maps.
};

#endif
