#include "stdafx.h"
#include "SPolySet.h"

#define SMALL_NUM   0.00000001 // anything that avoids division overflow

void SPolySet::addPoly(SPoly in_sPoly)
{
	secondaryPolys[numberOfPolys] = in_sPoly;		// add the poly, and increment it
	polyMorphTracker.morphablePolys[numberOfPolys] = in_sPoly;
	numberOfPolys++;
}

void SPolySet::reset()
{
	numberOfPolys = 0;
}

void SPolySet::configurePolys()
{
	for (int x = 0; x < numberOfPolys; x++)
	{
		// PHASE 1: determine border lines, empty normals, and planar normals
		secondaryPolys[x].determinePrimalPoints();		// determine the primal points for the triangle
		secondaryPolys[x].determineBorderLines();		// for each SPoly, determine its border lines.
		secondaryPolys[x].calculateEmptyNormal();		// find the empty normal for each poly
	}
}

void SPolySet::configurePolysWithoutNormalCalcs()
{
	for (int x = 0; x < numberOfPolys; x++)
	{
		// PHASE 1: determine border lines, and planar normals; empty normals would already be calculated for these secondaries, if using this function as intended.
		secondaryPolys[x].determinePrimalPoints();		// determine the primal points for the triangle
		secondaryPolys[x].determineBorderLines();		// for each SPoly, determine its border lines.
	}
}

void SPolySet::runPolyComparison()
{
	/*
	int comparisonCount = numberOfPolys - 1;	// the number of SPolys that the current poly will compare to is equal to the number of polys - 1
	for (int x = 0; x < numberOfPolys; x++)
	{
		int currentIndex = 0;	// the current index of what we will be comparing to
		for (int y = 0; y < comparisonCount; y++)
		{
			if (x == y)		// don't compare a poly to itself; skip this iteration
			{
				currentIndex++;
			}
			std::cout << "Comparing poly ID " << x << " to poly ID " << currentIndex << std::endl;
			std::cout << "!!! Number of polys is: " << numberOfPolys << std::endl;

			SPoly* polyA = &secondaryPolys[x];
			SPoly* polyB = &secondaryPolys[currentIndex];
			checkForIntersections(polyA, x, polyB, currentIndex);

		}
	}
	*/

	int compCount2 = numberOfPolys;
	for (int x = 0; x < compCount2; x++)
	{
		int currentIndex = 0;						// index begins at 0, increments every tick of the y loop that follows.
		for (int y = 0; y < numberOfPolys; y++)
		{
			if (!(x == y))	// never compare the poly to itself
			{
				std::cout << "Comparing poly ID " << x << " to poly ID " << currentIndex << std::endl;
				std::cout << "!!! Number of polys is: " << numberOfPolys << std::endl;

				SPoly* polyA = &secondaryPolys[x];
				SPoly* polyB = &secondaryPolys[currentIndex];
				checkForIntersections(polyA, x, polyB, currentIndex);		// PHASE 1
			}
			currentIndex++;
		}
	}
}

