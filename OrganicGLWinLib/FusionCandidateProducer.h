#pragma once

#ifndef FUSIONCANDIDATEPRODUCER_H
#define FUSIONCANDIDATEPRODUCER_H

#include "FusionCandidate.h"
#include "STriangle.h"
#include "STriangleLine.h"
#include <iostream>
#include "OrganicGLWinUtils.h"


class FusionCandidateProducer
{
	public:
		FusionCandidate produceCandidate(STriangle in_sTriangle, STriangleLine in_sTriangleLine);
	private:
		IntersectionResult determineRayRelationShipToTriangle(STriangle in_triangle, STriangleLine in_line);
		glm::vec3 roundPointToHundredths(glm::vec3 in_point);
		double doubledot(glm::vec3 in_A, glm::vec3 in_B);

};

#endif
