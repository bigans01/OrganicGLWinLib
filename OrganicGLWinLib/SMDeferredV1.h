#pragma once

#ifndef SMDEFERREDV1_H
#define SMDEFERREDV1_H

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderMachineBase.h"
#include "Gear.h"
#include "OrganicGLWinUtils.h"
#include <map>
#include <unordered_map>
#include <mutex>
#include "TerrainGearT1.h"
#include "HighlighterGearT1.h"

class SMDeferredV1 : public ShaderMachineBase
{
public:
	// virtual functions (defined, inherited)
	void initialize(int in_windowWidth, int in_windowHeight, int in_immutableBufferSize);
	//void setupTextureAtlas(AtlasMap* in_atlasMapRef, AtlasPropertiesGL* in_atlasPropertiesGLRef);	// STEP 2
	void setupTextureAtlases();	// STEP 2
	void runAllShadersAndSwap();
	void runAllShadersNoSwap();
	void shutdownGL();
	void multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections);		// called by OrganicSystem
	void printDataForGears();
	//void updateMVPinGears();
	void insertCollectionGLData(TerrainJobResults in_jobResults, int in_arraySize, GLfloat* in_arrayRef);
	void removeUnusedReplaceables();
	void insertWorldLight(WorldLight in_worldLight);
private:
	//std::map<int, GLuint> persistentBufferMap;							// map that stores IDs of persistent buffers
	//std::map<int, std::unique_ptr<Gear>> gearTrain;						// map that stores individual OpenGL programs (aka, "Gears"). GearTrain is borrowed from an engineering term.
	//std::unordered_map<std::string, int> bufferLookup;					// used to look up buffer IDs

	//void insertNewPersistentBuffer(std::string in_bufferName, int in_size);
	void insertTerrainGear(int in_gearID, GLuint in_programID);
	void insertHighlighterGear(int in_gearID, GLuint in_programID);

	void setupDeferredFBO();		// sets up the deferred FBO, according to this Machine's needs/specifications

	void createGBufText(GLenum texUnit, GLenum  format, GLuint &texid);
	void updateUniformRegistry();	// updates the uniform registry with all the appropriate values

	//void setupAlternativeDepthTexture();
};

#endif
