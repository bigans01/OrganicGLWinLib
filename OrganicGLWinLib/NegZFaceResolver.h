#pragma once

#ifndef NEGZFACERESOLVER_H
#define NEGZFACERESOLVER_H

#include "FaceResolverBase.h"
#include "XDimLine.h"
#include "YDimLine.h"
#include "CategorizedLine.h"
#include "CSCorrectionCandidate.h"
#include "SPolyFracturer.h"

class NegZFaceResolver : public FaceResolverBase
{
	public:
		void setupBorderLineRangesAndDimLoc();
		void runResolutionAlgorithm();
		void produceMalformedMitigation();
	private:
		float zLocation = 0.0f;		// since we're NEG_Z, this doesn't ever need to change
		bool attemptSolveByInvalidCount();
};

#endif
