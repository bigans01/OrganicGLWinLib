#pragma once

#ifndef SHAREDLINEREACTOR_H
#define SHAREDLINEREACTOR_H

#include "LineReactorBase.h"
#include "FusionAnalysis.h"
#include "FusedPointMetaPair.h"
#include <iostream>
#include <vector>
#include "FusedPointSubData.h"
#include "SPoly.h"

class SharedLineReactor : public LineReactorBase
{
	public:
		SharedLineReactor(FusionAnalysis* in_hostFusionAnalysisRef, FusionAnalysis* in_guestFusionAnalysisRef, FusedPointMetaPair* in_fusedPointMetaPairRef, PolyDebugLevel in_polyDebugLevel) :
			hostFusionAnalysisRef(in_hostFusionAnalysisRef),
			guestFusionAnalysisRef(in_guestFusionAnalysisRef),
			fusedPointMetaRef(in_fusedPointMetaPairRef)
		{
			reactorBaseLogger.setDebugLevel(in_polyDebugLevel);
			runAnalysis();
		};
	private:
		FusionAnalysis* hostFusionAnalysisRef = nullptr;
		FusionAnalysis* guestFusionAnalysisRef = nullptr;
		FusedPointMetaPair* fusedPointMetaRef = nullptr;
		void runAnalysis();

		void buildInterceptsPointPrecise(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint);
		void buildPartialBound(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint);
		void buildNonBound(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint);
};

#endif
