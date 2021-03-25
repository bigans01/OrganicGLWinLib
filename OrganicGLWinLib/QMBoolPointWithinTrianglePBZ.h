#pragma once

#ifndef QMBoolPointWithinTrianglePBZ_H
#define QMBoolPointWithinTrianglePBZ_H

#include "QMBoolBase.h"
#include "ThreeDLineSegment.h"
#include <stack>
#include "QuatRotationType.h"
#include "OrganicGLWinUtils.h"

class QMBoolPointWithinTrianglePBZ : public QMBoolBase
{
	public:
		bool solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel);
	private:
		void runExecutionsForFindingCentroidFacingNormal(QuatRotationPoints* in_quatRotationPoints);	// generates the centroid-facing normals for the triangle that we will run the test against
		bool runTriangleLinePlanarityTestForPoint(QuatRotationPoints* in_quatRotationPoints);           // takes the centroid-facing normals to see if a point passes the planarity test
		void rotateLineToZPlane(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderStackRef);
		void rotateLineAroundZToYZero(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef);
		void rotateLineAroundYToPosXAndPushIntoStack(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef);
		float getRadiansForRotateToPosYViaX(glm::vec3 in_vec3);
		bool runRotationsAndPlanarityTest(glm::vec3* in_pointToRotateFor, std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef, std::vector<QuatRotationType>* in_rotationOrderVectorRef);
		glm::vec3 roundPointToThousandths(glm::vec3 in_vec3);
		glm::vec3 roundPointToTenThousandths(glm::vec3 in_vec3);
};

#endif
