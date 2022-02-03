#pragma once

#ifndef POSZFACERESOLVER_H
#define POSZFACERESOLVER_H

#include "FaceResolverBase.h"

class PosZFaceResolver : public FaceResolverBase
{
	public:
		void setupBorderLineRangesAndDimLoc();
	private:
		float zLocation = 0.0f;		// can be 1, 4, or 32; will be same as dimensionalLimit value, since we are at POS_Z
};

#endif
