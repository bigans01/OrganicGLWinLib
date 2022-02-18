#pragma once

#ifndef NEGYFACERESOLVER_H
#define NEGYFACERESOLVER_H

#include "FaceResolverBase.h"
#include "XDimLine.h"
#include "ZDimLine.h"
#include "CategorizedLine.h"
#include "CSCorrectionCandidate.h"
#include "SPolyFracturer.h"

class NegYFaceResolver : public FaceResolverBase
{
	public:
		void setupBorderLineRangesAndDimLoc();
		void runResolutionAlgorithm();
		void produceMalformedMitigation();
	private:
		float yLocation = 0.0f;		// since we're NEG_Y, this doesn't ever need to change
		bool attemptSolveByInvalidCount();	// the basis for solving will be based on the number of lines
};

#endif