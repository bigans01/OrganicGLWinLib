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

	bool categorizedLineWonContest = false;

	// We want the first point and second point of the lines to be on the same X/Z coordinate, but have differing y values.
	// This will be a two step process:
	//

	// Step 1:	rotate the second point of the line around the X axis, to Positive Y; we know we will have to do this if the Z value 
	//			of the second point in the line is not 0. When this is done, the Z value of both points in the line should be 0 -- but 
	//			whether or not the second point of the line is in the positive X direction is unknown. That can only be verified in 
	//			step 2.
	contestedLinePointBRef = in_quatRotationPointsRef->getPointRefByIndex(1);	// the point at index 1 = the second point of the contested line.
	if (contestedLinePointBRef->z != 0.0f)
	{
		rotateContestedLineAroundXToPositiveY();
	}

	// Step 2:	We must ensure that the X value of the second point of the line resides in positive-X space. The only way to be sure of this is
	//			to rotate the line about the Z axis, to where the second point of the CategorizedLine has Y = 0 and a X value > 0.0f. This guarantees that the
	//			line is perfectly aligned in a way where both points now have Y and Z= 0 (we took care of rotate to Z = 0 in the previous step), 
	//          point A has X = 0, and point B has some value where X > 0.
	//
	//			In other words, the non-line points (the CategorizedLine empty normal and the SPoly face center point) now serve as an axis of rotation,
	//          around X.
	rotateContestedLinePointBAroundZToPositiveX();

	// Step 3:  We will rotate the other points about the axis we created in step 2. Whether or not the boundary SPoly center is rotated to positive or
	//			or negative y at this point is irrelevant; we will do checks for both cases. When done, the boundary center point should have a Z value of 0,
	//			(in other words, it exists in the X-Y plane)
	rotateBoundaryCenterPointAlongXToPosOrNegY();


	// Step 4:	Finally, in the last step, we will check to see if the empty normal of the categorized line points in the same direction as the location
	//			of the boundary point. If the Y direction of the CategorizedLine empty normal does not match the direction of the translated boundary center point, 
	//		 	it means the CategorizedLine "won" the contest, meaning that the SPoly should use this line as a basis 
	//			for creating the boundary SPoly, if needed. This is because of the fact that a CategorizedLine that has a normal which faces "outward" from the
	//			the SPoly it is a member of can still be potentially used.
	//
	//			However, if the CategorizedLine empty normal does face towards the SPoly boundary point, the line does not win the contest.
	// get a ref to the boundary center point
	glm::vec3* boundaryCenterRef = in_quatRotationPointsRef->getPointRefByIndex(2);
	glm::vec3* contestedLineEmptyNormalRef = in_quatRotationPointsRef->getPointRefByIndex(3);
	if (boundaryCenterRef->y > 0)
	{
		if (contestedLineEmptyNormalRef->y < 0)
		{
			categorizedLineWonContest = true;
			std::cout << "!! Categorized line won contest. " << std::endl;
		}
	}
	else if (boundaryCenterRef->y < 0)
	{
		if (contestedLineEmptyNormalRef->y > 0)
		{
			categorizedLineWonContest = true;
			std::cout << "!! Categorized line won contest. " << std::endl;
		}
	}

	if (categorizedLineWonContest == false)
	{
		std::cout << "!! Categorized line did NOT win contest. " << std::endl;
	}

	return categorizedLineWonContest;
}

void QMBoolResolveContestedCategorizedLine::rotateContestedLineAroundXToPositiveY()
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319f;
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
	float fullRadian360 = 6.28319f;
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
	float fullRadian360 = 6.28319f;
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