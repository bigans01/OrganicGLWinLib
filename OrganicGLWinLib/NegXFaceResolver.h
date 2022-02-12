#pragma once

#ifndef NEGXFACERESOLVER_H
#define NEGXFACERESOLVER_H

#include "FaceResolverBase.h"
#include "YDimLine.h"
#include "ZDimLine.h"
#include "CategorizedLine.h"
#include "CSCorrectionCandidate.h"
#include "SPolyFracturer.h"

class NegXFaceResolver : public FaceResolverBase
{
	public:
		void setupBorderLineRangesAndDimLoc();
		void runResolutionAlgorithm();
		void produceMalformedMitigation();
	private:
		float xLocation = 0.0f;		// since we're NEG_X, this doesn't ever need to change
		bool attemptSolveByInvalidCount();
};

#endif