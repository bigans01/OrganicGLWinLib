#include "stdafx.h"
#include "FusionCandidateProducer.h"

#define SMALL_NUM   0.000001 // anything that avoids division overflow

FusionCandidateProducer::FusionCandidateProducer(PolyDebugLevel in_debugOption)
{
	candidateProductionLogger.setDebugLevel(in_debugOption);
}

FusionCandidate FusionCandidateProducer::produceCandidate(STriangle in_sTriangle, STriangleLine in_sTriangleLine)
{
	FusionCandidate candidateResult;
	// do typical compare:
	RayIntersectionResult resultA = determineRayRelationShipToTriangle(in_sTriangle, in_sTriangleLine);

	// do a compare where the in_line is swapped
	STriangleLine swappedLine = in_sTriangleLine;
	swappedLine.pointA = in_sTriangleLine.pointB;
	swappedLine.pointB = in_sTriangleLine.pointA;

	// do the swapped compare
	RayIntersectionResult resultB = determineRayRelationShipToTriangle(in_sTriangle, swappedLine);

	// branch 1: if the ray lies within the triangle plane, the liesWithinPlane flag would be set on the first result.
	if (resultA.liesWithinTrianglePlane == true)
	{
		// Must meet one of the following conditions to return as "2":
		// 1. A is true, B is not
		// 2. B is true, A is not
		// 3. Both are true
		// 
		// A value of both being == false means that although on the same plane, it actually doesn't exist in the triangle.
		if
		(
			(
				(resultA.liesWithinTriangleZone == true)
				&&
				(resultB.liesWithinTriangleZone == false)
			)

			||

			(
				(resultA.liesWithinTriangleZone == false)
				&&
				(resultB.liesWithinTriangleZone == true)
			)

			||

			(
				(resultA.liesWithinTriangleZone == true)
				&&
				(resultB.liesWithinTriangleZone == true)
			)
		)
		{
			if (resultA.wasIntersectOnBorderLine == 1)
			{
				candidateResult.wasCandidateBorderLineAndWithinPlane = true;
			}
			candidateResult.candidateIntersectionResult.borderLineID = resultA.borderLineID;
			candidateResult.candidateIntersectionResult.intersectedPoint = resultA.intersectedPoint;
			candidateResult.candidateIntersectionResult.wasIntersectFound = 2;		// remember, a value of 2 indicates it was within the triangle's plane, and at least 1 point is within the triangle's plane.
			candidateResult.candidateIntersectionResult.wasIntersectOnBorderLine = resultA.wasIntersectOnBorderLine;
		}
		
	}

	// branch 2: one of the points of the line is equal to a point in the triangle.
	else if
	(
		(resultA.matchesPointExact == true)
		||
		(resultB.matchesPointExact == true)
	)
	{
		if (resultA.matchesPointExact == true)
		{
			candidateResult.candidateIntersectionResult.borderLineID = resultA.borderLineID;
			candidateResult.candidateIntersectionResult.wasIntersectOnBorderLine = resultA.wasIntersectOnBorderLine;
			candidateResult.candidateIntersectionResult.intersectedPoint = resultA.intersectedPoint;
		}
		else if (resultB.matchesPointExact == true)
		{

			candidateResult.candidateIntersectionResult.borderLineID = resultB.borderLineID;
			candidateResult.candidateIntersectionResult.wasIntersectOnBorderLine = resultB.wasIntersectOnBorderLine;
			candidateResult.candidateIntersectionResult.intersectedPoint = resultB.intersectedPoint;
		}
		candidateResult.candidateIntersectionResult.wasIntersectFound = 1;
	}

	// branch 3: otherwise, it's not either of the previous 2, so it must be a typical comparison.
	else if (resultA.liesWithinTrianglePlane == false)
	{
		if
		(
			(resultA.wasIntersectFound != 0)
			&&
			(resultB.wasIntersectFound != 0)
		)
		{
			candidateResult.candidateIntersectionResult.borderLineID = resultA.borderLineID;
			candidateResult.candidateIntersectionResult.intersectedPoint = resultA.intersectedPoint;
			candidateResult.candidateIntersectionResult.wasIntersectFound = resultA.wasIntersectFound;
			candidateResult.candidateIntersectionResult.wasIntersectOnBorderLine = resultA.wasIntersectOnBorderLine;
			if
			(
				(resultA.wasIntersectOnBorderLine == 1)
				&&
				(resultA.wasIntersectFound == 3)
			)
			{
				candidateResult.wasCandidateBorderLineAndWithinPlane = true;
			}
		}
	}

	return candidateResult;
}

