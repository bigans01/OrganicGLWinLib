#include "stdafx.h"
#include "QMBoolBase.h"

glm::vec3 QMBoolBase::findTriangleCentroid(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2)
{
	glm::vec3 foundCentroid;
	foundCentroid.x = (in_point0.x + in_point1.x + in_point2.x) / 3;
	foundCentroid.y = (in_point0.y + in_point1.y + in_point2.y) / 3;
	foundCentroid.z = (in_point0.z + in_point1.z + in_point2.z) / 3;
	//std::cout << "Centroid X: " << foundCentroid.x << std::endl;
	//std::cout << "Centroid y: " << foundCentroid.y << std::endl;
	return foundCentroid;
}

glm::quat QMBoolBase::createQuaternion(float radians, glm::vec3 in_angle)
{
	glm::quat returnQuat;

	// a = angle of rotation, in radians
	// R = [cos(a/2), sin(a/2)*x, sin(a/2)*y, sin(a/2)*z] quaternion formula
	// 90 degrees = 1.5708 radians
	returnQuat = glm::quat(cos(radians / 2), sin(radians / 2)*in_angle.x, sin(radians / 2)*in_angle.y, sin(radians / 2)*in_angle.z);
	return returnQuat;
}

void QMBoolBase::rotatePointsToOriginalPosition(std::stack<QuatRotationRecord>* in_quatRotationRecordStackRef, QuatRotationPoints* in_quatRotationPointsRef)
{
	while (!(*in_quatRotationRecordStackRef).empty())
	{
		QuatRotationRecord currentRecord = (*in_quatRotationRecordStackRef).top();
		//std::cout << "################# Reversing values... " << std::endl;
		glm::quat reverseQuat = currentRecord.returnReverseRotation();
		in_quatRotationPointsRef->applyQuaternion(reverseQuat);
		//rotationpointsRefVector->printPoints();

		if (qmBoolBaseLogger.isLoggingSet() == true)
		{
			qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) >>> starting printing of quat points( ** AFTER APPLYING A QUAT-RECORD #########) ", "\n");
			in_quatRotationPointsRef->printPoints();
			qmBoolBaseLogger.log("(QMBoolPointWithinTrianglePBZ) >>> finished printing of quat points( ** AFTER APPLYING A QUAT-RECORD #########)", "\n");
		}


		(*in_quatRotationRecordStackRef).pop();
	}
}

void QMBoolBase::flipOnXAxis(QuatRotationPoints* in_quatRotationPointsRef)
{
	glm::vec3 rotationAroundX;
	rotationAroundX.x = -1.0f;
	float rotateRadians = 6.28319f / 2;
	QuatRotationRecord s1record(rotateRadians, rotationAroundX);
	glm::quat originalQuat = s1record.returnOriginalRotation();
	in_quatRotationPointsRef->applyQuaternion(originalQuat);	// rotate all values by this one
}