#pragma once

#ifndef MAPPABLEVEC3_H
#define MAPPABLEVEC3_H

#include "PointToMassRelationshipJudge.h"
#include "IndividualVerdict.h"

class MappableJudge
{
	public:
		MappableJudge() {};
		MappableJudge(glm::vec3 in_point) :
			point(in_point)
		{};

		glm::vec3 point;
		PointToMassRelationshipJudge judge;

		bool equals(glm::vec3 in_point)
		{
			return (point == in_point);
		}

		IndividualVerdict runJudgements(PolyDebugLevel in_polyDebugLevel)
		{
			return judge.executeJudgementOnShellSlices(in_polyDebugLevel);	// run the judgements on all shell slices in the underlying judge.
		}
};

#endif
