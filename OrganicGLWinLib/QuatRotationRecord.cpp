#include "stdafx.h"
#include "QuatRotationRecord.h"

QuatRotationRecord::QuatRotationRecord(float in_quatRadians, glm::vec3 in_rotationVector)
{
	originalQuat = glm::quat(cos(in_quatRadians / 2), sin(in_quatRadians / 2)*in_rotationVector.x, sin(in_quatRadians / 2)*in_rotationVector.y, sin(in_quatRadians / 2)*in_rotationVector.z);
	originalQuatRadians = in_quatRadians;
	quatRotationVector = in_rotationVector;
	//returnQuat = glm::quat(cos(radians / 2), sin(radians / 2)*in_angle.x, sin(radians / 2)*in_angle.y, sin(radians / 2)*in_angle.z);
}

glm::quat QuatRotationRecord::returnOriginalRotation()
{
	return originalQuat;
}

glm::quat QuatRotationRecord::returnReverseRotation()
{
	// some logic here for reversing
	glm::quat reverseQuat;
	float reverseQuatRadians = fullRadian360 - originalQuatRadians;	// get the amount of radians to rotate by to complete the 360 degree rotation
	reverseQuat = glm::quat(cos(reverseQuatRadians / 2), sin(reverseQuatRadians / 2)*quatRotationVector.x, sin(reverseQuatRadians / 2)*quatRotationVector.y, sin(reverseQuatRadians / 2)*quatRotationVector.z);
	return reverseQuat;
}