glm::vec3 FusionCandidateProducer::roundPointToHundredths(glm::vec3 in_point)
{
	glm::vec3 returnPoint;
	returnPoint.x = float(floor(in_point.x * 100 + 0.5) / 100);
	returnPoint.y = float(floor(in_point.y * 100 + 0.5) / 100);
	returnPoint.z = float(floor(in_point.z * 100 + 0.5) / 100);
	return returnPoint;
}

double FusionCandidateProducer::doubledot(glm::vec3 in_A, glm::vec3 in_B)
{
	double x_multiplied = in_A.x * in_B.x;
	double y_multiplied = in_A.y * in_B.y;
	double z_multiplied = in_A.z * in_B.z;
	return x_multiplied + y_multiplied + z_multiplied;
}

bool FusionCandidateProducer::isPointEqualToTrianglePoint(glm::vec3 in_point, STriangle* in_triangleRef)
{
	bool result = false;
	for (int x = 0; x < 3; x++)
	{
		if (in_point == in_triangleRef->triangleLines[x].pointA)
		{
			//std::cout << "!!:##### Notice: although the ray isn't coplanar to the triangle (result of 2), one point was found as being equal to a point in the compared-to triangle. Flagging as 1. " << std::endl;
			result = true;
			break;
		}
	}
	return result;
}

