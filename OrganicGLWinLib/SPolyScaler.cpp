#include "stdafx.h"
#include "SPolyScaler.h"

void SPolyScaler::translateAndRotateToXZPlane()
{
	// load each of the SPoly's border line points into the sPolyQuatPoints
	auto sPolyBorderLinesBegin = scalableSPoly.borderLines.begin();
	auto sPolyBorderLinesEnd = scalableSPoly.borderLines.end();
	for (; sPolyBorderLinesBegin != sPolyBorderLinesEnd; sPolyBorderLinesBegin++)
	{
		sPolyQuatPoints.insertPointRefs(&sPolyBorderLinesBegin->second.pointA);	// push back point A of each border line into sPolyQuatPoints.
	}

	//glm::vec3 sPolyCenterPoint = findCenterPoint();

	// translate all points of the SPoly, such that point A of the very first border line = 0,0,0.
	glm::vec3 borderLineAPointA = scalableSPoly.borderLines[0].pointA;

	// translate all points to point A of border line at index 0; store the OP
	PointTranslationCheck pointTranslator;
	pointTranslator.performCheck(borderLineAPointA);
	if (pointTranslator.requiresTranslation == 1)
	{
		sPolyQuatPoints.applyTranslation(pointTranslator.getTranslationValue());
		scalingOps.setOP1Value(pointTranslator.getTranslationValue());					// Set the value for OP 1.
	}

	// rotate the SPoly to the XZ plane. Store the recorded operations (OP 2)
	QuatUtils::rotateSPolyToXYPlanAndRecord(&scalingOps, &sPolyQuatPoints);
}

/*
glm::vec3 SPolyScaler::findCenterPoint()
{
	glm::vec3 foundCenterPoint;
	int numberOfSTriangles = scalableSPoly.triangles.size();
	if (numberOfSTriangles == 1)	// for a single triangle
	{
		glm::vec3 singleTriangleCenter = OrganicGLWinUtils::findTriangleCentroid(
			scalableSPoly.triangles[0].triangleLines[0].pointA,
			scalableSPoly.triangles[0].triangleLines[1].pointA,
			scalableSPoly.triangles[0].triangleLines[2].pointA
		);
		foundCenterPoint = singleTriangleCenter;
	}

	else if ((numberOfSTriangles / 2) == 0)	// for an even number of STriangles
	{
		glm::vec3 rootPoint = scalableSPoly.borderLines[0].pointA;	// the root point of an SPoly's triangle fan is always point A of the first border line.
		int selectedTriangleIndex = (numberOfSTriangles / 2) - 1;	// i.e, if there are 4 triangles, you'd be looking at triangle #2, which would be at index 1.
		glm::vec3 selectedBorderPoint = scalableSPoly.triangles[selectedTriangleIndex].triangleLines[2].pointA;	// the point to compare to would be point A of the second border line of our target triangle.
		glm::vec3 midPoint = (selectedBorderPoint - rootPoint);
		midPoint /= 2;	// half way between the result
		foundCenterPoint = midPoint;
	}
	else  // for an odd number of STriangles
	{
		int oddTriangleMiddleIndex = (numberOfSTriangles / 2);		// if 3 triangles, the result would be 1; if 5, the result would be 2 (that's what we want)
		glm::vec3 oddTriangleCenter = OrganicGLWinUtils::findTriangleCentroid(
			scalableSPoly.triangles[oddTriangleMiddleIndex].triangleLines[0].pointA,
			scalableSPoly.triangles[oddTriangleMiddleIndex].triangleLines[1].pointA,
			scalableSPoly.triangles[oddTriangleMiddleIndex].triangleLines[2].pointA
		);
		foundCenterPoint = oddTriangleCenter;
	}
	return foundCenterPoint;
}
*/