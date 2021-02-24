#include "stdafx.h"
#include "QMVec3ResultBase.h"

glm::quat QMVec3ResultBase::createQuaternion(float radians, glm::vec3 in_angle)
{
	glm::quat returnQuat;

	// a = angle of rotation, in radians
	// R = [cos(a/2), sin(a/2)*x, sin(a/2)*y, sin(a/2)*z] quaternion formula
	// 90 degrees = 1.5708 radians
	returnQuat = glm::quat(cos(radians / 2), sin(radians / 2)*in_angle.x, sin(radians / 2)*in_angle.y, sin(radians / 2)*in_angle.z);
	return returnQuat;
}

void QMVec3ResultBase::rotatePointsToOriginalPosition(std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef)
{
	while (!(*in_quatRotationRecordStackRef).empty())
	{
		QuatRotationRecord currentRecord = (*in_quatRotationRecordStackRef).top();
		//std::cout << "################# Reversing values... " << std::endl;
		glm::quat reverseQuat = currentRecord.returnReverseRotation();
		in_quatRotationPointsRef->applyQuaternion(reverseQuat);
		//rotationpointsRefVector->printPoints();

		if (qmVec3BaseLogger.isLoggingSet() == true)
		{
			qmVec3BaseLogger.log("(QMVec3ResultBase) >>> starting printing of quat points( ** AFTER APPLYING A QUAT-RECORD #########) ", "\n");
			in_quatRotationPointsRef->printPoints();
			qmVec3BaseLogger.log("(QMVec3ResultBase) >>> finished printing of quat points( ** AFTER APPLYING A QUAT-RECORD #########)", "\n");
		}


		(*in_quatRotationRecordStackRef).pop();
	}
}

void QMVec3ResultBase::flipOnXAxis(QuatRotationPoints* in_quatRotationPointsRef)
{
	glm::vec3 rotationAroundX;
	rotationAroundX.x = -1.0f;
	float rotateRadians = 6.28319 / 2;
	QuatRotationRecord s1record(rotateRadians, rotationAroundX);
	glm::quat originalQuat = s1record.returnOriginalRotation();
	in_quatRotationPointsRef->applyQuaternion(originalQuat);	// rotate all values by this one
}