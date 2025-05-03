#pragma once

#ifndef GRAYSCALEMG_H
#define GRAYSCALEMG_H

#include "MGearBase.h"

/*
*
* Description: rendering MGear that can/should do experimental/fun stuff with grayscale rendering.

*/

class GrayscaleMG : public MGearBase
{
	public:
		void initializeMGear();
		std::vector<MAPIObjectMetadata> fetchRequiredObjects();	
		void render();
		void cleanupMGear();
		void setObjectBindings();
		void conductMAPIObjectScan(bool in_fullscanFlag, std::vector<MAPIObjectUpdate>* in_updateVectorRef);

	private:
		GLuint grayscaleVertexShaderId = 0;
		GLuint grayscaleFragmentShaderId = 0;
};

#endif