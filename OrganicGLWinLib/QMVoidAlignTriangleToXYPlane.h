#pragma once

#ifndef QMVOIDALIGNTRIANGLETOXYPLANE_H
#define QMVOIDALIGNTRIANGLETOXYPLANE_H

#include "SPolyScalingOps.h"
#include "QuatRotationPoints.h"
#include "QuatRotationType.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class QMVoidAlignTriangleToXYPlane
{
	public:
		void solve(SPolyScalingOps* in_scalingOpsRef, QuatRotationPoints* in_quatRotationPointsRef);
	private:
		SPolyScalingOps* scalingOpsRef = nullptr;
		QuatRotationPoints* quatRotationPointsRef = nullptr;

		glm::vec3* pointARef = nullptr;			// point A of the triangle
		glm::vec3* pointBRef = nullptr;			// point B "
		glm::vec3* pointCRef = nullptr;			// point C "
		std::vector<QuatRotationType> rotationTypeVector;

		void alignFirstLineToXYPlane();
		void rotateFirstLineAroundYAndPushIntoStack();
		void rotateFirstLineAroundZAndPushIntoStack();

		void alignSecondLineToXYPlane();
		void rotateSecondLineAroundXToXYPlane();
		float findRotationRadiansForSecondLine(glm::vec3 in_vec3);
		glm::quat createQuaternion(float radians, glm::vec3 in_angle);
};

#endif