int SPolySet::checkForIntersections(SPoly* in_polyAPtr, int in_polyAID, SPoly* in_polyBPtr, int in_polyBID)
{
	int numberOfIntersections = 0;

	int polyATertiaryCount = in_polyAPtr->numberOfTriangles;
	int polyBTertiaryCount = in_polyBPtr->numberOfTriangles;

	std::cout << "polyA count: " << polyATertiaryCount << std::endl;
	std::cout << "polyB count: " << polyBTertiaryCount << std::endl;

	//std::cout << ">>> A lines are: " << std::endl;
	//std::cout << "0: " << in_polyAPtr->triangles[0].triangleLines[0].pointA.x << ", " << in_polyAPtr->triangles[0].triangleLines[0].pointA.y << ", " << in_polyAPtr->triangles[0].triangleLines[0].pointA.z << " ||| " << in_polyAPtr->triangles[0].triangleLines[0].pointB.x << ", " << in_polyAPtr->triangles[0].triangleLines[0].pointB.y << ", " << in_polyAPtr->triangles[0].triangleLines[0].pointB.z << std::endl;
	//std::cout << "1: " << in_polyAPtr->triangles[0].triangleLines[1].pointA.x << ", " << in_polyAPtr->triangles[0].triangleLines[1].pointA.y << ", " << in_polyAPtr->triangles[0].triangleLines[1].pointA.z << " ||| " << in_polyAPtr->triangles[0].triangleLines[1].pointB.x << ", " << in_polyAPtr->triangles[0].triangleLines[1].pointB.y << ", " << in_polyAPtr->triangles[0].triangleLines[1].pointB.z << std::endl;
	//std::cout << "2: " << in_polyAPtr->triangles[0].triangleLines[2].pointA.x << ", " << in_polyAPtr->triangles[0].triangleLines[2].pointA.y << ", " << in_polyAPtr->triangles[0].triangleLines[2].pointA.z << " ||| " << in_polyAPtr->triangles[0].triangleLines[2].pointB.x << ", " << in_polyAPtr->triangles[0].triangleLines[2].pointB.y << ", " << in_polyAPtr->triangles[0].triangleLines[2].pointB.z << std::endl;



	for (int x = 0; x < polyATertiaryCount; x++)					// compare each of poly A's tertiaries...
	{
		// >>>>>>>>>>>>>>>>>>>>> STEP 1
		// compare A's line's to B
		STriangle* polyATrianglePtr = &in_polyAPtr->triangles[x];	// " " 
		IntersectionLineGroup polyALineGroup;						// the line group for poly A.
		IntersectionLineGroup polyBLineGroup;						// the line group for poly B.
		for (int y = 0; y < polyBTertiaryCount; y++)					// .. to each of poly B's tertiaries...
		{
			// for each pair (that is, A's current STriangle to B's current STriangle in the iterations), we must:
			// 1. compare the lines of STriangle A to that of STriangle B
			// 2. compare the lines of STriangle B to that of STriangle A
			// 3. analyze the relationship between them


			// STEP 1
			STriangle* polyBTrianglePtr = &in_polyBPtr->triangles[y]; // get the triangle of B
			//std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>> Comparing A to B " << std::endl;
			for (int z = 0; z < 3; z++)		// run the lines of A through B 
			{
				IntersectionResult intersectResult = checkIfLineIntersectsTriangle(*polyBTrianglePtr, in_polyAPtr->triangles[x].triangleLines[z]);		// check if poly A's line intersected B...if they did, we need to add them to the IntersectionLine

				IntersectionLine potentialLineAtoB;		// the line that will store the intersections.
				if (intersectResult.wasIntersectFound == 1)
				{
					//std::cout <<  "Line " << z << " intersects " << std::endl;
					if (intersectResult.wasIntersectOnBorderLine == 1)
					{
						std::cout << "Intersecting line was a border line. It's ID is: " << intersectResult.borderLineID << std::endl;
						//std::cout << "Line points: " << in_polyAPtr->triangles[x].triangleLines[z].pointA.x << ", " << in_polyAPtr->triangles[x].triangleLines[z].pointA.y << ", " << in_polyAPtr->triangles[x].triangleLines[z].pointA.z << " || "
						//							 << in_polyAPtr->triangles[x].triangleLines[z].pointB.x << ", " << in_polyAPtr->triangles[x].triangleLines[z].pointB.y << ", " << in_polyAPtr->triangles[x].triangleLines[z].pointB.z << std::endl;
						//potentialLineAtoB.numberOfBorderLines = 1;
					}
					else
					{
						std::cout << "Intersecting line was NOT a border line. (A to B)" << std::endl;
					}
					//std::cout << "(1) ## lines is now: " << potentialLineAtoB.numberOfBorderLines << std::endl;
					potentialLineAtoB.addIntersectionResult(intersectResult);		// add the result to the intersect line
					potentialLineAtoB.intersectedSecondaryID = in_polyBID;			// store the ID of the secondary that was intersected; this should always be B
					//std::cout << "(2) ## lines is now: " << potentialLineAtoB.numberOfBorderLines << std::endl;
					polyALineGroup.addIntersectionLine(potentialLineAtoB);			// only add a line to the group if the line intersection wtih poly B
				}
				//std::cout << "Current number of border lines is: " << potentialLineAtoB.numberOfBorderLines << std::endl;
			}
			// merge the found intersections for the candidate line, then make that result = potentialLineAtoB;
			IntersectionLine mergedLineAtoB = polyALineGroup.mergeLines();

			// STEP 2
			//std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>> Comparing B to A" << std::endl;
			for (int z = 0; z < 3; z++)
			{
				IntersectionResult intersectResult = checkIfLineIntersectsTriangle(*polyATrianglePtr, polyBTrianglePtr->triangleLines[z]);				// check if poly B's line intersected A...if they did, we need to add them to the IntersectionLine

				IntersectionLine potentialLineBtoA;		// the line that will store the intersections.
				if (intersectResult.wasIntersectFound == 1)
				{
					//std::cout << "Line " << z << " intersects " << std::endl;
					if (intersectResult.wasIntersectOnBorderLine == 1)
					{
						std::cout << "Intersecting line was a border line. It's ID is: " << intersectResult.borderLineID << std::endl;
						//std::cout << "Line points: " << polyBTrianglePtr->triangleLines[z].pointA.x << ", " << polyBTrianglePtr->triangleLines[z].pointA.y << ", " << polyBTrianglePtr->triangleLines[z].pointA.z << " || "
							//<< polyBTrianglePtr->triangleLines[z].pointB.x << ", " << polyBTrianglePtr->triangleLines[z].pointB.y << ", " << polyBTrianglePtr->triangleLines[z].pointB.z << std::endl;
						//potentialLineBtoA.numberOfBorderLines = 1;
					}
					else
					{
						std::cout << "Intersecting line was NOT a border line. (B to A)" << std::endl;
					}
					potentialLineBtoA.addIntersectionResult(intersectResult);		// add the result to the intersect line
					potentialLineBtoA.intersectedSecondaryID = in_polyBID;			// store the ID of the secondary that was intersected; this should always be B
					polyBLineGroup.addIntersectionLine(potentialLineBtoA);			// only add a line to the group if the line intersection wtih poly B
				}
			}
			// merge the found intersections for the candidate line, then make that result = potentialLineBtoA;
			IntersectionLine mergedLineBtoA = polyBLineGroup.mergeLines();

			// STEP 3
			// compare the IntersectionLines to determine the type of interect (if any) that was generated
			CategorizedLine currentCategorizedLine = determineCategorizedLine(mergedLineAtoB, mergedLineBtoA, in_polyBPtr->groupID, in_polyBPtr->polyEmptyNormal);	// find out what type of line this is; assign the appropriate groupID to the line

			// STEP 4
			// add any CategorizedLine to polygonA's map that isn't NONE
			if (currentCategorizedLine.type != IntersectionType::NONE)	// only add the line to polygon A's map if it was a valid intersection.
			{
				in_polyAPtr->addCategorizedLine(currentCategorizedLine);	// add the new line
				numberOfIntersections++;
			}

			polyALineGroup.reset();
			polyBLineGroup.reset();
		}



	}
	std::cout << "Current poly cleave size: " << in_polyAPtr->cleaveMap.size() << std::endl;
	in_polyAPtr->printLines();
	in_polyAPtr->moveLastCleave();		// move any remaining lines into a cleave, if they exist
	std::cout << "Current poly cleave size: " << in_polyAPtr->cleaveMap.size() << std::endl;
	in_polyAPtr->printAllCleaveLines();
	return numberOfIntersections;
}


