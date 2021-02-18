#include "stdafx.h"
#include "CuttingTriangle.h"

CuttingTriangle::CuttingTriangle(STriangle in_sTriangle)
{
	// find the centroid
	glm::vec3 pointA = in_sTriangle.triangleLines[0].pointA;
	glm::vec3 pointB = in_sTriangle.triangleLines[1].pointA;
	glm::vec3 pointC = in_sTriangle.triangleLines[2].pointA;
	glm::vec3 centroid = OrganicGLWinUtils::findTriangleCentroid(pointA, pointB, pointC);

	// find the centroid facing normal for each line
	for (int x = 0; x < 3; x++)
	{
		QuatRotationPoints points;	// point 0 = pointA of line
									// point 1 = pointB of line
									// point 2 = centroid
		glm::vec3 pointACopy = in_sTriangle.triangleLines[x].pointA;
		glm::vec3 pointBCopy = in_sTriangle.triangleLines[x].pointB;
		glm::vec3 centroidCopy = centroid;
		points.insertPointRefs(&pointACopy, &pointBCopy, &centroidCopy);

		QMVec3FindCentroidFacingNormal normalFinder;
		glm::vec3 centroidFacingNormal = normalFinder.solve(&points, PolyDebugLevel::NONE);
		centroidFacingNormal *= -1;	// outward facing normal is just the centroid normal multiplied by -1.

		CuttingTriangleLine newLine(in_sTriangle.triangleLines[x].pointA, in_sTriangle.triangleLines[x].pointB, centroidFacingNormal);
		cuttingLines[x] = newLine;
	}
}

void CuttingTriangle::reset()
{

}

void CuttingTriangle::printCuttingLineIntersections()
{
	for (int x = 0; x < 3; x++)
	{
		std::cout << "Data for line with index " << x << ": " << std::endl;
		cuttingLines[x].printCuttableIntersections();
	}
}
