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
		void render();
		void cleanupMGear();
	private:
		GLuint basicVertexShaderId = 0;
		GLuint basicFragmentShaderId = 0;
};

#endif
