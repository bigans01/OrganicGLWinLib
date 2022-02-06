#pragma once

#ifndef POSYFACERESOLVER_H
#define POSYFACERESOLVER_H

#include "FaceResolverBase.h"

class PosYFaceResolver : public FaceResolverBase
{
	public:
		void setupBorderLineRangesAndDimLoc();
		void runResolutionAlgorithm();
		void produceMalformedMitigation();
	private:
		float yLocation = 0.0f;		// can be 1, 4, or 32; will be same as dimensionalLimit value, since we are at POS_Y
};

#endif
