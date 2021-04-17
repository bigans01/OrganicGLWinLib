#pragma once

#ifndef ORGANICGLWINUTILS
#define ORGANICGLWINUTILS

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII


#define NOMINMAX		// required to use std::min without parenthesis
#include <algorithm>
#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <stdio.h>
#include <math.h>

#include <iostream>
#include <map>
#include <vector>

#include "OrganicShaderLoader.h"
#include "AtlasMetaData.h"
#include "AtlasMap.h"
#include "AtlasPropertiesGL.h"
#include "TileLoadData.h"
#include "PointTranslationCheck.h"
#include "QuatRotationManager.h"
#include "QuatRotationPoints.h"
#include "stb_image.h"
#include "TwoDLineSegment.h"
#include "TwoDPoint.h"
#include "ThreeDLineSegment.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imconfig.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class OrganicGLWinUtils
{
public:
	static void createImmutableBufferMode1(GLuint* in_bufferID, GLuint* in_swapBufferID, int in_bufferSize);		// creates an immutable buffer, and sets appropriate vertexarray attributes for mode 0
	static void createImmutableBufferMode0(GLuint* in_bufferID, GLuint* in_swapBufferID, int in_bufferSize);		// creates an immutable buffer, and sets appropriate vertexarray attributes for mode 1
	static void createImmutableBufferMode2(GLuint* in_bufferID, GLuint* in_swapBufferID, int in_bufferSize,  GLuint* in_textureRef);
	static void createImmutableBuffer(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers);
	static void createImmutableBufferExperimental(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers, GLuint* in_textureRef, GLuint* in_textureRef2);
	static void createImmutableBufferExperimental2(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers, GLuint* in_textureRef);
	static void createImmutableBufferExperimental3(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers, GLuint* in_textureRef);
	static void createFBO(GLuint* in_fboIDref);
	static void createBuffer(GLuint* in_bufferIDref);
	static AtlasMetaData findAtlasMetadata(int in_atlasWidth, int in_tileWidth);
	static void createAndBindVertexArray(GLuint* in_bufferID);													// creates and binds a single vertex array
	static void createAndBindDrawIndirectBuffer(GLuint* in_bufferID);											// creates and binds a single indirect buffer
	static void initializeLibraryAndSetHints();	// initializes the OpenGL library and the window; adds input(keypress) listening for the window
	static void loadShadersViaMode(GLuint* in_programID, int in_mode);
	static void loadShadersViaMode(GLuint* in_programID, std::string in_programName);
	static void loadComputeShader(GLuint* in_programID, std::string in_computeShaderName);
	static void initializeGlew();
	static void computeMatricesFromInputs(GLFWwindow* in_windowRef, float* in_horizontalAngle, float* in_verticalAngle, glm::vec3* in_position, float*  in_initialFoV, float* in_speed, float in_mouseSpeed, glm::mat4* in_projectionMatrix, glm::mat4* in_viewMatrix, glm::mat4* in_MVPref);
	static void setGLFWInputMode(GLFWwindow* in_window);
	static int setupTextureAtlasJPEG(GLuint* in_atlasTextureRef, AtlasMap* in_atlasRef, AtlasPropertiesGL* in_atlasPropertiesGLRef, float* in_atlasTileWidth, float* in_atlasWidth);
	static int setupTextureAtlasJPEG2(GLuint* in_atlasTextureRef, AtlasMap* in_atlasRef, AtlasPropertiesGL* in_atlasPropertiesGLRef);
	static void multiDrawArraysMode0(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, int in_numberOfCollections);
	static void multiDrawArraysMode1(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, int in_numberOfCollections);
	static void multiDrawArraysMode2(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, GLuint* in_textureRef, GLuint* in_textureUniformRef, int in_numberOfCollections);
	static void multiDrawArraysMode3(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, GLuint* in_textureRef, GLuint* in_textureUniformRef, int in_numberOfCollections, GLuint* in_textureWidthRef, glm::vec3* in_textureWidth, GLuint* in_atlasWidthRef, float in_atlasWidth, GLuint* in_atlasTileWidthRef, float in_atlasTileWidth);
	static void shutdownOpenGLBasic(GLuint* in_terrainBufferID, GLuint* in_vertexArrayID, GLuint* in_programID);			// shuts down OpenGL for modes 0 through 2
	static GLFWwindow* createGLFWWindow(int in_width, int in_height);
	static void makeContextCurrent(GLFWwindow* in_window);
	static void checkWindowValidity(GLFWwindow* in_window);
	static void setBasicStates();																				// sets basic OpenGL states
	static void setClearColor(float in_red, float in_green, float in_blue, float in_alpha);						// sets the background color
	static void copyToBuffer(GLuint* in_readBufferID, GLuint* in_writeBufferID, int in_readByteOffset, int in_readByteSize, int in_writeByteOffset);
	static void moveForCopy(GLuint* in_copyBufferID, int in_readByteOffset, int in_readByteSize, int in_targetWriteBackOffset);
	static GLuint loadDDS(const char* imagepath);
	static glm::quat createQuaternion(float in_degrees, glm::vec3 in_unitVector);
	static glm::vec3 roundVec3ToHundredths(glm::vec3 in_vec3);
	static glm::vec3 findTriangleCentroid(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
	static glm::vec3 findTriangleCentroidDebug(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
	static glm::vec3 findTriangleNormal(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
	static TwoDPoint convertGlmVec3To2D(glm::vec3 in_glmvec3);
	static glm::vec3 convert2DToGlmVec3(TwoDPoint in_twoDPoint);

	// IMGUI functions
	static void IMGuiInit(GLFWwindow* in_window);	// set up imgui context
	static void IMGuiNewFrame();	// set up the IMGui frame
	static void IMGuiTestText();	// a text box for testing purposes
	static void IMGuiTextButton();
	static void IMGuiRenderAndDraw();	// render and draw (all?) the IMGui stuff
	static void IMGuiPrepWorldLocation(float world_precise[3], int world_organicLoc[9]);	// will prepare data and run imgui calls for displaying world location
	static void IMGuiPrepBlockLocation(int world_organicLoc[9]);
};

#endif