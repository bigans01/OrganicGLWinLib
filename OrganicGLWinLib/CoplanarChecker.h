#pragma once

#ifndef COPLANARCHECKER_H
#define COPLANARCHECKER_H

#include "QuatRotationPoints.h"
#include "QuatRotationManager.h"
#include "SPoly.h"
#include "CoplanarComparisonCandidatePoints.h"
#include "PointTranslationCheck.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"

class CoplanarChecker
{
	public:
		CoplanarChecker(SPoly* in_polyA, SPoly* in_polyB, PolyDebugLevel in_polyDebugLevel) :
			polyAPtr(in_polyA),
			polyBPtr(in_polyB),
			coplanarCheckerLogLevel(in_polyDebugLevel)
		{
			coplanarCheckerLogger.setDebugLevel(in_polyDebugLevel);
			performCoPlanarAnalysis();
		}
		bool coplanarityDetected = false;	// defaults to false.
	private:
		SPoly* polyAPtr = nullptr;
		SPoly* polyBPtr = nullptr;
		PolyDebugLevel coplanarCheckerLogLevel = PolyDebugLevel::NONE;
		PolyLogger coplanarCheckerLogger;
		void performCoPlanarAnalysis();

};

#endif
