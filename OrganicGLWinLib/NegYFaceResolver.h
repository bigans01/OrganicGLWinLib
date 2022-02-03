#pragma once

#ifndef NEGYFACERESOLVER_H
#define NEGYFACERESOLVER_H

#include "FaceResolverBase.h"
class NegYFaceResolver : public FaceResolverBase
{
	public:
		void setupBorderLineRangesAndDimLoc();
	private:
		float yLocation = 0.0f;		// since we're NEG_Y, this doesn't ever need to change
};

#endif