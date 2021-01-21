#pragma once

#ifndef GUESTLINEREACTOR_H
#define GUESTLINEREACTOR_H

#include "LineReactorBase.h"
#include "FusionAnalysis.h"
#include "FusedPointMetaPair.h"
#include <iostream>
#include <vector>
#include "FusedPointSubData.h"
#include "SPoly.h"

class GuestLineReactor : public LineReactorBase
{
	public:
		GuestLineReactor(FusionAnalysis* in_guestFusionAnalysisRef, FusedPointMetaPair* in_fusedPointMetaPairRef) :
			guestFusionAnalysisRef(in_guestFusionAnalysisRef),
			fusedPointMetaRef(in_fusedPointMetaPairRef)
		{
			buildNonBorderLine();
		};
	private:
		FusionAnalysis* guestFusionAnalysisRef = nullptr;
		FusedPointMetaPair* fusedPointMetaRef = nullptr;
		void buildNonBorderLine();
};

#endif
