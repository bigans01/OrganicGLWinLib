#pragma once

#ifndef BASICMG_H
#define BASICMG_H

#include "MGearBase.h"

/*
* 
* Description: simple, basic rendering MGear.

*/

class BasicMG : public MGearBase
{
	public:
		void initializeMGear();
		std::vector<MAPIObjectMetadata> fetchRequiredObjects();
		void render();
		void cleanupMGear();
		void setObjectBindings();
		void conductMAPIObjectScan(bool in_fullscanFlag, std::vector<MAPIObjectUpdate>* in_updateVectorRef);
	private:
		GLuint basicVertexShaderId = 0;
		GLuint basicFragmentShaderId = 0;
};

#endif
