#pragma once

#ifndef COPLANARCHECKER_H
#define COPLANARCHECKER_H

#include "QuatRotationPoints.h"
#include "QuatRotationManager.h"
#include "SPoly.h"
#include "CoplanarComparisonCandidatePoints.h"
#include "PointTranslationCheck.h"

class CoplanarChecker
{
	public:
		CoplanarChecker(SPoly* in_polyA, SPoly* in_polyB) :
			polyAPtr(in_polyA),
			polyBPtr(in_polyB)
		{
			performCoPlanarAnalysis();
		}
		bool coplanarityDetected = false;	// defaults to false.
	private:
		SPoly* polyAPtr = nullptr;
		SPoly* polyBPtr = nullptr;
		void performCoPlanarAnalysis();

};

#endif
