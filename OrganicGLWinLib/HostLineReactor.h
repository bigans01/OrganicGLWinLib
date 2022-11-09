#pragma once

#ifndef HOSTLINEREACTOR_H
#define HOSTLINEREACTOR_H

#include "LineReactorBase.h"
#include "FusionAnalysis.h"
#include "FusedPointMetaPair.h"
#include "FusedPointSubData.h"
#include "SPoly.h"
#include "PolyDebugLevel.h"

class HostLineReactor : public LineReactorBase
{
	public:
		HostLineReactor(FusionAnalysis* in_hostFusionAnalysisRef, FusionAnalysis* in_guestFusionAnalysisRef, FusedPointMetaPair* in_fusedPointMetaPairRef, PolyDebugLevel in_polyDebugLevel) :
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
		void buildASliceSingleInterceptsPointPrecise(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint);
		void buildPartialBound(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint);
		void buildASlice(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint);
		void buildNonBound(glm::vec3 in_buildStartPoint, glm::vec3 in_otherPoint);
};

#endif
