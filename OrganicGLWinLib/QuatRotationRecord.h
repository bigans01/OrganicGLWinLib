#pragma once

#ifndef QUATROTATIONRECORD_H
#define QUATROTATIONRECORD_H

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class QuatRotationRecord
{
public:
	const float fullRadian360 = 6.28319;	// 360 degrees = this many radians
	glm::quat originalQuat;	// the original quaternion used when this record was generated
	float originalQuatRadians;
	glm::vec3 quatRotationVector;
	QuatRotationRecord(float in_quatRadians, glm::vec3 in_rotationVector);	// constructor; stores the original quat recorded when 

	glm::quat returnOriginalRotation();
	glm::quat returnReverseRotation();
};

#endif
