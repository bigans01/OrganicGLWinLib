#pragma once

#ifndef QMVec3FindOrientatedLineNormal_H
#define QMVec3FindOrientatedLineNormal_H

#include "QMVec3Base.h"

class QMVec3FindOrientatedLineNormal : public QMVec3Base
{
	public:
		glm::vec3 solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
	private:
		void runExecutionsForFindingCentroidFacingNormal(QuatRotationPoints* in_quatRotationPoints);
		void rotateLineAroundZToYZero(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef);
		void rotateLineAroundYToPosXAndPushIntoStack(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef);
		void rotateLineToZPlane(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef);
		float getRadiansForRotateToPosYViaX(glm::vec3 in_vec3);
};

#endif