IntersectionResult SPolySet::checkIfLineIntersectsTriangle(STriangle in_triangle, STriangleLine in_triangleLine)
{
	//int someVal = 0;
	IntersectionResult returnResult;
	glm::vec3 intersect_candidate;

	glm::vec3 point0 = in_triangle.triangleLines[0].pointA;
	glm::vec3 point1 = in_triangle.triangleLines[1].pointA;
	glm::vec3 point2 = in_triangle.triangleLines[2].pointA;

	glm::vec3 u = point1 - point0;	// u vector
	glm::vec3 v = point2 - point0;	// v " 

	//std::cout << "U: " << u.x << ", " << u.y << ", " << u.z << std::endl;
	//std::cout << "V: " << v.x << ", " << v.y << ", " << v.z << std::endl;
	glm::vec3 N = cross(u, v);		// the normal of the triangle
	//N.z = -1.0f;
	//std::cout << "Normal: " << N.x << ", " << N.y << ", " << N.z << std::endl;

	glm::vec3 dir = in_triangleLine.pointB - in_triangleLine.pointA;
	glm::vec3 w0 = in_triangleLine.pointA - point0;

	float a = -dot(N, w0);
	float b = dot(N, dir);
	if (fabs(b) < SMALL_NUM) {     // ray is  parallel to triangle plane
		if (a == 0)                 // ray lies in triangle plane
			//return 2;
			returnResult.setResult(2);
		//else return 0;              // ray disjoint from plane
		else returnResult.setResult(0);
	}

	float r = a / b;
	if (r < 0.0)
		//return 0;
		returnResult.setResult(0);

	intersect_candidate = in_triangleLine.pointA + (r * dir);

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
	if (s < 0.0 || s > 1.0)         // I is outside T
		//return 0;
		returnResult.setResult(0);
	t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		//return 0;
		returnResult.setResult(0);
	//return 1;
	returnResult.setResult(1);
	returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
	returnResult.borderLineID = in_triangleLine.borderLineID;
	returnResult.intersectedPoint = roundPointToHundredths(intersect_candidate);					// the intercept point should be rounded to hundredths!
	return returnResult;
	//return someVal;
}


glm::vec3 SPolySet::cross(glm::vec3 in_A, glm::vec3 in_B)
{
	glm::vec3 returnVec;

	returnVec.x = (in_A.y * in_B.z) - (in_A.z * in_B.y);
	returnVec.y = (in_A.z * in_B.x) - (in_A.x * in_B.z);
	returnVec.z = (in_A.x * in_B.y) - (in_A.y * in_B.x);

	return returnVec;
}

