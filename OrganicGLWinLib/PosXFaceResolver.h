#pragma once

#ifndef POSXFACERESOLVER_H
#define POSXFACERESOLVER_H

#include "FaceResolverBase.h"
#include "YDimLine.h"
#include "ZDimLine.h"
#include "CategorizedLine.h"
#include "CSCorrectionCandidate.h"

class PosXFaceResolver : public FaceResolverBase
{
	public:
		void setupBorderLineRangesAndDimLoc();
		void runResolutionAlgorithm();
		void produceMalformedMitigation();
	private:
		float xLocation = 0.0f;		// can be 1, 4, or 32; will be same as dimensionalLimit value, since we are at POS_X
		bool attemptSolveByInvalidCount();	// the basis for solving will be based on the number of lines
		void checkCleaveSequenceLinesAgainstDimLines(CleaveSequence* in_invalidPtr);
};

#endif
