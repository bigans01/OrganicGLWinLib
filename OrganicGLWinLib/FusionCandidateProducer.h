#pragma once

#ifndef FUSIONCANDIDATEPRODUCER_H
#define FUSIONCANDIDATEPRODUCER_H

#include "FusionCandidate.h"
#include "STriangle.h"
#include "STriangleLine.h"
#include <iostream>
#include "OrganicGLWinUtils.h"
#include "RayIntersectionResult.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "QuatUtils.h"


class FusionCandidateProducer
{
	public:
		FusionCandidateProducer(PolyDebugLevel in_debugOption);
		FusionCandidate produceCandidate(STriangle in_sTriangle, STriangleLine in_sTriangleLine);
	private:
		RayIntersectionResult determineRayRelationShipToTriangle(STriangle in_triangle, STriangleLine in_line);
		glm::vec3 roundPointToHundredths(glm::vec3 in_point);
		glm::vec3 roundPointToTenThousandths(glm::vec3 in_point);
		double doubledot(glm::vec3 in_A, glm::vec3 in_B);
		bool isPointEqualToTrianglePoint(glm::vec3 in_point, STriangle* in_triangleRef);
		PolyLogger candidateProductionLogger;
};

#endif