float SPolySet::dot(glm::vec3 in_A, glm::vec3 in_B)
{
	float returnVal;
	float x_multiplied = in_A.x * in_B.x;
	float y_multiplied = in_A.y * in_B.y;
	float z_multiplied = in_A.z * in_B.z;
	return x_multiplied + y_multiplied + z_multiplied;
}

CategorizedLine SPolySet::determineCategorizedLine(IntersectionLine in_lineA, IntersectionLine in_lineB, int in_groupID, glm::vec3 in_polyBEmptyNormal)
{
	CategorizedLine returnLine;
	// CASE 1: A is SLICED
	//std::cout << "Line A, " << in_lineA.numberOfBorderLines << std::endl;
	//std::cout << "Line B, " << in_lineB.numberOfBorderLines << std::endl;

	if (in_lineA.numberOfBorderLines == 2)
	{
		//std::cout << "This line is a SLICE" << std::endl;
		returnLine.convertLineToSlice(in_lineA);		// convert to SLICE, by sending in the slicing line, in_lineA
	}

	// CASE 2: B is SLICED, but A "engulfs" B
	else if
		(
		(in_lineB.numberOfBorderLines == 2)
			&&
			(in_lineA.numberOfBorderLines == 0)
			)
	{
		//std::cout << "Triangle A has engulfed triangle B; this is a NON_BOUND" << std::endl;
		returnLine.convertLineToNonbound(in_lineB);	// convert to NONBOUND, by sending in the engulfed line (which is in_lineB)
	}

	// CASE 3: PARTIAL_BOUND (two different conditions)
	// condition 3.1:
	else if
		(
		(in_lineB.numberOfBorderLines == 2)
			&&
			(in_lineA.numberOfBorderLines == 1)
			)
	{
		//std::cout << "Triangle A has one border line hit by B; this is a PARTIAL_BOUND" << std::endl;

		//std::cout << "TA p0: " << in_lineA.pointA.x << ", " << in_lineA.pointA.y << ", " << in_lineA.pointA.z << std::endl;
		//std::cout << "TB p0: " << in_lineB.pointA.x << ", " << in_lineB.pointA.y << ", " << in_lineB.pointA.z << std::endl;
		//std::cout << "TB p1: " << in_lineB.pointB.x << ", " << in_lineB.pointB.y << ", " << in_lineB.pointB.z << std::endl;
		//std::cout << "Line A stats: point A border = " << in_lineA.isPointAOnBorder << " | point B border = " << in_lineA.isPointBOnBorder << std::endl;
		//std::cout << "Line B stats: point A border = " << in_lineB.isPointAOnBorder << " | point B border = " << in_lineB.isPointBOnBorder << std::endl;

		glm::vec3 newSecondPoint = findSecondPointForLine(in_lineA.pointA, in_lineB.pointA, in_lineB.pointB);
		//std::cout << "Second point is: " << newSecondPoint.x << ", " << newSecondPoint.y << ", " << newSecondPoint.z << std::endl;
		returnLine.convertLineToPartialBound(in_lineA, in_lineB, newSecondPoint);	// convert to PARTIAL_BOUND
	}
	// condition 3.2:
	else if
		(
		(in_lineA.numberOfBorderLines == 1)
			&&
			(in_lineB.numberOfBorderLines == 1)
			)
	{
		/*
		std::cout << "PARTIAL_BOUND via condition 3.2 detected. " << std::endl;
		std::cout << "Line A point count: " << in_lineA.numberOfPoints << std::endl;
		std::cout << "Line B point count: " << in_lineB.numberOfPoints << std::endl;
		std::cout << "Line A is: " << in_lineA.pointA.x << ", " << in_lineA.pointA.y << ", " << in_lineA.pointA.z << " | " << in_lineA.pointB.x << ", " << in_lineA.pointB.y << ", " << in_lineA.pointB.z << std::endl;
		std::cout << "Line B is: " << in_lineB.pointA.x << ", " << in_lineB.pointA.y << ", " << in_lineB.pointA.z << " | " << in_lineB.pointB.x << ", " << in_lineB.pointB.y << ", " << in_lineB.pointB.z << std::endl;
		std::cout << "-----Borders: " << std::endl;
		std::cout << "Line A: " << std::endl;
		std::cout << "Point A: " << in_lineA.pointABorder << std::endl;
		std::cout << "Point B: " << in_lineA.pointBBorder << std::endl;
		std::cout << "Line B: " << std::endl;
		std::cout << "Point A: " << in_lineB.pointABorder << std::endl;
		std::cout << "Point B: " << in_lineB.pointBBorder << std::endl;
		*/
		if (in_lineA.numberOfPoints == 2)	// one of these should be 2
		{
			returnLine.type = IntersectionType::PARTIAL_BOUND;		// TWIN always has exactly one point on a border line, and a non-bound point.
			returnLine.line.numberOfBorderLines = 1;
			returnLine.line.isPointAOnBorder = 1;
			returnLine.line.pointABorder = in_lineA.pointABorder;
			returnLine.line.pointA = in_lineA.pointA;
			returnLine.line.pointB = in_lineA.pointB;
			returnLine.line.intersectedSecondaryID = in_lineA.intersectedSecondaryID;
		}
		else if (in_lineB.numberOfPoints == 2)
		{
			returnLine.type = IntersectionType::PARTIAL_BOUND;		// TWIN always has exactly one point on a border line, and a non-bound point.
			returnLine.line.numberOfBorderLines = 1;
			returnLine.line.isPointAOnBorder = 1;
			returnLine.line.pointABorder = in_lineA.pointABorder;
			returnLine.line.pointA = in_lineB.pointA;
			returnLine.line.pointB = in_lineB.pointB;
			returnLine.line.intersectedSecondaryID = in_lineB.intersectedSecondaryID;
		}
		//glm::vec3 newSecondPoint = findSecondPointForLine(in_lineA.pointA, in_lineB.pointA, in_lineB.pointB);
		//returnLine.convertLineToPartialBound(in_lineA, in_lineB);		// convert to TWIN
		//returnLine.convertLineToPartialBound(in_lineA, in_lineB, newSecondPoint);
		std::cout << "!!! Return line border is: " << returnLine.line.pointABorder << std::endl;

	}
	// CASE 4: A has one line, B has one line; this is a TWIN
	else if
		(
		(in_lineA.numberOfPoints == 1)	// check the number of point intercepts (not lines)
			&&
			(in_lineA.numberOfBorderLines == 1)

			&&

			(in_lineB.numberOfPoints == 1)	// " "
			&&
			(in_lineB.numberOfBorderLines == 1)
			)
	{
		std::cout << "twin-style PARTIAL_BOUND detected. " << std::endl;
		glm::vec3 newSecondPoint = findSecondPointForLine(in_lineA.pointA, in_lineB.pointA, in_lineB.pointB);
		//returnLine.convertLineToPartialBound(in_lineA, in_lineB);		// convert to TWIN
		returnLine.convertLineToPartialBound(in_lineA, in_lineB, newSecondPoint);
	}

	// CASE 5: NON-BOUND line is formed

	else if
		(
		(in_lineA.numberOfPoints == 1)
			&&
			(in_lineA.numberOfBorderLines == 0)

			&&

			(in_lineB.numberOfPoints == 1)
			&&
			(in_lineB.numberOfBorderLines == 0)
			)
	{
		std::cout << "NON-BOUND case 2 hit " << std::endl;
		glm::vec3 roundedA = in_lineA.pointA;
		glm::vec3 roundedB = in_lineB.pointA;
		if (checkIfPointsMatch(roundedA, roundedB) == 0)		// it can only be a valid line if the two points that make up the line do not match
		{
			returnLine.convertLinesToNonbound(in_lineA, in_lineB);
		}
		else
		{
			returnLine.type = IntersectionType::NONE;	// it's invalid, so set to 1
		}
	}

	// CASE 6: there is no intercept between them.
	else if
		(
		(in_lineA.numberOfPoints == 0)
			&&
			(in_lineB.numberOfPoints == 0)
			)
	{
		// do nothing here; default value of CategorizedLine.type is IntersectionType::NONE
		std::cout << "No intercept detected. " << std::endl;
	}
	returnLine.line.lineGroupID = in_groupID;
	returnLine.emptyNormal = in_polyBEmptyNormal;
	std::cout << "!!!-> GroupID is: " << in_groupID << std::endl;
	return returnLine;
}

