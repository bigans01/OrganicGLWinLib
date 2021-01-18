#include "stdafx.h"
#include "QMBoolPointWithinTriangle.h"

bool QMBoolPointWithinTriangle::solve(QuatRotationPoints* in_quatRotationPointsRef)
{
	bool returnValue = false;
	bool planeArrayCheckResult[3];
	struct TriangleLineSpec
	{
		ThreeDLineSegment segment;
		glm::vec3 centroidFacingNormal;
	};

	glm::vec3 pointToCheckRoot = in_quatRotationPointsRef->getPointByIndex(0);
	glm::vec3 trianglePoint0 = in_quatRotationPointsRef->getPointByIndex(1);
	glm::vec3 trianglePoint1 = in_quatRotationPointsRef->getPointByIndex(2);
	glm::vec3 trianglePoint2 = in_quatRotationPointsRef->getPointByIndex(3);

	ThreeDLineSegment threeDline1(trianglePoint0, trianglePoint1);
	TriangleLineSpec line1;
	line1.segment = threeDline1;

	ThreeDLineSegment threeDline2(trianglePoint1, trianglePoint2);
	TriangleLineSpec line2;
	line2.segment = threeDline2;

	ThreeDLineSegment threeDline3(trianglePoint2, trianglePoint0);
	TriangleLineSpec line3;
	line3.segment = threeDline3;

	TriangleLineSpec lines[3];
	lines[0] = line1;
	lines[1] = line2;
	lines[2] = line3;

	glm::vec3 centroid = findTriangleCentroid(lines[0].segment.pointA, lines[1].segment.pointA, lines[2].segment.pointA);
	for (int x = 0; x < 3; x++)
	{
		glm::vec3 pointACopy = lines[x].segment.pointA;
		glm::vec3 pointBCopy = lines[x].segment.pointB;
		glm::vec3 centroidPointCopy = centroid;

		// first, check for point translation
		PointTranslationCheck pointCheck;
		pointCheck.performCheck(pointACopy);
		if (pointCheck.requiresTranslation == 1)
		{
			glm::vec3 translationValue = pointCheck.getTranslationValue();
			pointACopy += translationValue;
			pointBCopy += translationValue;
			centroidPointCopy += translationValue;
		}

		//std::cout << ">>>>>: point A Copy, post translate: " << pointACopy.x << ", " << pointACopy.y << ", " << pointACopy.z << std::endl;
		//std::cout << ">>>>>: point B Copy, post translate: " << pointBCopy.x << ", " << pointBCopy.y << ", " << pointBCopy.z << std::endl;
		//std::cout << ">>>>>: centroid copy, post translate: " << centroidPointCopy.x << ", " << centroidPointCopy.y << ", " << centroidPointCopy.z << std::endl;

		QuatRotationPoints rotationPoints;
		rotationPoints.pointsRefVector.push_back(&pointACopy);
		rotationPoints.pointsRefVector.push_back(&pointBCopy);
		rotationPoints.pointsRefVector.push_back(&centroidPointCopy);

		QuatRotationManager rotationManager;
		//rotationManager.initializeAndRunForFindingBorderLineEmptyNormal(&rotationPoints);
		rotationManager.initializeAndRunForFindingBorderLineEmptyNormalWithRotateToZ(&rotationPoints);


		//std::cout << ">>>>>: centroid copy, post translate (2): " << centroidPointCopy.x << ", " << centroidPointCopy.y << ", " << centroidPointCopy.z << std::endl;

		lines[x].centroidFacingNormal = centroidPointCopy;
	}

	for (int x = 0; x < 3; x++)
	{

		glm::vec3 pointACopy = lines[x].segment.pointA;
		glm::vec3 pointBCopy = lines[x].segment.pointB;
		glm::vec3 pointToCheck = pointToCheckRoot;
		glm::vec3 centroidFacingNormal = lines[x].centroidFacingNormal;

		std::cout << ">>> pushing back this value for centroid facing normal: " << centroidFacingNormal.x << ", " << centroidFacingNormal.y << ", " << centroidFacingNormal.z << std::endl;

		// first, check for point translation; only translate pointA/B, and the point to check against.
		PointTranslationCheck pointCheck;
		pointCheck.performCheck(pointACopy);
		if (pointCheck.requiresTranslation == 1)
		{
			glm::vec3 translationValue = pointCheck.getTranslationValue();
			pointACopy += translationValue;
			pointBCopy += translationValue;
			pointToCheck += translationValue;
		}

		QuatRotationPoints rotationPoints;
		rotationPoints.pointsRefVector.push_back(&pointACopy);
		rotationPoints.pointsRefVector.push_back(&pointBCopy);
		rotationPoints.pointsRefVector.push_back(&pointToCheck);
		rotationPoints.pointsRefVector.push_back(&centroidFacingNormal);		// fetched by rotationManager, to check for flipping on Z-axis

		QuatRotationManager rotationManager;
		planeArrayCheckResult[x] = rotationManager.initializeAndRunForCheckingIfPointIswithinPlaneWithRotateToZ(&rotationPoints);	// the point to check will be rounded to nearest hundredths, 
																														// during the quat rotation

	}

	int withinPlaneCount = 0;
	for (int x = 0; x < 3; x++)
	{
		if (planeArrayCheckResult[x] == true)
		{
			std::cout << "!!! found as being within plane! " << std::endl;

			withinPlaneCount++;
		}
	}

	if (withinPlaneCount == 3)
	{
		//std::cout << "OrganicGLWinUtils::checkIfPointLiesWithinTriangle --> Point is WITHIN triangle. " << std::endl;
		returnValue = true;
	}
	return returnValue;
}