#include "stdafx.h"
#include "CuttableTriangle.h"

void CuttableTriangle::buildCuttableTriangle(STriangle in_cuttableTriangle)
{
	for (int x = 0; x < 3; x++)
	{
		CuttableTriangleLine newLine(in_cuttableTriangle.triangleLines[x]);
		cuttableTriangleLines[x] = newLine;
	}

	// find the centroid
	glm::vec3 pointA = cuttableTriangleLines[0].pointA;
	glm::vec3 pointB = cuttableTriangleLines[1].pointA;
	glm::vec3 pointC = cuttableTriangleLines[2].pointA;
	glm::vec3 centroid = OrganicGLWinUtils::findTriangleCentroid(pointA, pointB, pointC);

	// find the centroid facing normal for each line
	for (int x = 0; x < 3; x++)
	{
		QuatRotationPoints points;	// point 0 = pointA of line
									// point 1 = pointB of line
									// point 2 = centroid
		glm::vec3 pointACopy = cuttableTriangleLines[x].pointA;
		glm::vec3 pointBCopy = cuttableTriangleLines[x].pointB;
		glm::vec3 centroidCopy = centroid;
		points.pointsRefVector.push_back(&pointACopy);
		points.pointsRefVector.push_back(&pointBCopy);
		points.pointsRefVector.push_back(&centroidCopy);

		QMVec3FindCentroidFacingNormal normalFinder;
		cuttableTriangleLines[x].cuttableTriangleCentroidFacingNormal = normalFinder.solve(&points, PolyDebugLevel::NONE);
	}
}

void CuttableTriangle::compareAgainstCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef)
{
	// first, run each CuttableTriangleLine against each CuttingTriangleLine.
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			compareCuttableTriangleLineToCuttingTriangleLine(x, &cuttableTriangleLines[x], y, &in_cuttingTriangleRef->cuttingLines[y]);
		}
	}

	// second, do analysis on the previous loop's run.
}

void CuttableTriangle::compareCuttableTriangleLineToCuttingTriangleLine(int in_cuttableIndex, CuttableTriangleLine* in_cuttableTriangleLineRef, int in_cuttingIndex, CuttingTriangleLine* in_cuttingTriangleLineRef)
{

}