RayIntersectionResult FusionCandidateProducer::determineRayRelationShipToTriangle(STriangle in_triangle, STriangleLine in_line)
{
	//int someVal = 0;
	RayIntersectionResult returnResult;
	glm::vec3 intersect_candidate;

	//std::cout << "(Fusion) triangle, point 0: " << in_triangle.triangleLines[0].pointA.x << ", " << in_triangle.triangleLines[0].pointA.y << ", " << in_triangle.triangleLines[0].pointA.z << std::endl;
	//std::cout << "(Fusion) triangle, point 1: " << in_triangle.triangleLines[1].pointA.x << ", " << in_triangle.triangleLines[1].pointA.y << ", " << in_triangle.triangleLines[1].pointA.z << std::endl;
	//std::cout << "(Fusion) triangle, point 2: " << in_triangle.triangleLines[2].pointA.x << ", " << in_triangle.triangleLines[2].pointA.y << ", " << in_triangle.triangleLines[2].pointA.z << std::endl;
	candidateProductionLogger.log("(Fusion) triangle, point 0: ", in_triangle.triangleLines[0].pointA.x, ", ", in_triangle.triangleLines[0].pointA.y, ", ", in_triangle.triangleLines[0].pointA.z, "\n");
	candidateProductionLogger.log("(Fusion) triangle, point 1: ", in_triangle.triangleLines[1].pointA.x, ", ", in_triangle.triangleLines[1].pointA.y, ", ", in_triangle.triangleLines[1].pointA.z, "\n");
	candidateProductionLogger.log("(Fusion) triangle, point 2: ", in_triangle.triangleLines[2].pointA.x, ", ", in_triangle.triangleLines[2].pointA.y, ", ", in_triangle.triangleLines[2].pointA.z, "\n");

	//std::cout << "(Fusion) Checking if this line intersects: pointA: " << in_line.pointA.x << ", " << in_line.pointA.y << ", " << in_line.pointA.z << " | pointB: " << in_line.pointB.x << ", " << in_line.pointB.y << ", " << in_line.pointB.z << std::endl;
	//std::cout << "=========" << std::endl;
	candidateProductionLogger.log("(Fusion) Checking if this line intersects: pointA: ", in_line.pointA.x, ", ", in_line.pointA.y, ", ", in_line.pointA.z, " | pointB: ", in_line.pointB.x, ", ", in_line.pointB.y, ", ", in_line.pointB.z, "\n");
	candidateProductionLogger.log("(Fusion) =========", "\n");

	glm::vec3 point0 = in_triangle.triangleLines[0].pointA;
	glm::vec3 point1 = in_triangle.triangleLines[1].pointA;
	glm::vec3 point2 = in_triangle.triangleLines[2].pointA;


	int matchCount = 0;

	// attempt match of point A of the triangleLine to all 3 points:
	for (int x = 0; x < 3; x++)
	{
		if (in_line.pointA == in_triangle.triangleLines[x].pointA)
		{
			matchCount++;
		}
	}

	// ...of point B
	for (int x = 0; x < 3; x++)
	{
		if (in_line.pointB == in_triangle.triangleLines[x].pointA)
		{
			matchCount++;
		}
	}

	//std::cout << "##### Matchcount is: " << matchCount << std::endl;
	//if (matchCount == 1)
	//{
		//std::cout << "!! Match count 1 hit.  " << std::endl;
	//}

	// condition one, matchCount is 2
	if (matchCount == 2)
	{
		//std::cout << "~~~~~~~~~~~~~~~~ note special case; the triangleLine matches a line exactly in the incoming triangle. flagging as 2." << std::endl;
		//int someVal = 3;
		//std::cin >> someVal;
		returnResult.setResult(3);		// set flag indicatiing that the passed in STriangleLine is equal to a STriangleLine that belongs to the passed in STriangle.
	}

	// condition two, matchCount is 1 (commented out for now)
	/*
	else if (matchCount == 1)
	{
		std::cout << "!! Match count 1 hit.  " << std::endl;

		if
			(
			(in_triangleLine.pointA == point0)
				||
				(in_triangleLine.pointA == point1)
				||
				(in_triangleLine.pointA == point2)

				||
				(in_triangleLine.pointB == point0)
				||
				(in_triangleLine.pointB == point1)
				||
				(in_triangleLine.pointB == point2)
				)
		{
			if (in_triangleLine.pointA == point0)
			{
				//std::cout << "!-> Intersection notice: point A of the line, equals point0." << std::endl;
				returnResult.setResult(1);
				returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
				returnResult.borderLineID = in_triangleLine.borderLineID;
				returnResult.intersectedPoint = roundPointToHundredths(point0);					// the intercept point should be rounded to hundredths!
			}
			else if (in_triangleLine.pointA == point1)
			{
				//std::cout << "!-> Intersection notice: point A of the line, equals point1." << std::endl;
				returnResult.setResult(1);
				returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
				returnResult.borderLineID = in_triangleLine.borderLineID;
				returnResult.intersectedPoint = roundPointToHundredths(point1);					// the intercept point should be rounded to hundredths!
			}

			else if (in_triangleLine.pointA == point2)
			{
				//std::cout << "!-> Intersection notice: point A of the line, equals point2." << std::endl;
				returnResult.setResult(1);
				returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
				returnResult.borderLineID = in_triangleLine.borderLineID;
				returnResult.intersectedPoint = roundPointToHundredths(point2);					// the intercept point
			}
			else if (in_triangleLine.pointB == point0)
			{
				//std::cout << "!-> Intersection notice: point B of the line, equals point0." << std::endl;
				returnResult.setResult(1);
				returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
				returnResult.borderLineID = in_triangleLine.borderLineID;
				returnResult.intersectedPoint = roundPointToHundredths(point0);					// the intercept point
			}
			else if (in_triangleLine.pointB == point1)
			{
				//std::cout << "!-> Intersection notice: point B of the line, equals point1." << std::endl;
				returnResult.setResult(1);
				returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
				returnResult.borderLineID = in_triangleLine.borderLineID;
				returnResult.intersectedPoint = roundPointToHundredths(point1);					// the intercept point
			}
			else if (in_triangleLine.pointB == point2)
			{
				//std::cout << "!-> Intersection notice: point B of the line, equals point2." << std::endl;
				returnResult.setResult(1);
				returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
				returnResult.borderLineID = in_triangleLine.borderLineID;
				returnResult.intersectedPoint = roundPointToHundredths(point2);					// the intercept point
			}
		}
	}
	*/
	else if (matchCount != 2)
	{
		int tempDebug = 0;

		/*
		std::cout << "##########################################" << std::endl;
		std::cout << "triangle, point 0: " << point0.x << ", " << point0.y << ", " << point0.z << std::endl;
		std::cout << "triangle, point 1: " << point1.x << ", " << point1.y << ", " << point1.z << std::endl;
		std::cout << "triangle, point 2: " << point2.x << ", " << point2.y << ", " << point2.z << std::endl;
		std::cout << "intersecting line, point A: " << in_triangleLine.pointA.x << ", " << in_triangleLine.pointA.y << ", " << in_triangleLine.pointA.z << std::endl;
		std::cout << "intersecting line, point B: " << in_triangleLine.pointB.x << ", " << in_triangleLine.pointB.y << ", " << in_triangleLine.pointB.z << std::endl;
		std::cout << "##########################################" << std::endl;
		*/

		glm::vec3 u = point1 - point0;	// u vector
		glm::vec3 v = point2 - point0;	// v " 

		//std::cout << "U: " << u.x << ", " << u.y << ", " << u.z << std::endl;
		//std::cout << "V: " << v.x << ", " << v.y << ", " << v.z << std::endl;
		glm::vec3 N = cross(u, v);		// the normal of the triangle
		//N.z = -1.0f;


		glm::vec3 dir = in_line.pointB - in_line.pointA;
		glm::vec3 w0 = in_line.pointA - point0;

		//std::cout << "Normal: " << N.x << ", " << N.y << ", " << N.z << std::endl;
		//std::cout << "Dir: " << dir.x << ", " << dir.y << ", " << dir.z << std::endl;

		float a = -dot(N, w0);
		float b = dot(N, dir);
		double doubleb = doubledot(N, dir);


		//std::cout <<std::setprecision(9);
		//std::cout << "(a) is: " << a << std::endl;
		//std::cout << "(b) is: " << b << std::endl;
		candidateProductionLogger.log("(Fusion) (a) is: ", a, "\n");
		candidateProductionLogger.log("(Fusion) (b) is: ", b, "\n");
		//std::cout << "(doubleb) is: " << doubleb << std::endl;

		// remember, if SMALL_NUM is too sensitive (i.e., too precise in the IEEE float), there may be a "miss" on detecting when
		// the passed-in STriangleLine's two points exactly match two points in the STriangle. 
		if (fabs(b) < SMALL_NUM) {     // ray is  parallel to triangle plane

			//std::cout << "!!! Logic for less than SMALL_NUM entered....enter number to continue. " << std::endl;
			//int someValAwesome = 3;
			//std::cin >> someValAwesome;

			//if (a == 0)                 // ray lies in triangle plane
			//if (a < SMALL_NUM)

			if							  // ray lies in triangle plane
			(
				(a < SMALL_NUM)
				&&
				(a > (SMALL_NUM*-1.0f))
			)
			{
				//std::cout << "::> Line is lies within triangle. " << std::endl;
				candidateProductionLogger.log("::> Line is lies within triangle. ", "\n");

				// we must set the flag indicating the line lies within the triangle's plane.
				returnResult.liesWithinTrianglePlane = true;

				// ...and a flag used to store the result of whether it not it lies within the triangle's plane's
				bool withinFlag = false;


				//withinFlag = OrganicGLWinUtils::checkIfPointLiesWithinTriangleWithRotateToZ(in_line.pointB, point0, point1, point2);

				// first point = point to check, remaining 3 = points of triangle (see header file for QMBoolPointWithinTrianglePBZ)
				withinFlag = QuatUtils::checkIfPointLiesWithinTrianglePBZ(in_line.pointB, point0, point1, point2);

				//std::cout << "||||||||| Triangle planar check; triangle points are: " << std::endl;
				//std::cout << "triangle, point 0: " << point0.x << ", " << point0.y << ", " << point0.z << std::endl;
				//std::cout << "triangle, point 1: " << point1.x << ", " << point1.y << ", " << point1.z << std::endl;
				//std::cout << "triangle, point 2: " << point2.x << ", " << point2.y << ", " << point2.z << std::endl;
				candidateProductionLogger.log("||||||||| Triangle planar check; triangle points are: ", "\n");
				candidateProductionLogger.log("triangle, point 0: ", point0.x, ", ", point0.y, ", ", point0.z, "\n");
				candidateProductionLogger.log("triangle, point 1: ", point1.x, ", ", point1.y, ", ", point1.z, "\n");
				candidateProductionLogger.log("triangle, point 2: ", point2.x, ", ", point2.y, ", ", point2.z, "\n");


				//std::cout << "::>>> Checking if Point B lies within triangle " << in_line.pointB.x << ", " << in_line.pointB.y << ", " << in_line.pointB.z << std::endl;
				candidateProductionLogger.log("::>>> Checking if Point B lies within triangle ", in_line.pointB.x, ", ", in_line.pointB.y, ", ", in_line.pointB.z, "\n");

				// since we know it's coplanar, the point lies within the triangle if either of the two is met:
				// 1.) it lies within the triangle (withinFlag would be true)
				// 2.) point B of the ray is equal to exactly one point in the triangle
				if
				(
					(withinFlag == true)	
					||
					(isPointEqualToTrianglePoint(in_line.pointB, &in_triangle) == true)
				)
				{
					//std::cout << "::>>> (This point lies WITHIN the triangle!) " << in_line.pointB.x << ", " << in_line.pointB.y << ", " << in_line.pointB.z << std::endl;
					candidateProductionLogger.log("::>>> (This point lies WITHIN the triangle!) ", in_line.pointB.x, ", ", in_line.pointB.y, ", ", in_line.pointB.z, "\n");
					returnResult.liesWithinTriangleZone = true;
					returnResult.setResult(2);
				}
				else if (withinFlag == false)
				{
					//std::cout << "::>>> (This point DOES NOT LIE WITHIN the triangle!) " << in_line.pointB.x << ", " << in_line.pointB.y << ", " << in_line.pointB.z << std::endl;
					candidateProductionLogger.log("::>>> (This point DOES NOT LIE WITHIN the triangle!) ", in_line.pointB.x, ", ", in_line.pointB.y, ", ", in_line.pointB.z, "\n");
					returnResult.setResult(0);
				}

				//std::cout << "## Finished checking if point lies within triangle...(determineRayRelationShipToTriangle)" << std::endl;
				//int comparisonVal = 3;
				//std::cin >> comparisonVal;

				//returnResult.setResult(2);
			}
			//else return 0;              // ray disjoint from plane
			else returnResult.setResult(0);
		}

		// check whether or not point B of the line matches a point
		for (int x = 0; x < 3; x++)
		{
			if (in_line.pointB == in_triangle.triangleLines[x].pointA)
			{
				//std::cout << "!!:##### Notice: although the ray isn't coplanar to the triangle (result of 2), one point was found as being equal to a point in the compared-to triangle. Flagging as 1. " << std::endl;
				candidateProductionLogger.log("!!:##### Notice: although the ray isn't coplanar to the triangle (result of 2), one point was found as being equal to a point in the compared-to triangle. Flagging as 1. ", "\n");
				returnResult.matchesPointExact = true;
				returnResult.setResult(1);
			}
		}


		float r = a / b;
		if (r < 0.0)
		{
			//std::cout << "!! Note: r is  less than 0. " << std::endl;
			//return 0;
			returnResult.setResult(0);
		}

		intersect_candidate = in_line.pointA + (r * dir);

		// is I inside T?
		glm::vec3 w;
		float    uu, uv, vv, wu, wv, D;
		uu = dot(u, u);
		uv = dot(u, v);
		vv = dot(v, v);
		w = intersect_candidate - point0;
		wu = dot(w, u);
		wv = dot(w, v);
		D = uv * uv - uu * vv;

		// get and test parametric coords
		float s, t;
		s = (uv * wv - vv * wu) / D;
		//std::cout << "--> S, pre-round: " << s << std::endl;

		s = float(floor(s * 1000 + 0.5) / 1000);

		//std::cout << "--> Value of s: " << s << std::endl;
		//std::cout << "--> S, post-round: " << s << std::endl;
		//comparisonLogger.log("--> Value of s: ", s, "\n");
		if (s < 0.0 || s > 1.001f)         // I is outside S
			//return 0;
		{
			//std::cout << "!! Note: I is outside S. " << std::endl;
			returnResult.setResult(0);
		}
		t = (uv * wu - uu * wv) / D;
		//std::cout << "--> T, pre-round: " << t << std::endl;

		t = float(floor(t * 1000 + 0.5) / 1000);
		//std::cout << "--> T, post-round: " << t << std::endl;

		//std::cout << "--> Value of t: " << t << std::endl;
		//comparisonLogger.log("--> Value of t: ", t, "\n");
		if (t < 0.0 || (s + t) > 1.001f)  // I is outside T
			//return 0;
		{
			//std::cout << "!! Note: I is outside T. " << std::endl;
			//comparisonLogger.log("!! Note: I is outside T. ", "\n");
			returnResult.setResult(0);
		}
		//return 1;
		returnResult.setResult(1);
		returnResult.wasIntersectOnBorderLine = in_line.isBorderLine;
		returnResult.borderLineID = in_line.borderLineID;
		returnResult.intersectedPoint = roundPointToHundredths(intersect_candidate);					// the intercept point should be rounded to hundredths!



		//std::cout << "(Fusion) Resulting intersect attempt value: " << returnResult.wasIntersectFound << std::endl;
		candidateProductionLogger.log("(Fusion) Resulting intersect attempt value: ", returnResult.wasIntersectFound, "\n");

	}
	return returnResult;
}