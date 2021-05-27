#include "stdafx.h"
#include "QMBoolResolveContestedCategorizedLine.h"

bool QMBoolResolveContestedCategorizedLine::solve(QuatRotationPoints* in_quatRotationPointsRef, PolyDebugLevel in_polyDebugLevel)
{
	// Assumptions:
	// -the QuatRotationPoints instance has points in this index order:
	// 0: point A of the contested line
	// 1: point B of the contested line
	// 2: the boundary's center point
	// 3: the empty normal of the contested line
	rotationPointsRefVector = in_quatRotationPointsRef;

	bool didCategorizedLineWinContest = false;

	// first, check if the second point of the categorized line has a Z value of 0; if it doesn't, we need to rotate to positive y.
	contestedLinePointBRef = in_quatRotationPointsRef->getPointRefByIndex(1);	// the point at index 1 = the second point of the contested line.
	if (contestedLinePointBRef->z != 0.0f)
	{
		rotateContestedLineAroundXToPositiveY();
	}

	// second, rotate the line to positive x, and y 0.
	rotateContestedLinePointBAroundZToPositiveX();

	// third, ensure that the center point of the boundary is aligned to the X/Y plane (will have a Z value of 0).
	rotateBoundaryCenterPointAlongXToPosOrNegY();


	// get a ref to the boundary center point
	glm::vec3* boundaryCenterRef = in_quatRotationPointsRef->getPointRefByIndex(2);
	glm::vec3* contestedLineEmptyNormalRef = in_quatRotationPointsRef->getPointRefByIndex(3);
	if (boundaryCenterRef->y > 0)
	{
		if (contestedLineEmptyNormalRef->y < 0)
		{
			didCategorizedLineWinContest = true;
			std::cout << "!! Categorized line won contest. " << std::endl;
		}
	}
	else if (boundaryCenterRef->y < 0)
	{
		if (contestedLineEmptyNormalRef->y > 0)
		{
			didCategorizedLineWinContest = true;
			std::cout << "!! Categorized line won contest. " << std::endl;
		}
	}

	if (didCategorizedLineWinContest == false)
	{
		std::cout << "!! Categorized line did NOT win contest. " << std::endl;
	}

	return didCategorizedLineWinContest;
}

void QMBoolResolveContestedCategorizedLine::rotateContestedLineAroundXToPositiveY()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;
	float atan2result = atan2(contestedLinePointBRef->y, contestedLinePointBRef->z); // find the radians we'll need to rotate by
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

	// to get to pos z = 1.0f, add 1.57 radians.
	firstPassRotateRadians += ((fullRadian360 / 4) * 3);

	glm::vec3 rotationAroundX;
	rotationAroundX.x = 1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundX);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	rotationPointsRefVector->applyQuaternion(originalQuat);	// rotate all values by this one

	std::cout << "++++++++++ printing points: " << std::endl;
	rotationPointsRefVector->printPoints();
}

void QMBoolResolveContestedCategorizedLine::rotateContestedLinePointBAroundZToPositiveX()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;
	float atan2result = atan2(contestedLinePointBRef->x, contestedLinePointBRef->y); // find the radians we'll need to rotate by
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

	// to get to pos z = 1.0f, add 1.57 radians.
	//firstPassRotateRadians += ((fullRadian360 / 4) * 3);
	firstPassRotateRadians += ((fullRadian360 / 4));

	glm::vec3 rotationAroundZ;
	rotationAroundZ.z = -1.0f;

	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundZ);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	rotationPointsRefVector->applyQuaternion(originalQuat);	// rotate all values by this one

	std::cout << "++++++++++ printing points, after rotate around Z to positive X: " << std::endl;
	rotationPointsRefVector->printPoints();
}

void QMBoolResolveContestedCategorizedLine::rotateBoundaryCenterPointAlongXToPosOrNegY()
{
	glm::vec3* boundaryCenterRef = rotationPointsRefVector->getPointRefByIndex(2);
	float radians = 0.0f;
	float fullRadian360 = 6.28319;
	float atan2result = atan2(boundaryCenterRef->y, boundaryCenterRef->z); // find the radians we'll need to rotate by
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

	std::cout << ">>> value of atan2result: " << atan2result << std::endl;

	// to get to pos z = 1.0f, add 1.57 radians.
	//firstPassRotateRadians += ((fullRadian360 / 4) * 3);
	firstPassRotateRadians += ((fullRadian360 / 4));

	std::cout << ">>> value of firstPassRotateRadians: " << firstPassRotateRadians << std::endl;

	glm::vec3 rotationAroundX;
	rotationAroundX.x = -1.0f;
	QuatRotationRecord s1record(firstPassRotateRadians, rotationAroundX);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	//*pointBRef = originalQuat * *pointBRef;	
	rotationPointsRefVector->applyQuaternion(originalQuat);	// rotate all values by this one

	std::cout << "++++++++++ printing points, after rotate for boundary center: " << std::endl;
	rotationPointsRefVector->printPoints();
}