#pragma once

#ifndef POSXFACERESOLVER_H
#define POSXFACERESOLVER_H

#include "FaceResolverBase.h"
#include "YDimLine.h"
#include "ZDimLine.h"

class PosXFaceResolver : public FaceResolverBase
{
	public:
		void setupBorderLineRangesAndDimLoc();
	private:
		float xLocation = 0.0f;		// can be 1, 4, or 32; will be same as dimensionalLimit value, since we are at POS_X
};

#endif
