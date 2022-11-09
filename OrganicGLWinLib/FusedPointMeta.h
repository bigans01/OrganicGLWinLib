#pragma once

#ifndef FUSEDPOINTMETA_H
#define FUSEDPOINTMETA_H

#include "FusedPointSubdataLineType.h"
#include "FusedPointSummary.h"

class FusedPointMeta
{
	public:
		glm::vec3 point;
		FusionCandidateOrigin origin = FusionCandidateOrigin::NONE;							// will need to be set as either HOST or GUEST
		int numberOfSubdataEntries = 0;
		FusedPointSubdataLineType highestLineType = FusedPointSubdataLineType::NONBORDER;	// will be set to BORDER if a border line is ever detected.
		FusedPointSummary summary;
		void printSummary()
		{
			std::cout << "| Point: " << point.x << ", " << point.y << ", " << point.z << " | ";
			if (origin == FusionCandidateOrigin::GUEST)
			{
				std::cout << "Origin: GUEST | ";
			}
			else if (origin == FusionCandidateOrigin::HOST)
			{
				std::cout << "Origin: HOST | ";
			}

			if (summary == FusedPointSummary::PRECISE_BORDERLINE)
			{
				std::cout << "PRECISE_BORDERLINE" << std::endl;
			}
			else if (summary == FusedPointSummary::PRECISE_NONBORDERLINE)
			{
				std::cout << "PRECISE_NONBORDERLINE" << std::endl;
			}
			else if (summary == FusedPointSummary::TYPICAL_BORDERLINE)
			{
				std::cout << "TYPICAL_BORDERLINE" << std::endl;
			}
			else if (summary == FusedPointSummary::TYPICAL_NONBORDERLINE)
			{
				std::cout << "TYPICAL_NONBORDERLINE" << std::endl;
			}
			else if (summary == FusedPointSummary::PRECISE_MIXED)
			{
				std::cout << "PRECISE_MIXED" << std::endl;
			}

		}
};

#endif
