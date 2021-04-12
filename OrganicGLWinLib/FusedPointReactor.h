#pragma once

#ifndef FUSEDPOINTREACTOR_H
#define FUSEDPOINTREACTOR_H

#include "FusionAnalysis.h"
#include "FusedPointContainer.h"
#include <iostream>
#include "FusedPointMetaPair.h"
#include "FusedPointMeta.h"
#include "HostLineReactor.h"
#include "SharedLineReactor.h"
#include "GuestLineReactor.h"
#include "CategorizedLine.h"
#include "FusedPointReactorResult.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"

class FusedPointReactor
{
	public:
		FusedPointReactor(FusionAnalysis* in_hostFusionAnalysisRef, FusionAnalysis* in_guestFusionAnalysisRef, PolyDebugLevel in_polyDebugLevel) :
			hostFusionAnalysisRef(in_hostFusionAnalysisRef),
			guestFusionAnalysisRef(in_guestFusionAnalysisRef),
			fusedPointReactorDebugLevel(in_polyDebugLevel)
		{
			fusedPointReactorLogger.setDebugLevel(in_polyDebugLevel);
			igniteReaction();
		};
		FusionAnalysis* hostFusionAnalysisRef = nullptr;
		FusionAnalysis* guestFusionAnalysisRef = nullptr;
		FusedPointReactorResult getReactorResult();
	private:
		FusedPointContainer reactorPointContainer;
		bool fusionContinuationFlag = true;		// default is true, until it is flagged as false through various cases.
		void runValidityTests();
		void runPointCountTest();
		void runPointAcquisitionAndPointUniquenessTest();
		
		void runCategorizedLineBaseTypeAnalysis();

		void igniteReaction();
		CategorizedLine producedLine;
		PolyLogger fusedPointReactorLogger;
		PolyDebugLevel fusedPointReactorDebugLevel;
		bool performDistanceTest();

};

#endif
