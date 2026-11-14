#pragma once

#ifndef BASICGRAYSCALEMG_H
#define BASICGRAYSCALEMG_H

#include "MGearBase.h"
#include <mutex>
/*
*
* Description: rendering MGear that can/should do experimental/fun stuff with grayscale rendering; use with the MSBasicGrayscale MShader.
* This MGearBase-child expects the following resources:
* 
* testRenderBuffer -> buffer that contains buffer data for rendering; currently filled with a hard-coded triangle
* commonVAO -> vertex array object for the testRenderBuffer object
* MVP -> typical mat4 uniform found in 3d shaders.

*/

class BasicGrayscaleMG : public MGearBase
{
	public:
		void initializeMGear();		// create shaders, fetch and store program id
		std::vector<MAPIObjectMetadata> fetchRequiredMAPIObjects();	
		std::vector<GLUniformRequest> fetchRequiredGLUniforms();
		void render();			// call setupVAO the very first time only, and render data.
		void cleanupMGear();	// delete corresponding shaders and program ID.
		void setObjectBindings();
		void conductMAPIObjectScan(bool in_fullscanFlag, std::vector<MAPIObjectUpdate>* in_updateVectorRef);	// scan for updates to required objects

	private:
		GLuint grayscaleVertexShaderId = 0;
		GLuint grayscaleFragmentShaderId = 0;

		bool vaoSetup = false;	// false indicates the VAO should be setup; set to true when it's setup.
		int vaoId = 0;		// stores the VAO ID of the expected commonVAO resource; set in setupVAO()
		int grayscaleMainBufferId = 0;	// stores the buffer ID value of the expected testRenderBuffer resource; set in setupVAO()

		void setupVAO(); // binds the testRenderBuffer and commonVAO vertex array object, then builds the VAO attributes.

		std::unique_ptr<GLfloat[]> testPointArray;
};

#endif