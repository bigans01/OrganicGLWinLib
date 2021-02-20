#pragma once

#ifndef QMBOOLFINDCYCLINGDIRECTIONPOINT_H
#define QMBOOLFINDCYCLINGDIRECTIONPOINT_H

#include "QMVec3Base.h"

class QMVec3FindCyclingDirectionPoint : public QMVec3Base
{
	public:
		glm::vec3 solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
	private:
		void rotateLineToYZeroPositiveX(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef);
		glm::vec3 originalCandidatePointA, originalCandidatePointB;	// the original values of points A and B of the line that we'll be finding the cycling direction for
		void roundVec3YByTenThousandths(glm::vec3* in_vec3Ref);
};


#endif
