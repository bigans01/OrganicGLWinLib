#pragma once

#ifndef FUSEDPOINTREACTOR_H
#define FUSEDPOINTREACTOR_H

#include "FusionAnalysis.h"
#include "FusedPointContainer.h"
#include <iostream>
#include "FusedPointMetaPair.h"
#include "FusedPointMeta.h"
#include "HostLineReactor.h"

class FusedPointReactor
{
	public:
		FusedPointReactor(FusionAnalysis* in_hostFusionAnalysisRef, FusionAnalysis* in_guestFusionAnalysisRef) :
			hostFusionAnalysisRef(in_hostFusionAnalysisRef),
			guestFusionAnalysisRef(in_guestFusionAnalysisRef)
		{
			igniteReaction();
		};
		FusionAnalysis* hostFusionAnalysisRef = nullptr;
		FusionAnalysis* guestFusionAnalysisRef = nullptr;
	private:
		FusedPointContainer reactorPointContainer;
		bool fusionContinuationFlag = true;		// default is true, until it is flagged as false through various cases.
		void runValidityTests();
		void runPointCountTest();
		void runPointAcquisitionAndPointUniquenessTest();
		
		void runCategorizedLineBaseTypeAnalysis();

		void igniteReaction();
};

#endif