glm::vec3 SPolySet::findSecondPointForLine(glm::vec3 in_beginPoint, glm::vec3 in_candidate1, glm::vec3 in_candidate2)
{
	// round before comparing
	glm::vec3 secondPoint;

	glm::vec3 rounded_begin = roundPointToHundredths(in_beginPoint);
	glm::vec3 rounded_candidate1 = roundPointToHundredths(in_candidate1);
	glm::vec3 rounded_candidate2 = roundPointToHundredths(in_candidate2);
	if (checkIfPointsMatch(in_beginPoint, in_candidate1) == 1)	// do they match? 
	{
		secondPoint = in_candidate2;	// yes; then candidate 2 
		//std::cout << "####### second point is candidate 2. " << std::endl;
	}
	else
	{
		secondPoint = in_candidate1;	// otherwise, candidate 1
		//std::cout << "####### second point is candidate 1. " << std::endl;
	}
	return secondPoint;
}

glm::vec3 SPolySet::roundPointToHundredths(glm::vec3 in_point)
{
	glm::vec3 returnPoint;
	returnPoint.x = floor(in_point.x * 100 + 0.5) / 100;
	returnPoint.y = floor(in_point.y * 100 + 0.5) / 100;
	returnPoint.z = floor(in_point.z * 100 + 0.5) / 100;
	return returnPoint;
}

