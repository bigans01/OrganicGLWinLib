#include "stdafx.h"
#include "QMVoidTranslatePointsForRasterization.h"

void QMVoidTranslatePointsForRasterization::solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel)
{
	// Logic:
	// All points need to be translated such that their Z is on the same plane -- this doesn't necessarily need to be 0. This is done by taking the
	// empty normal of the tracked SPoly, and using it in quaternions to meet this condition.

	// set debug level
	qmVoidBaseLogger.setDebugLevel(in_polyDebugLevel);
	qmVoidBaseLoggerDebugLevel = in_polyDebugLevel;

	rotationpointsRefVector = in_quatRotationPointsRef;

	// the last point in the set of points should be the empty normal in the tracked SPoly.
	emptyNormalRef = in_quatRotationPointsRef->getLastPoint();

	// check if we need to rotate around X-axis to get to the normal to 0.
	if (emptyNormalRef.y != 0.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_X;
		rotationOrder.push_back(rotateType);
	}

	// check if we need to rotate around Y-axis to get the normal to positive Z
	if (emptyNormalRef.z != 1.0f)
	{
		QuatRotationType rotateType = QuatRotationType::ROTATE_AROUND_Y;
		rotationOrder.push_back(rotateType);
	}
	
	runRotations();

	//std::cout << "+++++++++++++++++ Done rotating points; points are: " << std::endl;
	//in_quatRotationPointsRef->printPoints();
}

void QMVoidTranslatePointsForRasterization::runRotations()
{
	auto vectorBegin = rotationOrder.begin();
	auto vectorEnd = rotationOrder.end();
	for (; vectorBegin != vectorEnd; vectorBegin++)
	{
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_X)
		{
			rotateEmptyNormalAroundXToYZero();
		}
		if (*vectorBegin == QuatRotationType::ROTATE_AROUND_Y)
		{
			rotateEmptyNormalAroundYToPosZ();
		}
	}
}

void QMVoidTranslatePointsForRasterization::rotateEmptyNormalAroundXToYZero()
{
	glm::vec3 currentNormalValue = rotationpointsRefVector->getLastPoint();	//
	//float radiansToRotateBy = findRotationRadiansForGettingToYZeroThroughX(currentNormalValue);

	//glm::vec3 rotateAroundX;
	//rotateAroundX.x = -1.0f;
	//QuatRotationRecord s1record(radiansToRotateBy, rotateAroundX);

	findRotationRadiansForGettingToYZeroThroughX(currentNormalValue);
}

void QMVoidTranslatePointsForRasterization::findRotationRadiansForGettingToYZeroThroughX(glm::vec3 in_vec3)
{
	float degreesToRotateOnX = 0.0f;
	float fullRadian360 = 6.28319f;	// 360 degrees = this many radians

	// get the atan2 result, and analyze it
	float atan2result = atan2(in_vec3.y, in_vec3.z);
	float firstPassRotateRadians = 0.0f;

	if (atan2result > 0.0)
	{
		//firstPassRotateRadians = fullRadian360 - atan2result;
		firstPassRotateRadians = atan2result;
	}
	else if (atan2result < 0.0) // if a is less than 0, add the result to fullRadian360 to get the amount to rotate by. (the quat goes CW when the rotation axis is pointing in a positive direction)
	{
		//firstPassRotateRadians = abs(atan2result);
		firstPassRotateRadians = fullRadian360 + atan2result;
	}

	glm::vec3 rotationAroundX;
	rotationAroundX.x = -1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundX);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	rotationpointsRefVector->applyQuaternion(originalQuat);	// rotate all values by this one
	rotationRecords.push(s1record);


}

void QMVoidTranslatePointsForRasterization::rotateEmptyNormalAroundYToPosZ()
{
	//std::cout << "(QMVoidTranslatePointsForRasterization): calling rotateEmptyNormalAroundYToPosZ()..." << std::endl;

	glm::vec3 currentNormalValue = rotationpointsRefVector->getLastPoint();	//

	//std::cout << "(QMVoidTranslatePointsForRasterization): normal value before quat application: " << currentNormalValue.x << ", " << currentNormalValue.y << ", " << currentNormalValue.z << std::endl;

	float radiansToRotateBy = findRotationRadiansForGettingToPosZThroughY(currentNormalValue);
	//std::cout << "radians to rotate by value: " << radiansToRotateBy << std::endl;

	glm::vec3 rotateAroundY;
	rotateAroundY.y = -1.0f;
	QuatRotationRecord s1record(radiansToRotateBy, rotateAroundY);

	glm::quat fractureQuat = s1record.returnOriginalRotation();
	rotationpointsRefVector->applyQuaternion(fractureQuat);
	rotationRecords.push(s1record);

	currentNormalValue = rotationpointsRefVector->getLastPoint();	//

	//std::cout << "(QMVoidTranslatePointsForRasterization): normal value after quat application: " << currentNormalValue.x << ", " << currentNormalValue.y << ", " << currentNormalValue.z << std::endl;
}

float QMVoidTranslatePointsForRasterization::findRotationRadiansForGettingToPosZThroughY(glm::vec3 in_vec3)
{
	float degreesToRotateOnY = 0.0f;
	float fullRadian360 = 6.28319f;	// 360 degrees = this many radians

	// get the atan2 result, 
	float atan2result = atan2(in_vec3.x, in_vec3.z);
	float firstPassRotateRadians = 0.0f;

	//std::cout << "#- Quaternion Rotation  > (Rotate to Pos-Y, via Z-axis) Passed point is:  " << in_vec3.x << ", " << in_vec3.y << ", " << in_vec3.z << std::endl;
	//std::cout << "#- Quaternion Rotation  > (Rotate to Pos-Y, via Z-axis) Rotating passed point to Pos X (X = 1.0f), and finding degrees to rotate to get to it... " << std::endl;


	//if (debugFlag == 1)
	//{
	//	std::cout << "Atan result is: " << atan2result << std::endl;
	//}


	if (atan2result > 0.0)
	{
		//firstPassRotateRadians = fullRadian360 - atan2result;
		firstPassRotateRadians = atan2result;
	}
	else if (atan2result < 0.0) // if a is less than 0, add the result to fullRadian360 to get the amount to rotate by. (the quat goes CW when the rotation axis is pointing in a positive direction)
	{
		//firstPassRotateRadians = abs(atan2result);
		firstPassRotateRadians = fullRadian360 + atan2result;
	}

	glm::vec3 rotationAngle;
	rotationAngle.y = -1.0f;
	glm::quat rotationQuat = createQuaternion(firstPassRotateRadians, rotationAngle);
	glm::vec3 rotationResult = rotationQuat * in_vec3;

	glm::vec3 targetPosZ;
	targetPosZ.z = 1.0f;
	targetPosZ = rotationQuat * targetPosZ;

	float secondPassAtan = atan2(targetPosZ.x, targetPosZ.z);


	float secondPassRotateRadians;
	if (secondPassAtan > 0.0)
	{
		secondPassRotateRadians = secondPassAtan;
	}
	else if (secondPassAtan < 0.0)
	{
		secondPassRotateRadians = fullRadian360 + secondPassAtan;
	}
	float secondPassRotateFinal = fullRadian360 - secondPassRotateRadians;

	degreesToRotateOnY = secondPassRotateFinal;

	return degreesToRotateOnY;
}