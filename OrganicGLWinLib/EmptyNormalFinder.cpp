#include "stdafx.h"
#include "EmptyNormalFinder.h"

EmptyNormalFinder::EmptyNormalFinder(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2, glm::vec3 in_massReferencePoint)
{
	// push back the points
	glm::vec3 point0Copy = in_point0;
	glm::vec3 point1Copy = in_point1;
	glm::vec3 point2Copy = in_point2;
	glm::vec3 mrp = in_massReferencePoint;

	std::cout << "!!! Initial points are: " << std::endl;
	std::cout << point0Copy.x << ", " << point0Copy.y << ", " << point0Copy.z << std::endl;
	std::cout << point1Copy.x << ", " << point1Copy.y << ", " << point1Copy.z << std::endl;
	std::cout << point2Copy.x << ", " << point2Copy.y << ", " << point2Copy.z << std::endl;

	quatPoints.pointsRef.push_back(&point0Copy);
	quatPoints.pointsRef.push_back(&point1Copy);
	quatPoints.pointsRef.push_back(&point2Copy);
	quatPoints.pointsRef.push_back(&mrp);			// push the mrp into the vector
	quatPoints.pointsRef.push_back(&calculatedNormal);

	// check for any translation
	pointTranslator.performCheck(quatPoints.getFirstPoint());
	if (pointTranslator.requiresTranslation == 1)
	{
		quatPoints.applyTranslation(pointTranslator.getTranslationValue());
		std::cout << "!!! Points were translated; their values are: " << std::endl;
		std::cout << "0: " << point0Copy.x << ", " << point0Copy.y << ", " << point0Copy.z << std::endl;
		std::cout << "1: " << point1Copy.x << ", " << point1Copy.y << ", " << point1Copy.z << std::endl;
		std::cout << "2: " << point2Copy.x << ", " << point2Copy.y << ", " << point2Copy.z << std::endl;
	}



	//glm::vec3 ab = point1Copy - point0Copy;		// get the ab_> vector, to do the alignment for
	//std::cout << "Before rotation: " << ab.x << ", " << ab.y << ", " << ab.z << std::endl;

	rotationManager.initializeAndRun(&quatPoints);
	glm::vec3 validCalculatedNormal = rotationManager.getEmptyNormal();		// this will be stored in calculatedNormal when we are all done
	if (pointTranslator.requiresTranslation == 1)
	{
		quatPoints.applyTranslation(pointTranslator.getReverseTranslationValue());
		std::cout << "!!!!!! Points were reverse translated; their values are: " << std::endl;
		std::cout << "0: " << point0Copy.x << ", " << point0Copy.y << ", " << point0Copy.z << std::endl;
		std::cout << "1: " << point1Copy.x << ", " << point1Copy.y << ", " << point1Copy.z << std::endl;
		std::cout << "2: " << point2Copy.x << ", " << point2Copy.y << ", " << point2Copy.z << std::endl;
	}

	calculatedNormal = validCalculatedNormal;
	std::cout << ">> MRP point is: " << mrp.x << ", " << mrp.y << ", " << mrp.z << std::endl;
	std::cout << ">> Empty normal is: " << calculatedNormal.x << ", " << calculatedNormal.y << ", " << calculatedNormal.z << std::endl;

	/*
	float radiansToRotateBy = getRadiansForXAlignmentS1(ab);
	glm::vec3 rotationAroundY;
	rotationAroundY.y = 1.0f;
	QuatRotationRecord s1record(radiansToRotateBy, rotationAroundY);

	glm::quat originalQuat = s1record.returnOriginalRotation();
	glm::vec3 newPosition = originalQuat * ab;
	std::cout << "Post rotation: " << newPosition.x << ", " << newPosition.y << ", " << newPosition.z << std::endl;

	quatPoints.applyQuaternion(originalQuat);
	std::cout << "!!!! Quat points vector, post rotation: " << std::endl;
	quatPoints.printPoints();


	glm::quat reverseQuat = s1record.returnReverseRotation();
	glm::vec3 reversePosition = reverseQuat * newPosition;
	std::cout << "Reverse rotation: " << reversePosition.x << ", " << reversePosition.y << ", " << reversePosition.z << std::endl;
	*/

}

float EmptyNormalFinder::getRadiansForXAlignmentS1(glm::vec3 in_vec3)
{
	float radians = 0.0f;
	float fullRadian360 = 6.28319;
	float atan2result = atan2(in_vec3.z, in_vec3.x);
	float firstPassRotateRadians;
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
	return firstPassRotateRadians;
}