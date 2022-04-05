#include "stdafx.h"
#include "RenderableCube.h"

void RenderableCube::buildTriangles()
{
	// part 1: set up the corners.
	setupCorners();

	// part 2: apply scaling to corners
	applyDistScaling();

	// part 3: build triangles.
	// clear the vector first.
	triangleVector.clear();

	// east face triangles
	Triangle eastA(upperSE, upperNE, lowerSE);
	Triangle eastB(upperNE, lowerSE, lowerNE);
	triangleVector.push_back(eastA);
	triangleVector.push_back(eastB);

	// south face triangles
	Triangle southA(upperSW, upperSE, lowerSW);
	Triangle southB(upperSE, lowerSW, lowerSE);
	triangleVector.push_back(southA);
	triangleVector.push_back(southB);

	// west face triangles
	Triangle westA(upperNW, upperSW, lowerNW);
	Triangle westB(upperSW, lowerNW, lowerSW);
	triangleVector.push_back(westA);
	triangleVector.push_back(westB);

	// north face triangles
	Triangle northA(upperNE, upperNW, lowerNE);
	Triangle northB(upperNW, lowerNE, lowerNW);
	triangleVector.push_back(northA);
	triangleVector.push_back(northB);

	// top face triangles
	Triangle topA(upperSW, upperNW, upperSE);
	Triangle topB(upperNW, upperSE, upperNE);
	triangleVector.push_back(topA);
	triangleVector.push_back(topB);

	// bottom face triangles
	Triangle bottomA(lowerNE, lowerSE, lowerNW);
	Triangle bottomB(lowerSE, lowerNW, lowerSW);
	triangleVector.push_back(bottomA);
	triangleVector.push_back(bottomB);

	// set total floats; for a cube this should always be 12 * 9 * 2 (12 triangles: 9 floats per triangles for points, 9 floats per triangle for color
	// ...so value should be 216.
	totalFloats = 12 * 9 * 2;
	pointArray.reset(new GLfloat[totalFloats]);
	auto trianglesBegin = triangleVector.begin();
	auto trianglesEnd = triangleVector.end();
	int highlighterIndex = 0;
	for (; trianglesBegin != trianglesEnd; trianglesBegin++)
	{
		for (int x = 0; x < 3; x++)
		{
			pointArray[highlighterIndex] = trianglesBegin->points[x].x;
			pointArray[highlighterIndex + 1] = trianglesBegin->points[x].y;
			pointArray[highlighterIndex + 2] = trianglesBegin->points[x].z;

			pointArray[highlighterIndex + 3] = color.x;
			pointArray[highlighterIndex + 4] = color.y;
			pointArray[highlighterIndex + 5] = color.z;

			highlighterIndex += 6;
		}
	}
}

void RenderableCube::setupCorners()
{
	//std::cout << "Setting up corners; center is at: " << center.x << ", " << center.y << ", " << center.z << std::endl;
	// set lower NE
	lowerNE = center;
	lowerNE.x += distFromCenter;
	lowerNE.y -= distFromCenter;
	lowerNE.z -= distFromCenter;

	// set lower SE
	lowerSE = center;
	lowerSE.x += distFromCenter;
	lowerSE.y -= distFromCenter;
	lowerSE.z += distFromCenter;

	// set lower SW
	lowerSW = center;
	lowerSW.x -= distFromCenter;
	lowerSW.y -= distFromCenter;
	lowerSW.z += distFromCenter;

	// set lower NW
	lowerNW = center;
	lowerNW.x -= distFromCenter;
	lowerNW.y -= distFromCenter;
	lowerNW.z -= distFromCenter;

	// set upper NE
	upperNE = center;
	upperNE.x += distFromCenter;
	upperNE.y += distFromCenter;
	upperNE.z -= distFromCenter;

	// set upper SE
	upperSE = center;
	upperSE.x += distFromCenter;
	upperSE.y += distFromCenter;
	upperSE.z += distFromCenter;

	// set upper SW
	upperSW = center;
	upperSW.x -= distFromCenter;
	upperSW.y += distFromCenter;
	upperSW.z += distFromCenter;

	// set upper NW
	upperNW = center;
	upperNW.x -= distFromCenter;
	upperNW.y += distFromCenter;
	upperNW.z -= distFromCenter;


}

void RenderableCube::applyDistScaling()
{
	// scale lower NE corner
	glm::vec3 lowerNEDiff = lowerNE - center;

	//std::cout << "> lowerNE: " << lowerNE.x << ", " << lowerNE.y << ", " << lowerNE.z << std::endl;
	//std::cout << "> lowerNEDiff: " << lowerNEDiff.x << ", " << lowerNEDiff.y << ", " << lowerNEDiff.z << std::endl;
	//std::cout << "> Center: " << center.x << ", " << center.y << ", " << center.z << std::endl;

	lowerNEDiff *= distScale;
	lowerNE = center + lowerNEDiff;

	// scale lower SE corner
	glm::vec3 lowerSEDiff = lowerSE - center;
	lowerSEDiff *= distScale;
	lowerSE = center + lowerSEDiff;

	// scale lower SW corner
	glm::vec3 lowerSWDiff = lowerSW - center;
	lowerSWDiff *= distScale;
	lowerSW = center + lowerSWDiff;

	// scale lower NW corner
	glm::vec3 lowerNWDiff = lowerNW - center;
	lowerNWDiff *= distScale;
	lowerNW = center + lowerNWDiff;

	// scale upper NE corner
	glm::vec3 upperNEDiff = upperNE - center;
	upperNEDiff *= distScale;
	upperNE = center + upperNEDiff;

	// scale upper SE corner
	glm::vec3 upperSEDiff = upperSE - center;
	upperSEDiff *= distScale;
	upperSE = center + upperSEDiff;

	// scale upper SW corner
	glm::vec3 upperSWDiff = upperSW - center;
	upperSWDiff *= distScale;
	upperSW = center + upperSWDiff;

	// scale upper NW corner
	glm::vec3 upperNWDiff = upperNW - center;
	upperNWDiff *= distScale;
	upperNW = center + upperNWDiff;
}