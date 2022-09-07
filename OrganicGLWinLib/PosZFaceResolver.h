#pragma once

#ifndef POSZFACERESOLVER_H
#define POSZFACERESOLVER_H

#include "FaceResolverBase.h"
#include "XDimLine.h"
#include "YDimLine.h"
#include "CategorizedLine.h"
#include "CSCorrectionCandidate.h"
#include "SPolyFracturer.h"

class PosZFaceResolver : public FaceResolverBase
{
	public:
		void setupBorderLineRangesAndDimLoc();
		void runResolutionAlgorithm();
		void produceMalformedMitigation();
	private:
		float zLocation = 0.0f;		// can be 1, 4, or 32; will be same as dimensionalLimit value, since we are at POS_Z
		bool attemptSolveByInvalidCount();
};

#endif