int SPolySet::checkIfPointsMatch(glm::vec3 in_pointA, glm::vec3 in_pointB)
{
	int matchFound = 0;
	if
		(
		(in_pointA.x == in_pointB.x)
			&&
			(in_pointA.y == in_pointB.y)
			&&
			(in_pointA.z == in_pointB.z)
			)
	{
		matchFound = 1;
	}
	return matchFound;
}

float SPolySet::getRadiansForPosZViaY(glm::vec3 in_vec3)
{
	float degreesToRotateOnY = 0.0f;
	float fullRadian360 = 6.28319;	// 360 degrees = this many radians


	// get the atan2 result, and analyze it
	float atan2result = atan2(in_vec3.z, in_vec3.x);
	float firstPassRotateRadians;
	std::cout << "Atan result is: " << atan2result << std::endl;
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

	glm::vec3 rotationAngle;
	rotationAngle.y = 1;
	glm::quat rotationQuat = createQuaternion(firstPassRotateRadians, rotationAngle);
	glm::vec3 rotationResult = rotationQuat * in_vec3;

	glm::vec3 targetPosZ;
	targetPosZ.z = 1.0f;
	targetPosZ = rotationQuat * targetPosZ;

	std::cout << "Rotated point is: " << rotationResult.x << ", " << rotationResult.y << ", " << rotationResult.z << std::endl;
	std::cout << "Target pos z is at: " << targetPosZ.x << ", " << targetPosZ.y << ", " << targetPosZ.z << std::endl;
	std::cout << "Rotated by this many radians: " << firstPassRotateRadians << std::endl;

	std::cout << ">>>> Attempting second pass rotation... " << std::endl;

	float secondPassAtan = atan2(targetPosZ.z, targetPosZ.x);
	std::cout << ">>>> Seond pass a-tan is: " << secondPassAtan << std::endl;
	float secondPassRotateRadians;
	if (secondPassAtan > 0.0)
	{
		secondPassRotateRadians = secondPassAtan;
	}
	else if (secondPassAtan < 0.0)
	{
		secondPassRotateRadians = fullRadian360 + secondPassAtan;
	}
	float secondPassRotateFinal = fullRadian360 - secondPassRotateRadians;
	glm::quat secondRotationQuat = createQuaternion(secondPassRotateFinal, rotationAngle);
	glm::vec3 finalRotatedVec = secondRotationQuat * in_vec3;
	std::cout << "Final rotated vec is: " << finalRotatedVec.x << ", " << finalRotatedVec.y << ", " << finalRotatedVec.z << std::endl;
	std::cout << "It was rotated to pos Z, via this many radians: " << secondPassRotateFinal << std::endl;

	return degreesToRotateOnY;
}

float SPolySet::getRadiansForPosYViaX(glm::vec3 in_vec3)
{
	float degreesToRotateOnX = 0.0f;
	float fullRadian360 = 6.28319;	// 360 degrees = this many radians

	// get the atan2 result, and analyze it
	float atan2result = atan2(in_vec3.y, in_vec3.z);
	float firstPassRotateRadians;
	std::cout << "Atan result is: " << atan2result << std::endl;
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

	glm::vec3 rotationAngle;
	rotationAngle.x = 1;
	glm::quat rotationQuat = createQuaternion(firstPassRotateRadians, rotationAngle);
	glm::vec3 rotationResult = rotationQuat * in_vec3;

	glm::vec3 targetPosY;
	targetPosY.y = 1.0f;
	targetPosY = rotationQuat * targetPosY;

	std::cout << "Rotated point is: " << rotationResult.x << ", " << rotationResult.y << ", " << rotationResult.z << std::endl;
	std::cout << "Target pos Y is at: " << targetPosY.x << ", " << targetPosY.y << ", " << targetPosY.z << std::endl;
	std::cout << "Rotated by this many radians: " << firstPassRotateRadians << std::endl;

	std::cout << ">>>> Attempting second pass rotation... " << std::endl;

	float secondPassAtan = atan2(targetPosY.y, targetPosY.z);
	std::cout << ">>>> Second pass a-tan is: " << secondPassAtan << std::endl;
	float secondPassRotateRadians;
	if (secondPassAtan > 0.0)
	{
		secondPassRotateRadians = secondPassAtan;
	}
	else if (secondPassAtan < 0.0)
	{
		secondPassRotateRadians = fullRadian360 + secondPassAtan;
	}
	float secondPassRotateFinal = fullRadian360 - secondPassRotateRadians;
	glm::quat secondRotationQuat = createQuaternion(secondPassRotateFinal, rotationAngle);
	glm::vec3 finalRotatedVec = secondRotationQuat * in_vec3;
	std::cout << "Final rotated vec is: " << finalRotatedVec.x << ", " << finalRotatedVec.y << ", " << finalRotatedVec.z << std::endl;
	std::cout << "It was rotated to pos Y, via this many radians: " << secondPassRotateFinal << std::endl;

	return degreesToRotateOnX;
}

