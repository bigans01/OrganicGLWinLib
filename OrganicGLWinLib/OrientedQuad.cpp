#include "stdafx.h"
#include "OrientedQuad.h"

OrientedQuad::OrientedQuad(glm::vec3 in_startPoint, float in_coreDim, glm::vec3 in_dir1, glm::vec3 in_dir2)
{
	// first point
	quadPoints[0].worldSpaceCoord = in_startPoint;
	quadPoints[0].orientation = QuadPointOrientation::ON_ORIGIN;

	// second point
	quadPoints[1].worldSpaceCoord = in_dir1 * in_coreDim;
	quadPoints[1].orientation = QuadPointOrientation::FURTHEST_FROM_ORIGIN;

	// third point
	quadPoints[2].worldSpaceCoord = quadPoints[1].worldSpaceCoord + (in_dir2 * in_coreDim);
	quadPoints[2].orientation = QuadPointOrientation::FURTHEST_FROM_ORIGIN;

	// fourth point
	quadPoints[3].worldSpaceCoord = quadPoints[2].worldSpaceCoord - (in_dir1 * in_coreDim);
	quadPoints[3].orientation = QuadPointOrientation::FURTHEST_FROM_ORIGIN;

	//std::cout << "!!!  > 0: " << quadPoints[0].worldSpaceCoord.x << ", " << quadPoints[0].worldSpaceCoord.y << ", " << quadPoints[0].worldSpaceCoord.z << std::endl;
	//std::cout << "!!!  > 1: " << quadPoints[1].worldSpaceCoord.x << ", " << quadPoints[1].worldSpaceCoord.y << ", " << quadPoints[1].worldSpaceCoord.z << std::endl;
	//std::cout << "!!!  > 2: " << quadPoints[2].worldSpaceCoord.x << ", " << quadPoints[2].worldSpaceCoord.y << ", " << quadPoints[2].worldSpaceCoord.z << std::endl;
	//std::cout << "!!!  > 3: " << quadPoints[3].worldSpaceCoord.x << ", " << quadPoints[3].worldSpaceCoord.y << ", " << quadPoints[3].worldSpaceCoord.z << std::endl;
}

OrientedQuad::OrientedQuad(glm::vec3 in_startPoint, float in_widthDim, float in_lengthDim, glm::vec3 in_lengthDir, glm::vec3 in_widthDir)
{
	quadPoints[0].worldSpaceCoord = OrganicGLWinUtils::roundVec3ToHundredths(in_startPoint);
	quadPoints[0].orientation = QuadPointOrientation::NEAREST_TO_ORIGIN;

	quadPoints[1].worldSpaceCoord = OrganicGLWinUtils::roundVec3ToHundredths(in_startPoint + (in_lengthDim * in_lengthDir));
	quadPoints[1].orientation = QuadPointOrientation::FURTHEST_FROM_ORIGIN;

	quadPoints[2].worldSpaceCoord = OrganicGLWinUtils::roundVec3ToHundredths(quadPoints[1].worldSpaceCoord + (in_widthDim * in_widthDir));
	quadPoints[2].orientation = QuadPointOrientation::FURTHEST_FROM_ORIGIN;

	quadPoints[3].worldSpaceCoord = OrganicGLWinUtils::roundVec3ToHundredths(quadPoints[2].worldSpaceCoord - (in_lengthDim * in_lengthDir));
	quadPoints[3].orientation = QuadPointOrientation::NEAREST_TO_ORIGIN;

	//std::cout << "!!! (Non-core) > 0: " << quadPoints[0].worldSpaceCoord.x << ", " << quadPoints[0].worldSpaceCoord.y << ", " << quadPoints[0].worldSpaceCoord.z << std::endl;
	//std::cout << "!!! (Non-core) > 1: " << quadPoints[1].worldSpaceCoord.x << ", " << quadPoints[1].worldSpaceCoord.y << ", " << quadPoints[1].worldSpaceCoord.z << std::endl;
	//std::cout << "!!! (Non-core) > 2: " << quadPoints[2].worldSpaceCoord.x << ", " << quadPoints[2].worldSpaceCoord.y << ", " << quadPoints[2].worldSpaceCoord.z << std::endl;
	//std::cout << "!!! (Non-core) > 3: " << quadPoints[3].worldSpaceCoord.x << ", " << quadPoints[3].worldSpaceCoord.y << ", " << quadPoints[3].worldSpaceCoord.z << std::endl;
}

glm::vec3* OrientedQuad::getPointRef(int in_pointIndex)
{
	return &quadPoints[in_pointIndex].worldSpaceCoord;
}

glm::vec3 OrientedQuad::getPoint(int in_pointIndex)
{
	return quadPoints[in_pointIndex].worldSpaceCoord;
}

QuadTriangles OrientedQuad::getQuadTriangles()
{
	QuadTriangles returnQuadTriangles;
	Triangle triangle1, triangle2;

	triangle1.points[0] = quadPoints[0].worldSpaceCoord;
	triangle1.points[1] = quadPoints[1].worldSpaceCoord;
	triangle1.points[2] = quadPoints[2].worldSpaceCoord;

	triangle2.points[0] = quadPoints[0].worldSpaceCoord;
	triangle2.points[1] = quadPoints[2].worldSpaceCoord;
	triangle2.points[2] = quadPoints[3].worldSpaceCoord;

	returnQuadTriangles.triangles[0] = triangle1;
	returnQuadTriangles.triangles[1] = triangle2;

	return returnQuadTriangles;
}