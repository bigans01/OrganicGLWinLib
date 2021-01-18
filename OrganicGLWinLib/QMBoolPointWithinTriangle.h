#pragma once

#ifndef QMBOOLPOINTWITHINTRIANGLE_H
#define QMBOOLPOINTWITHINTRIANGLE_H

#include "QMBase.h"
#include "ThreeDLineSegment.h"
#include <stack>
#include "QuatRotationType.h"
#include "OrganicGLWinUtils.h"

class QMBoolPointWithinTriangle : public QMBase
{
	public:
		bool solve(QuatRotationPoints* in_quatRotationPointsRef);
	private:
		void runExecutionsForFindingCentroidFacingNormal(QuatRotationPoints* in_quatRotationPoints);	// generates the centroid-facing normals for the triangle that we will run the test against
		bool runTriangleLinePlanarityTestForPoint(QuatRotationPoints* in_quatRotationPoints);           // takes the centroid-facing normals to see if a point passes the planarity test
		void rotateLineToZPlane(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderStackRef);
		void rotateLineAroundZToYZero(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef);
		void rotateLineAroundYToPosXAndPushIntoStack(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef);
		float getRadiansForRotateToPosYViaX(glm::vec3 in_vec3);
		bool runRotationsAndPlanarityTest(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef);
};

#endif