glm::quat SPolySet::createQuaternion(float radians, glm::vec3 in_angle)
{
	glm::quat returnQuat;

	// a = angle of rotation, in radians
	// R = [cos(a/2), sin(a/2)*x, sin(a/2)*y, sin(a/2)*z] quaternion formula
	// 90 degrees = 1.5708 radians
	returnQuat = glm::quat(cos(radians / 2), sin(radians / 2)*in_angle.x, sin(radians / 2)*in_angle.y, sin(radians / 2)*in_angle.z);
	return returnQuat;
}

void SPolySet::performFracturing()
{
	// determine what needs to be fractured, by cycling through all secondaryPolys.
	for (int x = 0; x < secondaryPolys.size(); x++)
	{
		if (secondaryPolys[x].cleaveMap.size() != 0)	// fracture only if there are actual cleave map entries
		{
			// pass the secondary poly to the PolyFracturer
			if (secondaryPolys[x].debugFlag == 1)
			{
				std::cout << "~~~~~~~Poly requires fracturing, running..." << std::endl;
			}
			SPolyFracturer fracturer(&secondaryPolys[x], &polyMorphTracker);
		}
	}
}

void SPolySet::runTest1()		// runs use case 1 
{
	// the first poly
	SPoly polyA;
	glm::vec3 pA_0, pA_1, pA_2, pA_3;
	glm::vec3 pA_MRP;		// mass reference point for polygon A

	pA_MRP.x = 0.5f;
	pA_MRP.y = 0.5f;
	pA_MRP.z = -0.5f;

	pA_0.x = 0.0f;
	pA_0.y = 0.0f;
	pA_0.z = 0.0f;

	pA_1.x = 1.0f;
	pA_1.y = 0.0f;
	pA_1.z = 0.0f;

	pA_2.x = 1.0f;
	pA_2.y = 1.0f;
	pA_2.z = 0.0f;

	pA_3.x = 0.0f;
	pA_3.y = 1.0f;
	pA_3.z = 0.0f;

	STriangle polyATriangle(pA_0, pA_1, pA_2);
	STriangle polyATriangle2(pA_0, pA_2, pA_3);
	polyA.groupID = 0;		// set the group ID to 0 (default anyway)
	polyA.setMRP(pA_MRP);
	polyA.addTriangle(polyATriangle);
	polyA.addTriangle(polyATriangle2);
	//polyA.calculateEmptyNormal();


	// the second poly
	SPoly polyB;
	glm::vec3 pB_0, pB_1, pB_2, pB_3;
	glm::vec3 pB_MRP;		// mass reference point for polygon B

	pB_MRP.x = 0.5f;
	pB_MRP.y = 0.2f;
	pB_MRP.z = 0.0f;

	pB_0.x = 1.0f;
	pB_0.y = .4f;
	pB_0.z = .5f;

	pB_1.x = 1.0f;
	pB_1.y = .5f;
	pB_1.z = -.5f;

	pB_2.x = 0.00f;
	pB_2.y = .5f;
	pB_2.z = -.5f;

	pB_3.x = 0.00f;
	pB_3.y = .4f;
	pB_3.z = .5f;




	//pB_1.x = 0.01f;
	//pB_1.y = .5f;
	//pB_1.z = .5f;

	//pB_2.x = 1.0f;
	//pB_2.y = .5f;
	//pB_2.z = -.5f;



	STriangle polyBTriangle(pB_0, pB_1, pB_2);
	STriangle polyBTriangle2(pB_0, pB_2, pB_3);
	polyB.groupID = 1;		// set the group ID to 1
	polyB.setMRP(pB_MRP);
	polyB.addTriangle(polyBTriangle);
	//polyB.addTriangle(polyBTriangle2);
	//polyB.calculateEmptyNormal();
	polyB.setDebugFlag(1);

	addPoly(polyA);
	addPoly(polyB);
	configurePolys();           // PHASE 1 (determine border lines, find empty normals)

	runPolyComparison();		// PHASE 2 (generate cleave lines)

	performFracturing();			// PHASE 3 run weave for new triangles thqat will be fractured

	reset();
}

