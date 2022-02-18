#pragma once

#ifndef POSYFACERESOLVER_H
#define POSYFACERESOLVER_H

#include "FaceResolverBase.h"
#include "XDimLine.h"
#include "ZDimLine.h"
#include "CategorizedLine.h"
#include "CSCorrectionCandidate.h"
#include "SPolyFracturer.h"

class PosYFaceResolver : public FaceResolverBase
{
	public:
		void setupBorderLineRangesAndDimLoc();
		void runResolutionAlgorithm();
		void produceMalformedMitigation();
	private:
		float yLocation = 0.0f;		// can be 1, 4, or 32; will be same as dimensionalLimit value, since we are at POS_Y
		bool attemptSolveByInvalidCount();	// the basis for solving will be based on the number of lines
};

#endif
