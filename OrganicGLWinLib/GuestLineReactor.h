#pragma once

#ifndef GUESTLINEREACTOR_H
#define GUESTLINEREACTOR_H

#include "LineReactorBase.h"
#include "FusionAnalysis.h"
#include "FusedPointMetaPair.h"
#include "FusedPointSubData.h"
#include "SPoly.h"

class GuestLineReactor : public LineReactorBase
{
	public:
		GuestLineReactor(FusionAnalysis* in_guestFusionAnalysisRef, FusedPointMetaPair* in_fusedPointMetaPairRef, PolyDebugLevel in_polyDebugLevel) :
			guestFusionAnalysisRef(in_guestFusionAnalysisRef),
			fusedPointMetaRef(in_fusedPointMetaPairRef)
		{
			reactorBaseLogger.setDebugLevel(in_polyDebugLevel);
			buildNonBorderLine();
		};
	private:
		FusionAnalysis* guestFusionAnalysisRef = nullptr;
		FusedPointMetaPair* fusedPointMetaRef = nullptr;
		void buildNonBorderLine();
};

#endif
