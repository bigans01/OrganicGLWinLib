#pragma once

#ifndef COPLANARCOMPARISONCANDIDATEPOINTS_H
#define COPLANARCOMPARISONCANDIDATEPOINTS_H

#include "STriangle.h"

class CoplanarComparisonCoandidatePoints
{
	public:
		CoplanarComparisonCoandidatePoints(STriangle* in_sTriangleRef)
		{
			for (int x = 0; x < 3; x++)
			{
				points[x] = in_sTriangleRef->triangleLines[x].pointA;
				availableCandidates.insert(x);
			}
		};
		void removeCandidate(int in_pointIndex)
		{
			availableCandidates.erase(in_pointIndex);
		}

		glm::vec3 getFirstAvailableCandidate()
		{
			auto firstCandidate = availableCandidates.begin();
			return points[*firstCandidate];
		}
		std::map<int, glm::vec3> points;
		std::set<int> availableCandidates;
};

#endif

