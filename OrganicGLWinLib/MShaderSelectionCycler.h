#pragma once

#ifndef MSHADERSELECTIONCYCLER_H
#define MSHADERSELECTIONCYCLER_H

#include "MShaderBase.h"
#include "MShaderCatalog.h"

/*

Description: a simple array used to keep track of MShaders that are currently in use, or have been used. It is one of the primary components 
required to enable the "transiting" feature, as it allows for both the previous and target shaders to be tracked for such a transition.

*/

class MShaderSelectionCycler
{
	public:
		MShaderSelectionCycler() {};	// default constructor, so that this class can be happily inserted into MShaderController.
		void setCatalogRef(MShaderCatalog* in_cyclerCatalogRef);	// sets the value of cyclerCatalogRef; this function should be called
																	// first, before any other functions besides the constructor (so make sure in the constructor
																	// of the MShaderController)
		Message switchToMShader(std::string in_shaderName);
		MShaderBase* getTargetShaderRef();		// should return the value at cycleArray[0], which will be the current MShader we are on/transitioning to.
		MShaderBase* getPreviousShaderRef();	// should return the value at cycleArray[2], which will be the previous MShader we transitioned from.
		int getNumberOfLoadedShaders();
	private:
		MShaderCatalog* cyclerCatalogRef = nullptr;	// must be set by the call to setCatalogRef
		MShaderBase* cycleArray[2];		// the first index at 0 will be the currently used shader; the index at 1 will be the last used shader.
		int currentLoadedShaders = 0;	// keeps track of the number of used shaders.

		void iterateLoadedShaders();	// iterate up to two times, so that the value becomes 2 (the size of cycleArray); should not go beyond 2.
};

#endif