void SPolySet::runTest2()
{
	// the first poly. group 0 

	SPoly polyA;
	glm::vec3 pA_0, pA_1, pA_2, pA_3;
	pA_0.x = 0.0f;
	pA_0.y = 0.0f;
	pA_0.z = 0.0f;

	pA_1.x = 1.0f;
	pA_1.y = 0.0f;
	pA_1.z = 0.0f;

	pA_2.x = 1.0f;
	pA_2.y = 1.0f;
	pA_2.z = 0.0f;

	pA_3.x = 0.0f;
	pA_3.y = 1.0f;
	pA_3.z = 0.0f;

	STriangle polyATriangle(pA_0, pA_1, pA_2);
	STriangle polyATriangle2(pA_0, pA_2, pA_3);
	polyA.groupID = 0;		// set the group ID to 0 (default anyway)
	polyA.addTriangle(polyATriangle);
	polyA.addTriangle(polyATriangle2);






	// the second poly, group 1
	SPoly polyB;
	glm::vec3 pB_0, pB_1, pB_2, pB_3;

	pB_0.x = 1.0f;
	pB_0.y = .4f;
	pB_0.z = .5f;

	pB_1.x = 1.0f;
	pB_1.y = .4f;
	pB_1.z = -.5f;

	pB_2.x = 0.00f;
	pB_2.y = .4f;
	pB_2.z = -.5f;

	pB_3.x = 0.00f;
	pB_3.y = .4f;
	pB_3.z = .5f;

	STriangle polyBTriangle(pB_0, pB_1, pB_2);
	STriangle polyBTriangle2(pB_0, pB_2, pB_3);
	polyB.groupID = 1;		// set the group ID to 1
	polyB.addTriangle(polyBTriangle);
	polyB.addTriangle(polyBTriangle2);


	// second poly, group 2
	SPoly polyC;
	glm::vec3 pC_0, pC_1, pC_2, pC_3;
	pC_0.x = 1.0f;
	pC_0.y = .8f;
	pC_0.z = .5f;

	pC_1.x = 1.0f;
	pC_1.y = .8f;
	pC_1.z = -.5f;

	pC_2.x = 0.00f;
	pC_2.y = .8f;
	pC_2.z = -.5f;

	pC_3.x = 0.00f;
	pC_3.y = .8f;
	pC_3.z = .5f;

	STriangle polyCTriangle(pC_0, pC_1, pC_2);
	STriangle polyCTriangle2(pC_0, pC_2, pC_3);
	polyC.groupID = 2;
	polyC.addTriangle(polyCTriangle);
	polyC.addTriangle(polyCTriangle2);

	addPoly(polyA);
	addPoly(polyB);
	addPoly(polyC);
	configurePolys();

	runPolyComparison();		// PHASE 1 (generate cleave lines) -- (see email, 8/20/19)
	performFracturing();			// PHASE 3 run weave for new triangles

	reset();

	std::cout << "!!!!! ~~~~~ Test 2 output results, for polyA: " << std::endl;
	secondaryPolys[0].printAllCleaveLines();
}

void SPolySet::runTest3()
{
	// the first poly
	SPoly polyA;
	glm::vec3 pA_0, pA_1, pA_2, pA_3;
	pA_0.x = 0.0f;
	pA_0.y = 0.0f;
	pA_0.z = 0.0f;

	pA_1.x = 1.0f;
	pA_1.y = 0.0f;
	pA_1.z = 0.0f;

	pA_2.x = 1.0f;
	pA_2.y = 1.0f;
	pA_2.z = 0.0f;

	pA_3.x = 0.0f;
	pA_3.y = 1.0f;
	pA_3.z = 0.0f;

	STriangle polyATriangle(pA_0, pA_1, pA_2);
	//STriangle polyATriangle2(pA_0, pA_2, pA_3);
	polyA.groupID = 0;		// set the group ID to 0 (default anyway)
	polyA.addTriangle(polyATriangle);
	//polyA.addTriangle(polyATriangle2);



	// the second poly
	SPoly polyB;
	glm::vec3 pB_0, pB_1, pB_2, pB_3;

	pB_0.x = 0.9f;
	pB_0.y = .5f;
	pB_0.z = .5f;

	pB_1.x = 0.9f;
	pB_1.y = .5f;
	pB_1.z = -.5f;

	pB_2.x = 0.40f;
	pB_2.y = .5f;
	pB_2.z = -.5f;

	pB_3.x = 0.00f;
	pB_3.y = .5f;
	pB_3.z = .5f;




	//pB_1.x = 0.01f;
	//pB_1.y = .5f;
	//pB_1.z = .5f;

	//pB_2.x = 1.0f;
	//pB_2.y = .5f;
	//pB_2.z = -.5f;



	STriangle polyBTriangle(pB_0, pB_1, pB_2);
	//STriangle polyBTriangle2(pB_0, pB_2, pB_3);
	polyB.groupID = 1;		// set the group ID to 1
	polyB.addTriangle(polyBTriangle);
	//polyB.addTriangle(polyBTriangle2);

	addPoly(polyA);
	addPoly(polyB);
	configurePolys();

	runPolyComparison();		// PHASE 1 (generate cleave lines) (see email, 8/20/2019)rc
	performFracturing();			// PHASE 3 run weave for new triangles

	reset();
}