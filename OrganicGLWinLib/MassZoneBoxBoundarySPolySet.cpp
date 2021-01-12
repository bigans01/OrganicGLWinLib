#include "stdafx.h"
#include "MassZoneBoxBoundarySPolySet.h"

#ifndef SMALL_NUM
#define SMALL_NUM   0.000001 // anything that avoids division overflow
#endif

void MassZoneBoxBoundarySPolySet::setBoundarySPolyRef(SPoly* in_sPolyRef)
{
	boundarySPolyRef = in_sPolyRef;

	//std::cout << ">>> Inserted BoundarySPolyRef; which has " << boundarySPolyRef->borderLines.size() << " Lines. " << std::endl;
	//int stopVal = 3;
	//std::cin >> stopVal;
}

void MassZoneBoxBoundarySPolySet::setLogLevel(PolyDebugLevel in_sPolyDebugLevel)
{
	boxBoundarySPolySetLogLevel = in_sPolyDebugLevel;
	boxBoundarySPolySetLogger.setDebugLevel(in_sPolyDebugLevel);
}

void MassZoneBoxBoundarySPolySet::compareSPolySubZoneSPolyToBoundarySPoly(SPoly* in_sPolyRef)
{
	SPoly* polyA = boundarySPolyRef;


	std::cout << "!!! Size of triangles: " << polyA->triangles.size() << std::endl;
	std::cout << "!!! Size of border lines: " << polyA->borderLines.size() << std::endl;
	std::cout << "!!! Size of border lines, direct: " << boundarySPolyRef->borderLines.size() << std::endl;

	//SPoly dumbPoly;
	//std::cout << "!!! Dumb poly size of border lines: " << dumbPoly.borderLines.size() << std::endl;


	SPoly* polyB = in_sPolyRef;
	std::cout << ">>> Entering before coplanarity checker. " << std::endl;
	CoplanarChecker checker(polyA, polyB, boxBoundarySPolySetLogger.getLogLevel());
	std::cout << ">>> Passed coplanarity checker. " << std::endl;
	if (checker.coplanarityDetected == false)	// can only compare to a guest sPoly that is non-coplanar to the boundary SPoly.
	{
		std::cout << " >>> Performing comparison. " << std::endl;
		insertCategorizedLinesFromNonboundarySPoly(in_sPolyRef);
	}

	polyA->sequenceFactory.printLineCounts();
	polyA->sequenceFactory.printLinesInPool();
	int waitVal;
	std::cin >> waitVal;
}

void MassZoneBoxBoundarySPolySet::insertCategorizedLinesFromNonboundarySPoly(SPoly* in_guestPolyPtr)
{
	SPoly* in_hostPolyPtr = boundarySPolyRef;
	int in_guestPolyID = currentComparableSPolyIndex;
	int hostPolyTriangleCount = in_hostPolyPtr->numberOfTriangles;
	int guestPolyTriangleCount = in_guestPolyPtr->numberOfTriangles;

	for (int currentHostPolyTriangle = 0; currentHostPolyTriangle < hostPolyTriangleCount; currentHostPolyTriangle++)					// compare each of poly A's tertiaries...
	{

		//std::cout << "::::::::::::::::::::::::::::::::::: ----------------------------------+++++++++>>>>>>>>>>> Running host poly Triangle comparison: " << std::endl;

		STriangle* hostTrianglePtr = &in_hostPolyPtr->triangles[currentHostPolyTriangle];	// " " 
		IntersectionLineGroup hostLineGroup(boxBoundarySPolySetLogger.getLogLevel());						// the line group for poly A.
		IntersectionLineGroup guestLineGroup(boxBoundarySPolySetLogger.getLogLevel());						// the line group for poly B.
		for (int y = 0; y < guestPolyTriangleCount; y++)					// .. to each of poly B's tertiaries...
		{
			// for each pair (that is, A's current STriangle to B's current STriangle in the iterations), we must:
			// 1. compare the lines of STriangle A to that of STriangle B
			// 2. compare the lines of STriangle B to that of STriangle A
			// 3. analyze the relationship between them


			// >>>>>>>>>>>>>>>>>>>>> STEP 1
			// compare the host triangle lines, to the guest triangles.

			STriangle* guestTrianglePtr = &in_guestPolyPtr->triangles[y]; // get the guest poly's triangle
			//std::cout << "::::::::::::::::::::::::::::::::::: >>>>>>>>>>>>>>>>>>>>>>>>>>> Comparing lines of the host to the guest triangle " << std::endl;
			//std::cout << ">>> B triangle (Guest triangle) points are: " << std::endl;
			//std::cout << "0: " << guestTrianglePtr->triangleLines[0].pointA.x << ", " << guestTrianglePtr->triangleLines[0].pointA.y << ", " << guestTrianglePtr->triangleLines[0].pointA.z << std::endl;
			//std::cout << "1: " << guestTrianglePtr->triangleLines[1].pointA.x << ", " << guestTrianglePtr->triangleLines[1].pointA.y << ", " << guestTrianglePtr->triangleLines[1].pointA.z << std::endl;
			//std::cout << "2: " << guestTrianglePtr->triangleLines[2].pointA.x << ", " << guestTrianglePtr->triangleLines[2].pointA.y << ", " << guestTrianglePtr->triangleLines[2].pointA.z << std::endl;
			//std::cout << "2 (B): " << guestTrianglePtr->triangleLines[2].pointB.x << ", " << guestTrianglePtr->triangleLines[2].pointB.y << ", " << guestTrianglePtr->triangleLines[2].pointB.z << std::endl;

			bool isHostParallelToGuestTriangle = false;
			for (int currentHostTriangleLine = 0; currentHostTriangleLine < 3; currentHostTriangleLine++)		// run the lines of A (the host) through triangle B (the guest)
			{
				// check if any of the host poly's lines intersected the guest poly...
				// if they did, we need to add them to the IntersectionLine

				// an intersection is only valid if both rays that the line uses to trace against the triangle actually intercept, not just one.
				IntersectionResult intersectResult = checkIfLineIntersectsTriangle(*guestTrianglePtr, in_hostPolyPtr->triangles[currentHostPolyTriangle].triangleLines[currentHostTriangleLine]);
				IntersectionLine potentialHostLine;		// the line that will store the intersections.
				//potentialHostLine.intersectionFoundResult = intersectResult.wasIntersectFound;
				if (intersectResult.wasIntersectFound == 1)			// a typical intersection scenario; the line intersected the triangle.
				{
					//std::cout <<  "Line " << z << " intersects " << std::endl;
					if (intersectResult.wasIntersectOnBorderLine == 1)
					{
						//std::cout << "(Host lines going through Guest Triangle) Intersecting line was a border line. It's ID is: " << intersectResult.borderLineID << std::endl;
						//std::cout << "(Host lines going through Guest Triangle) The intersecting point is: " << intersectResult.intersectedPoint.x << ", " << intersectResult.intersectedPoint.y << ", " << intersectResult.intersectedPoint.z << std::endl;
						//std::cout << "(Host lines going through Guest Triangle) Line points: " << in_polyAPtr->triangles[x].triangleLines[z].pointA.x << ", " << in_polyAPtr->triangles[x].triangleLines[z].pointA.y << ", " << in_polyAPtr->triangles[x].triangleLines[z].pointA.z << " || "
						//							 << in_polyAPtr->triangles[x].triangleLines[z].pointB.x << ", " << in_polyAPtr->triangles[x].triangleLines[z].pointB.y << ", " << in_polyAPtr->triangles[x].triangleLines[z].pointB.z << std::endl;
						//potentialLineAtoB.numberOfBorderLines = 1;
					}
					else
					{
						//std::cout << "(Host lines going through Guest Triangle) Intersecting line was NOT a border line. (A to B)" << std::endl;
						//std::cout << "!!! Points are: " << potentialLineAtoB.pointA.x << ", " << potentialLineAtoB.pointA.y << ", " << potentialLineAtoB.pointA.z << " |  " << potentialLineAtoB.pointB.x << ", " << potentialLineAtoB.pointB.y << ", " << potentialLineAtoB.pointB.z << std::endl;
						//std::cout << "(Host lines going through Guest Triangle) The intersecting point is: " << intersectResult.intersectedPoint.x << ", " << intersectResult.intersectedPoint.y << ", " << intersectResult.intersectedPoint.z << std::endl;
						//std::cout << "(Host lines going through Guest Triangle) Line points: " << in_polyAPtr->triangles[x].triangleLines[z].pointA.x << ", " << in_polyAPtr->triangles[x].triangleLines[z].pointA.y << ", " << in_polyAPtr->triangles[x].triangleLines[z].pointA.z << " || "
						//							 << in_polyAPtr->triangles[x].triangleLines[z].pointB.x << ", " << in_polyAPtr->triangles[x].triangleLines[z].pointB.y << ", " << in_polyAPtr->triangles[x].triangleLines[z].pointB.z << std::endl;
					}
					//std::cout << "(1) ## lines is now: " << potentialLineAtoB.numberOfBorderLines << std::endl;
					potentialHostLine.addIntersectionResult(intersectResult);		// add the result to the intersect line
					potentialHostLine.intersectedSecondaryID = in_guestPolyID;			// store the ID of the secondary that was intersected; this should always be B
					//std::cout << "!!! Points are: " << potentialLineAtoB.pointA.x << ", " << potentialLineAtoB.pointA.y << ", " << potentialLineAtoB.pointA.z << " |  " << potentialLineAtoB.pointB.x << ", " << potentialLineAtoB.pointB.y << ", " << potentialLineAtoB.pointB.z << std::endl;
					//std::cout << "(2) ## lines is now: " << potentialLineAtoB.numberOfBorderLines << std::endl;
					hostLineGroup.addIntersectionLine(potentialHostLine);			// only add a line to the group if the line intersection wtih poly B
				}

				if (intersectResult.wasIntersectFound == 2)		// the line was found as being within the triangle's plane
				{
					//std::cout << "!! ++++ NOTICE: ray lies in triangle plane! (host lines compared to guest triangle); setting isHostParallelToGuestTriangle to TRUE." << std::endl;
					//std::cout << "(Host lines going through Guest Triangle) The intersecting point is: " << intersectResult.intersectedPoint.x << ", " << intersectResult.intersectedPoint.y << ", " << intersectResult.intersectedPoint.z << std::endl;
					//std::cout << "(Host lines going through Guest Triangle) Line points: " << in_hostPolyPtr->triangles[currentHostPolyTriangle].triangleLines[currentHostTriangleLine].pointA.x << ", " << in_hostPolyPtr->triangles[currentHostPolyTriangle].triangleLines[currentHostTriangleLine].pointA.y << ", " << in_hostPolyPtr->triangles[currentHostPolyTriangle].triangleLines[currentHostTriangleLine].pointA.z << " || "
								//<< in_hostPolyPtr->triangles[currentHostPolyTriangle].triangleLines[currentHostTriangleLine].pointB.x << ", " << in_hostPolyPtr->triangles[currentHostPolyTriangle].triangleLines[currentHostTriangleLine].pointB.y << ", " << in_hostPolyPtr->triangles[currentHostPolyTriangle].triangleLines[currentHostTriangleLine].pointB.z << std::endl;
				}

				if (intersectResult.wasIntersectFound == 3)		// the line was found as being equal to a line in the triangle it was intersecting
				{
					//std::cout << "!! ++++ NOTICE: ray lies in triangle plane! (host lines compared to guest triangle); setting isHostParallelToGuestTriangle to TRUE." << std::endl;
					isHostParallelToGuestTriangle = true;
				}
				//std::cout << "Current number of border lines is: " << potentialLineAtoB.numberOfBorderLines << std::endl;
			}

			//IntersectionLine mergedHostLine = hostLineGroup.mergeLines();
			// merge the found intersections for the candidate line, then make that result = potentialLineAtoB; only do this if none of the IntersectionLines were
			// found as having their points equal to a line's points in the guest triangle.
			IntersectionLine mergedHostLine;
			if (isHostParallelToGuestTriangle == false)
			{
				mergedHostLine = hostLineGroup.mergeLines();
			}
			std::cout << "::::::::::::::::::::::::::::::::::: >>>>>>>>>>>>>>>>>>>>>>>>>>> ENDED Comparing lines of the host to the guest triangle " << std::endl;

			// >>>>>>>>>>>>>>>>>>>>> STEP 2
			// compare the GUEST triangle lines, to the host triangles.

			//std::cout << "::::::::::::::::::::::::::::::::::: >>>>>>>>>>>>>>>>>>>>>>>>>>> Comparing lines of the guest to the host triangle" << std::endl;
			bool isGuestParallelToHostTriangle = false;
			for (int z = 0; z < 3; z++)		// run the lines of B (the guest) through triangle A (the host)
			{
				IntersectionResult intersectResult = checkIfLineIntersectsTriangle(*hostTrianglePtr, guestTrianglePtr->triangleLines[z]);				// check if poly B's line intersected A...if they did, we need to add them to the IntersectionLine
				IntersectionLine potentialGuestLine;		// the line that will store the intersections.
				//potentialGuestLine.intersectionFoundResult = intersectResult.wasIntersectFound;
				if (intersectResult.wasIntersectFound == 1)
				{
					//std::cout << "Line " << z << " intersects " << std::endl;
					if (intersectResult.wasIntersectOnBorderLine == 1)		// a typical intersection scenario; the line intersected the triangle.
					{
						//std::cout << "(Guest lines going through Host Triangle) Intersecting line was a border line. It's ID is: " << intersectResult.borderLineID << std::endl;
						//std::cout << "(Guest lines going through Host Triangle) The intersecting point is: " << intersectResult.intersectedPoint.x << ", " << intersectResult.intersectedPoint.y << ", " << intersectResult.intersectedPoint.z << std::endl;
						//std::cout << "(Guest lines going through Host Triangle) Line points: " << guestTrianglePtr->triangleLines[z].pointA.x << ", " << guestTrianglePtr->triangleLines[z].pointA.y << ", " << guestTrianglePtr->triangleLines[z].pointA.z << " || "
							//<< guestTrianglePtr->triangleLines[z].pointB.x << ", " << guestTrianglePtr->triangleLines[z].pointB.y << ", " << guestTrianglePtr->triangleLines[z].pointB.z << std::endl;
						//potentialLineBtoA.numberOfBorderLines = 1;
					}
					else
					{
						//std::cout << "(Guest lines going through Host Triangle) Intersecting line was NOT a border line. (B to A)" << std::endl;
						//std::cout << "(Guest lines going through Host Triangle) The intersecting point is: " << intersectResult.intersectedPoint.x << ", " << intersectResult.intersectedPoint.y << ", " << intersectResult.intersectedPoint.z << std::endl;
						//std::cout << "(Guest lines going through Host Triangle) Line points: " << guestTrianglePtr->triangleLines[z].pointA.x << ", " << guestTrianglePtr->triangleLines[z].pointA.y << ", " << guestTrianglePtr->triangleLines[z].pointA.z << " || "
							//<< guestTrianglePtr->triangleLines[z].pointB.x << ", " << guestTrianglePtr->triangleLines[z].pointB.y << ", " << guestTrianglePtr->triangleLines[z].pointB.z << std::endl;
						//std::cout << "Intersecting line was NOT a border line. (B to A)" << std::endl;
					}
					potentialGuestLine.addIntersectionResult(intersectResult);		// add the result to the intersect line
					potentialGuestLine.intersectedSecondaryID = in_guestPolyID;			// store the ID of the secondary that was intersected; this should always be B
					guestLineGroup.addIntersectionLine(potentialGuestLine);			// only add a line to the group if the line intersection wtih poly B
				}
				// for when the intersection line is found as being parallel to the the triangle it attempted to intersect.
				if (intersectResult.wasIntersectFound == 2)		// the line was found as being within the triangle's plane
				{
					std::cout << "!! ++++ NOTICE: ray lies in triangle plane! (guest lines compared to host triangle); setting isHostParallelToGuestTriangle to TRUE." << std::endl;
				}


				if (intersectResult.wasIntersectFound == 3)		// the line was found as being equal to a line in the triangle it was intersecting
				{
					//std::cout << "!! ++++ NOTICE: ray lies in triangle plane! (guest lines compared to host triangle); setting isHostParallelToGuestTriangle to TRUE." << std::endl;
					isGuestParallelToHostTriangle = true;
					//int someVal = 6;
					//std::cin >> someVal;
				}

			}


			//IntersectionLine mergedGuestLine = guestLineGroup.mergeLines();

			// merge the found intersections for the candidate line, then make that result = potentialLineBtoA; only do this if none of the IntersectionLines were
			// found as having their points equal to a line's points in the host triangle.
			IntersectionLine mergedGuestLine;
			if (isGuestParallelToHostTriangle == false)
			{
				//std::cout << ">>>>>>>> GUEST LINE MERGE BEGIN " << std::endl;
				mergedGuestLine = guestLineGroup.mergeLines();
				//std::cout << ">>>>>>>> GUEST LINE MERGE ENd " << std::endl;
				//std::cout << "::: mergedGuestLine stats: " << std::endl;
				//std::cout << ":: pointA: " << mergedGuestLine.pointA.z << ", " << mergedGuestLine.pointA.y << ", " << mergedGuestLine.pointA.z << std::endl;
				//std::cout << ":: pointB: " << mergedGuestLine.pointB.z << ", " << mergedGuestLine.pointB.y << ", " << mergedGuestLine.pointB.z << std::endl;
			}
			//std::cout << "::::::::::::::::::::::::::::::::::: >>>>>>>>>>>>>>>>>>>>>>>>>>> ENDED Comparing lines of the guest to the host triangle" << std::endl;

			// STEP 3
			// compare the IntersectionLines to determine the type of interect (if any) that was generated;
			// it is important to remember that, the "view" or "context" of the categorized line is at is viewed from the host triangle.
			CategorizedLine currentCategorizedLine = determineCategorizedLineThroughHostTriangleContext(mergedHostLine, mergedGuestLine, in_guestPolyPtr->groupID, in_guestPolyPtr->polyEmptyNormal);	// find out what type of line this is; assign the appropriate groupID to the line
			/*
			if (guestLineGroup.lineMap.size() == 3)
			{
				std::cout << "!!! Special check halt, printing out CategorizedLine points and performing tests: " << std::endl;
				std::cout << "Point A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z << std::endl;
				std::cout << "Point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;
				int someVal = 3;
				std::cin >> someVal;

				CategorizedLineColinearTester tester(currentCategorizedLine, *hostTrianglePtr);
			}
			*/

			if (currentCategorizedLine.type == IntersectionType::NONE)	// only add the line to polygon A's map if it was a valid intersection.
			{
				//std::cout << "!!! Warning, line detected as NONE " << std::endl;
			}

			// STEP 4
			// add any CategorizedLine to polygonA's map that isn't NONE
			if (currentCategorizedLine.type != IntersectionType::NONE)	// only add the line to polygon A's map if it was a valid intersection.
			{
				// we must test whether or not the generated categorized line is colinear to another line in the host triangle. If it is
				// colinear, it is invalid. (see the bool flag, tester.colinearDetected)
				CategorizedLineColinearTester tester(currentCategorizedLine, *hostTrianglePtr, boxBoundarySPolySetLogger.getLogLevel());
				//in_polyAPtr->addCategorizedLine(currentCategorizedLine);	// add the new line
				currentCategorizedLine.parentPoly = in_guestPolyID;

				if (currentCategorizedLine.line.pointA == currentCategorizedLine.line.pointB)
				{
					std::cout << "!!! Warning, invalid CategorizedLine detected; points are same! " << std::endl;
					std::cout << "point is: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z << std::endl;
					//std::cout << "___________________ Adding categorized line from this iteration: " << std::endl;
					std::cout << "Point A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z << std::endl;
					std::cout << "Point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;

					if (tester.colinearDetected == true)
					{
						std::cout << "Colinear WAS DETECTED! " << std::endl;
					}
					else if (tester.colinearDetected == false)
					{
						std::cout << "Colinear WAS NOT DETECTED!!" << std::endl;
					}

					int contVal = 3;
					std::cin >> contVal;
				}
				if (tester.colinearDetected == false)		// the categorized line isn't colinear to any line in the host triangle (remember, context is from host triangle)
				{
					in_hostPolyPtr->sequenceFactory.addCategorizedLine(currentCategorizedLine);
					// new code for adding to LineSequenceFactory goes here
					//numberOfIntersections++;
				}
				else if (tester.colinearDetected == true)
				{
					std::cout << "!!#########!!!!!!!!! Categorized line detected as colinear to a line in the host STriangle; will not be inserted. " << std::endl;

					//int someVal = 3;
					//std::cin >> someVal;
				}
			}

			std::cout << "######>>>>>>> Comparison complete, enter number to continue to next comparison. " << std::endl;
			int continueVal = 3;
			std::cin >> continueVal;

			hostLineGroup.reset();
			guestLineGroup.reset();

			//std::cout << "+++++++++ comparing next tertiary..." << std::endl;
		}

		//std::cout << "------------++ done comparing current tertiary in triangle A; moving to next tertiary in triangle A..." << std::endl;

	}

	currentComparableSPolyIndex++;	// this must be incremented, to set the appropriate index for the next SPoly (if there are any to compare against)
}

CategorizedLine MassZoneBoxBoundarySPolySet::determineCategorizedLineThroughHostTriangleContext(IntersectionLine in_hostLine, IntersectionLine in_guestLine, int in_groupID, glm::vec3 in_polyBEmptyNormal)
{
	CategorizedLine returnLine;
	returnLine.type = IntersectionType::NONE;
	// Remember, in_hostLine is the IntersectionLine belonging to the polygon we are adding to (the result of tracing A's line's through the triangle of B).
	// the CategorizedLine type returned should be calculated from polygon A's "view"; meaning, for example, that if A (which is what we are adding to) slices B completely, and A doesn't have any of it's border lines touched in 
	// the process, the line is NON_BOUND (meaning the categorized line exists only in the area of A, and not any border lines)

	std::cout << ">>>>>>>>>>>>> (MassZoneBoxBoundarySPolySet) Calling determine categorized line..." << std::endl;
	std::cout << "Line A, number of points: " << in_hostLine.numberOfPoints << std::endl;
	std::cout << "Line B, number of points: " << in_guestLine.numberOfPoints << std::endl;

	std::cout << "----Line A, number of border lines: " << in_hostLine.numberOfBorderLines << std::endl;
	std::cout << "----Line B, number of border lines: " << in_guestLine.numberOfBorderLines << std::endl;
	//std::cout << "line A, point A: " << in_hostLine.pointA.x << ", " << in_hostLine.pointA.y << ", " << in_hostLine.pointA.z << std::endl;
	//std::cout << "line A, point B: " << in_hostLine.pointB.x << ", " << in_hostLine.pointB.y << ", " << in_hostLine.pointB.z << std::endl;

	//std::cout << "line B, point A: " << in_guestLine.pointA.x << ", " << in_guestLine.pointA.y << ", " << in_guestLine.pointA.z << std::endl;
	//std::cout << "line B, point B: " << in_guestLine.pointB.x << ", " << in_guestLine.pointB.y << ", " << in_guestLine.pointB.z << std::endl;


	//std::cout << "## !! Silly test: " << std::endl;
	//int someVal7 = 3;
	//in_hostLine.lineGroupID = 7;
	//std::cout << "## Host line group ID is now: " << in_hostLine.lineGroupID << std::endl;
	//std::cin >> someVal7;

	// very first check: check for exact same point condition. If this is true, then ignore this determination, as there is a non-existent line.
	bool conditionCheck = checkForSamePointCondition(in_hostLine, in_guestLine);
	int totalNumberOfPoints = in_hostLine.numberOfPoints + in_guestLine.numberOfPoints;
	if (conditionCheck == false)
	{
		std::cout << "!! Points are not the same; " << std::endl;
		// ROOT CASE 1: Both lines are considered valid.
		if
			(
			(in_hostLine.lineValidity == IntersectionLineValidity::VALID)
				)
		{

			// SPECIAL CASE: the guest line is invalid, but the host is not. The host line is a partial bound.
			if
				(
				(in_guestLine.lineValidity == IntersectionLineValidity::INVALID)
					)
			{
				std::cout << "SPECIAL CASE hit " << std::endl;
				int special = 3;
				std::cin >> special;

				if (in_hostLine.numberOfPoints == 2)		// can only perform this special case if the host line has 2 points in it.
				{
					//std::cout << "!! Warning, number of points in host line isn't 2! " << std::endl;


				/*
				std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++ HOST-VALID, GUEST-INVALID, branch entered..." << std::endl;
				std::cout << ":::: testing of new categorized line case, halt. " << std::endl;


				std::cout << "++++++Host line stats: " << std::endl;
				std::cout << "Number of points: " << in_hostLine.numberOfPoints << std::endl;
				std::cout << "Number of border lines: " << in_hostLine.numberOfBorderLines << std::endl;

				std::cout << "++++++Guest line stats: " << std::endl;
				std::cout << "Number of points: " << in_guestLine.numberOfPoints << std::endl;
				std::cout << "Number of border lines: " << in_guestLine.numberOfBorderLines << std::endl;

				std::cout << "line A, point A: " << in_hostLine.pointA.x << ", " << in_hostLine.pointA.y << ", " << in_hostLine.pointA.z << std::endl;
				std::cout << "line A, point B: " << in_hostLine.pointB.x << ", " << in_hostLine.pointB.y << ", " << in_hostLine.pointB.z << std::endl;
				std::cout << "line A, point A border: is on border? ->" << in_hostLine.isPointAOnBorder << "; " << in_hostLine.pointABorder << std::endl;
				std::cout << "line A, point B border: is on border? ->" << in_hostLine.isPointBOnBorder << "; " << in_hostLine.pointBBorder << std::endl;


				std::cout << "line B, point A: " << in_guestLine.pointA.x << ", " << in_guestLine.pointA.y << ", " << in_guestLine.pointA.z << std::endl;
				std::cout << "line B, point B: " << in_guestLine.pointB.x << ", " << in_guestLine.pointB.y << ", " << in_guestLine.pointB.z << std::endl;
				std::cout << "line B, point A border: is on border? ->" << in_guestLine.isPointAOnBorder << "; " << in_guestLine.pointABorder << std::endl;
				std::cout << "line B, point B border: is on border? ->" << in_guestLine.isPointBOnBorder << "; " << in_guestLine.pointBBorder << std::endl;

				std::cout << ":::: END TEST. " << std::endl;
				*/
					returnLine.type = IntersectionType::PARTIAL_BOUND;		// TWIN always has exactly one point on a border line, and a non-bound point.
					returnLine.line.numberOfBorderLines = 1;
					returnLine.line.isPointAOnBorder = 1;
					returnLine.line.pointABorder = in_hostLine.pointABorder;
					returnLine.line.pointA = in_hostLine.pointA;
					returnLine.line.pointB = in_hostLine.pointB;
					returnLine.line.intersectedSecondaryID = in_hostLine.intersectedSecondaryID;
				}
			}


			// CASE 1.1: The host is SLICED (A_SLICE); the host triangle line has two border lines in it
			//std::cout << "Line A, " << in_hostLine.numberOfBorderLines << std::endl;
			//std::cout << "Line B, " << in_guestLine.numberOfBorderLines << std::endl;
			else if (in_hostLine.numberOfBorderLines == 2)			// This means: polygon A had two border lines going through polygon B. That means it is SLICED.
			{
				std::cout << "Number of borders lines = 2 hit " << std::endl;
				returnLine.convertLineToSlice(in_hostLine);		// convert to A_SLICE, by sending in the slicing line, in_hostLine
			}

			// CASE 1.2: the guest triangle is SLICED -- but the triangle being sliced only contains one border line. The host "engulfs" B, in one of two ways; 
			// B may have 1 to 2 border lines being hit, but none of the border lines in A are hit. 
			// This means: A had no border lines go through B, but B had one to two border lines go through A. So B is considered "SLICED." 
			//          
			else if
				(
				(in_guestLine.numberOfBorderLines >= 1)
					&&
					(in_hostLine.numberOfBorderLines == 0)
					&&
					(totalNumberOfPoints > 1)
					)
			{
				std::cout << "CASE 1.2: Triangle A has engulfed triangle B; this is a NON_BOUND" << std::endl;
				/*
				std::cout << "CASE 1.2: Triangle A has engulfed triangle B; this is a NON_BOUND" << std::endl;
				std::cout << "####### halting, temporary, to analyze lines..." << std::endl;

				std::cout << "Host line, point A: " << in_hostLine.pointA.x << ", " << in_hostLine.pointA.y << ", " << in_hostLine.pointA.z << std::endl;
				std::cout << "Host line, point B: " << in_hostLine.pointB.x << ", " << in_hostLine.pointB.y << ", " << in_hostLine.pointB.z << std::endl;
				std::cout << "Host A, number of points: " << in_hostLine.numberOfPoints << std::endl;
				std::cout << "Host A, number of border lines: " << in_hostLine.numberOfBorderLines << std::endl;

				std::cout << "Guest line, point A: " << in_guestLine.pointA.x << ", " << in_guestLine.pointA.y << ", " << in_guestLine.pointA.z << std::endl;
				std::cout << "Guest line, point B: " << in_guestLine.pointB.x << ", " << in_guestLine.pointB.y << ", " << in_guestLine.pointB.z << std::endl;
				std::cout << "Guest line, number of points: " << in_guestLine.numberOfPoints << std::endl;
				std::cout << "Guest line, number of border lines: " << in_guestLine.numberOfPoints << std::endl;
				*/

				IntersectionLine newLine;
				if ((in_guestLine.numberOfPoints == 1) && (in_hostLine.numberOfPoints == 1))
				{
					newLine = in_guestLine;
					newLine.pointB = in_hostLine.pointA;
					newLine.numberOfPoints = 2;
				}
				else if (in_guestLine.numberOfPoints == 2)
				{
					newLine = in_guestLine;
				}


				//int someVal = 3;
				//std::cin >> someVal;

				//returnLine.convertLineToNonbound(in_guestLine);	// convert to NONBOUND, by sending in the engulfed line (which is in_guestLine)
				returnLine.convertLineToNonbound(newLine);	// convert to NONBOUND, by sending in the engulfed line (which is in_guestLine)
			}

			// CASE 1.3: PARTIAL_BOUND -- condition 1
			else if
				(
				(in_guestLine.numberOfBorderLines == 2)
					&&
					(in_hostLine.numberOfBorderLines == 1)
					)
			{
				std::cout << "CASE 1.3: Triangle A has one border line hit by B; this is a PARTIAL_BOUND" << std::endl;

				//std::cout << "TA p0: " << in_hostLine.pointA.x << ", " << in_hostLine.pointA.y << ", " << in_hostLine.pointA.z << std::endl;
				//std::cout << "TB p0: " << in_guestLine.pointA.x << ", " << in_guestLine.pointA.y << ", " << in_guestLine.pointA.z << std::endl;
				//std::cout << "TB p1: " << in_guestLine.pointB.x << ", " << in_guestLine.pointB.y << ", " << in_guestLine.pointB.z << std::endl;
				//std::cout << "Line A stats: point A border = " << in_hostLine.isPointAOnBorder << " | point B border = " << in_hostLine.isPointBOnBorder << std::endl;
				//std::cout << "Line B stats: point A border = " << in_guestLine.isPointAOnBorder << " | point B border = " << in_guestLine.isPointBOnBorder << std::endl;

				glm::vec3 newSecondPoint = findSecondPointForLine(in_hostLine.pointA, in_guestLine.pointA, in_guestLine.pointB);
				//std::cout << "Second point is: " << newSecondPoint.x << ", " << newSecondPoint.y << ", " << newSecondPoint.z << std::endl;
				returnLine.convertLineToPartialBound(in_hostLine, in_guestLine, newSecondPoint);	// convert to PARTIAL_BOUND
			}
			// CASE 1.4: PARTIAL_BOUND -- condition 1
			else if
				(
				(in_hostLine.numberOfBorderLines == 1)
					&&
					(in_guestLine.numberOfBorderLines == 1)
					)
			{
				std::cout << "PARTIAL_BOUND via condition 1.4 detected. " << std::endl;
				//std::cout << ":: Line A point count: " << in_hostLine.numberOfPoints << std::endl;
				//std::cout << ":: Line B point count: " << in_guestLine.numberOfPoints << std::endl;
				//std::cout << ":: Line A is: " << in_hostLine.pointA.x << ", " << in_hostLine.pointA.y << ", " << in_hostLine.pointA.z << " | " << in_hostLine.pointB.x << ", " << in_hostLine.pointB.y << ", " << in_hostLine.pointB.z << std::endl;
				//std::cout << ":: Line B is: " << in_guestLine.pointA.x << ", " << in_guestLine.pointA.y << ", " << in_guestLine.pointA.z << " | " << in_guestLine.pointB.x << ", " << in_guestLine.pointB.y << ", " << in_guestLine.pointB.z << std::endl;
				/*
				std::cout << "PARTIAL_BOUND via condition 3.2 detected. " << std::endl;
				std::cout << "Line A point count: " << in_hostLine.numberOfPoints << std::endl;
				std::cout << "Line B point count: " << in_guestLine.numberOfPoints << std::endl;
				std::cout << "Line A is: " << in_hostLine.pointA.x << ", " << in_hostLine.pointA.y << ", " << in_hostLine.pointA.z << " | " << in_hostLine.pointB.x << ", " << in_hostLine.pointB.y << ", " << in_hostLine.pointB.z << std::endl;
				std::cout << "Line B is: " << in_guestLine.pointA.x << ", " << in_guestLine.pointA.y << ", " << in_guestLine.pointA.z << " | " << in_guestLine.pointB.x << ", " << in_guestLine.pointB.y << ", " << in_guestLine.pointB.z << std::endl;
				std::cout << "-----Borders: " << std::endl;
				std::cout << "Line A: " << std::endl;
				std::cout << "Point A: " << in_hostLine.pointABorder << std::endl;
				std::cout << "Point B: " << in_hostLine.pointBBorder << std::endl;
				std::cout << "Line B: " << std::endl;
				std::cout << "Point A: " << in_guestLine.pointABorder << std::endl;
				std::cout << "Point B: " << in_guestLine.pointBBorder << std::endl;
				*/
				if (in_hostLine.numberOfPoints == 2)	// one of these should be 2
				{
					returnLine.type = IntersectionType::PARTIAL_BOUND;		// TWIN always has exactly one point on a border line, and a non-bound point.
					returnLine.line.numberOfBorderLines = 1;
					returnLine.line.isPointAOnBorder = 1;
					returnLine.line.pointABorder = in_hostLine.pointABorder;
					returnLine.line.pointA = in_hostLine.pointA;
					returnLine.line.pointB = in_hostLine.pointB;
					returnLine.line.intersectedSecondaryID = in_hostLine.intersectedSecondaryID;
				}
				else if (in_guestLine.numberOfPoints == 2)
				{
					returnLine.type = IntersectionType::PARTIAL_BOUND;		// TWIN always has exactly one point on a border line, and a non-bound point.
					returnLine.line.numberOfBorderLines = 1;
					returnLine.line.isPointAOnBorder = 1;
					returnLine.line.pointABorder = in_hostLine.pointABorder;
					returnLine.line.pointA = in_guestLine.pointA;
					returnLine.line.pointB = in_guestLine.pointB;
					returnLine.line.intersectedSecondaryID = in_guestLine.intersectedSecondaryID;
				}
				else if
					(
					(in_guestLine.numberOfPoints == 1)
						&&
						(in_hostLine.numberOfPoints == 1)
						)
				{
					std::cout << "CONDITION 1.4, entered 1/1 branch. " << std::endl;

					std::cout << "Host line, isPointAOnBorder: " << in_hostLine.isPointAOnBorder << std::endl;
					std::cout << "Host line, point A border: " << in_hostLine.pointABorder << std::endl;

					std::cout << "Host line, point A: " << in_hostLine.pointA.x << ", " << in_hostLine.pointA.y << ", " << in_hostLine.pointA.z << std::endl;
					std::cout << "Guest line, point A: " << in_guestLine.pointA.x << ", " << in_guestLine.pointA.y << ", " << in_guestLine.pointA.z << std::endl;

					returnLine.type = IntersectionType::PARTIAL_BOUND;
					returnLine.line.numberOfBorderLines = 1;
					returnLine.line.isPointAOnBorder = 1;
					returnLine.line.pointABorder = in_hostLine.pointABorder;
					returnLine.line.pointA = in_hostLine.pointA;
					returnLine.line.pointB = in_guestLine.pointA;

				}
				//glm::vec3 newSecondPoint = findSecondPointForLine(in_hostLine.pointA, in_guestLine.pointA, in_guestLine.pointB);
				//returnLine.convertLineToPartialBound(in_hostLine, in_guestLine);		// convert to TWIN
				//returnLine.convertLineToPartialBound(in_hostLine, in_guestLine, newSecondPoint);
				//std::cout << "CASE 3.2: " << std::endl;
				//std::cout << "!!! Return line border is: " << returnLine.line.pointABorder << std::endl;

			}

			// CASE 1.5 PARTIAL_BOUND -- condition 2; requires at least 2 points to work.
			else if
				(
				(in_hostLine.numberOfBorderLines == 1)		// A hit's B, but it's only one border line
					&&
					(in_guestLine.numberOfBorderLines == 0)		// B "engulfs" A, but B has no border lines going through A
					&&
					(totalNumberOfPoints == 2)
					)
			{
				std::cout << "CASE 1.5 PARTIAL_BOUND hit " << std::endl;

				returnLine.type = IntersectionType::PARTIAL_BOUND;
				returnLine.line.numberOfBorderLines = 1;
				/*
				if (in_hostLine.isPointAOnBorder == 1)
				{
					returnLine.line.isPointAOnBorder = 1;
				}
				else if (in_hostLine.isPointBOnBorder == 1)
				{
					returnLine.line.isPointBOnBorder = 1;
				}
				*/

				IntersectionLine newLine;

				// ******One of two cases is possible; in both cases, The point A of the host line
				//             would have to be on a border.
				// CASE 1.5.1: either both the host and guest lines have 1 intersection point,
				//                -OR-
				// CASE 1.5.2: the host line has two points, even though it only has 1 border line. The point A of the host line
				//             would have to be on a border.
				//


				// CASE 1.5.1: >>>>
				if ((in_guestLine.numberOfPoints == 1) && (in_hostLine.numberOfPoints == 1))
				{


					//std::cout << "1.5.1, newLine case 1 triggered..." << std::endl;
					//std::cout << "Guest line border line count: " << in_guestLine.numberOfBorderLines << std::endl;
					//std::cout << "Host line border line count: " << in_hostLine.numberOfBorderLines << std::endl;

					newLine = in_hostLine;

					// remember, in this case, the host line is on a border, so we must store the border line data from that;
					// (this should always come from point A, when there is one border line)
					newLine.isPointAOnBorder = 1;
					newLine.pointABorder = in_hostLine.pointABorder;
					newLine.pointB = in_guestLine.pointA;

					newLine.numberOfPoints = 2;
				}
				/*
				else if (in_guestLine.numberOfPoints == 2)
				{
					std::cout << "3.3, newLine case 2 triggered..." << std::endl;
					newLine = in_guestLine;
				}
				*/

				// CASE 1.5.2: >>>>
				else if (in_hostLine.numberOfPoints == 2)
				{
					//std::cout << "1.5.2, newLine case2 triggered..." << std::endl;
					newLine = in_hostLine;
				}

				returnLine.line = newLine;

				//returnLine.line.pointA = in_hostLine.pointA;
				//returnLine.line.pointB = in_hostLine.pointB;
				//returnLine.line.intersectedSecondaryID = in_hostLine.intersectedSecondaryID;

				//std::cout << "CASE 1.5: A hits B with one border line, but B doesn't hit A with any border lines" << std::endl;
				//std::cout << "(A) Number of points: " << in_hostLine.numberOfPoints << std::endl;
				//std::cout << "(A) Host Line Points are: " << in_hostLine.pointA.x << ", " << in_hostLine.pointA.y << ", " << in_hostLine.pointA.z << " | " << in_hostLine.pointB.x << ", " << in_hostLine.pointB.y << ", " << in_hostLine.pointB.z << std::endl;
				//std::cout << "(A) Guest Line Points are: " << in_guestLine.pointA.x << ", " << in_guestLine.pointA.y << ", " << in_guestLine.pointA.z << " | " << in_guestLine.pointB.x << ", " << in_guestLine.pointB.y << ", " << in_guestLine.pointB.z << std::endl;


				//int someVal = 3;
				//std::cin >> someVal;
			}

			// CASE 1.6: A has one line, B has one line; this is a TWIN (partial bound?)
			else if
				(
				(in_hostLine.numberOfPoints == 1)	// check the number of point intercepts (not lines)
					&&
					(in_hostLine.numberOfBorderLines == 1)

					&&

					(in_guestLine.numberOfPoints == 1)	// " "
					&&
					(in_guestLine.numberOfBorderLines == 1)
					)
			{
				std::cout << "CASE 1.6: twin-style PARTIAL_BOUND detected. " << std::endl;
				glm::vec3 newSecondPoint = findSecondPointForLine(in_hostLine.pointA, in_guestLine.pointA, in_guestLine.pointB);
				//returnLine.convertLineToPartialBound(in_hostLine, in_guestLine);		// convert to TWIN
				returnLine.convertLineToPartialBound(in_hostLine, in_guestLine, newSecondPoint);
			}

			// CASE 1.7: NON-BOUND line is formed

			else if
				(
				(in_hostLine.numberOfPoints == 1)
					&&
					(in_hostLine.numberOfBorderLines == 0)

					&&

					(in_guestLine.numberOfPoints == 1)
					&&
					(in_guestLine.numberOfBorderLines == 0)
					)
			{
				std::cout << "CASE 1.7: NON-BOUND case 2 hit " << std::endl;
				glm::vec3 roundedA = in_hostLine.pointA;
				glm::vec3 roundedB = in_guestLine.pointA;
				if (checkIfPointsMatch(roundedA, roundedB) == 0)		// it can only be a valid line if the two points that make up the line do not match
				{
					returnLine.convertLinesToNonbound(in_hostLine, in_guestLine);
				}
				else
				{
					returnLine.type = IntersectionType::NONE;	// it's invalid, so set to 1
				}
			}

			// CASE 1.8: there is no intercept between them.
			else if
				(
				(in_hostLine.numberOfPoints == 0)
					&&
					(in_guestLine.numberOfPoints == 0)
					)
			{
				// do nothing here; default value of CategorizedLine.type is IntersectionType::NONE
				std::cout << "CASE 1.8: No intercept detected. " << std::endl;
				//int someVal = 3;
				//std::cin >> someVal;
			}
			returnLine.line.lineGroupID = in_groupID;
			returnLine.emptyNormal = in_polyBEmptyNormal;
			//std::cout << "!!!-> GroupID is: " << in_groupID << std::endl;
		}
		// ROOT CASE 2: At least one line is considered INVALID. (OR)
		else if
			(
			(in_hostLine.lineValidity == IntersectionLineValidity::INVALID)
				//||		
				//(in_guestLine.lineValidity == IntersectionLineValidity::INVALID)
				)
		{
			if (in_hostLine.lineValidity == IntersectionLineValidity::INVALID)
			{
				//std::cout << "~~~~ The host line is INVALID. " << std::endl;
			}
			if (in_guestLine.lineValidity == IntersectionLineValidity::INVALID)
			{
				//std::cout << "~~~~ The guest line is INVALID. " << std::endl;
			}

			std::cout << "!!! Handling special case, where host line is INVALID: " << std::endl;

			/*
			std::cout << "!!! Handling special case, where at least one line is INVALID: " << std::endl;

			std::cout << "++++++Host line stats: " << std::endl;
			std::cout << "Number of points: " << in_hostLine.numberOfPoints << std::endl;
			std::cout << "Number of border lines: " << in_hostLine.numberOfBorderLines << std::endl;

			std::cout << "++++++Guest line stats: " << std::endl;
			std::cout << "Number of points: " << in_guestLine.numberOfPoints << std::endl;
			std::cout << "Number of border lines: " << in_guestLine.numberOfBorderLines << std::endl;

			std::cout << "line A, point A: " << in_hostLine.pointA.x << ", " << in_hostLine.pointA.y << ", " << in_hostLine.pointA.z << std::endl;
			std::cout << "line A, point B: " << in_hostLine.pointB.x << ", " << in_hostLine.pointB.y << ", " << in_hostLine.pointB.z << std::endl;
			std::cout << "line A, point A border: is on border? ->" << in_hostLine.isPointAOnBorder << "; " << in_hostLine.pointABorder << std::endl;
			std::cout << "line A, point B border: is on border? ->" << in_hostLine.isPointBOnBorder << "; " << in_hostLine.pointBBorder << std::endl;
			std::cout << "line A, number of border lines: " << in_hostLine.numberOfBorderLines << std::endl;


			std::cout << "line B, point A: " << in_guestLine.pointA.x << ", " << in_guestLine.pointA.y << ", " << in_guestLine.pointA.z << std::endl;
			std::cout << "line B, point B: " << in_guestLine.pointB.x << ", " << in_guestLine.pointB.y << ", " << in_guestLine.pointB.z << std::endl;
			std::cout << "line B, point A border: is on border? ->" << in_guestLine.isPointAOnBorder << "; " << in_guestLine.pointABorder << std::endl;
			std::cout << "line B, point B border: is on border? ->" << in_guestLine.isPointBOnBorder << "; " << in_guestLine.pointBBorder << std::endl;
			std::cout << "line A, number of border lines: " << in_guestLine.numberOfBorderLines << std::endl;
			*/

			if (in_guestLine.numberOfPoints == 2)	// can only perform this special case if the guest line has 2 points in it.
			{
				returnLine.convertLinesToInterceptsPointPrecise(in_hostLine, in_guestLine);
				returnLine.line.lineGroupID = in_groupID;
				returnLine.emptyNormal = in_polyBEmptyNormal;
			}
			//}

			//std::cout << "Special case halt; " << std::endl;
			//int someVal = 3;
			//std::cin >> someVal;
		}
	}
	return returnLine;
}

bool MassZoneBoxBoundarySPolySet::checkForSamePointCondition(IntersectionLine in_lineA, IntersectionLine in_lineB)
{
	//std::cout << ">>>>> checking for same point condition..." << std::endl;
	std::cout << "line A -> point A: " << in_lineA.pointA.x << ", " << in_lineA.pointA.y << ", " << in_lineA.pointA.z << std::endl;
	std::cout << "line A -> point B: " << in_lineA.pointB.x << ", " << in_lineA.pointB.y << ", " << in_lineA.pointB.z << std::endl;
	std::cout << "line B -> point A: " << in_lineB.pointA.x << ", " << in_lineB.pointA.y << ", " << in_lineB.pointA.z << std::endl;
	std::cout << "line B -> point B: " << in_lineB.pointB.x << ", " << in_lineB.pointB.y << ", " << in_lineB.pointB.z << std::endl;

	bool wasDetected = false;


	if
		(
		(in_lineA.pointA == in_lineA.pointB)		// the line's point's equal each other
			&&
			(in_lineB.pointA == in_lineB.pointB)		// ...
			&&
			(in_lineA.pointA == in_lineB.pointA)		// and all the points are the same.

			)
	{
		//std::cout << "############################################### Notice: checkForSameCondition was found as true; no intersection exists. " << std::endl;
		//int stupidVal = 3;
		//std::cin >> stupidVal;
		wasDetected = true;
	}



	return wasDetected;
}

glm::vec3 MassZoneBoxBoundarySPolySet::findSecondPointForLine(glm::vec3 in_beginPoint, glm::vec3 in_candidate1, glm::vec3 in_candidate2)
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

int MassZoneBoxBoundarySPolySet::checkIfPointsMatch(glm::vec3 in_pointA, glm::vec3 in_pointB)
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

IntersectionResult MassZoneBoxBoundarySPolySet::checkIfLineIntersectsTriangle(STriangle in_triangle, STriangleLine in_line)
{
	IntersectionResult returnResult;

	// do typical compare:
	IntersectionResult resultA = checkIfRayIntersectsTriangle(in_triangle, in_line);

	// do a compare where the in_line is swapped
	STriangleLine swappedLine = in_line;
	swappedLine.pointA = in_line.pointB;
	swappedLine.pointB = in_line.pointA;

	// do the swapped compare
	IntersectionResult resultB = checkIfRayIntersectsTriangle(in_triangle, swappedLine);

	std::cout << "!!!! Bi-dirrectional comparison results: " << std::endl;
	std::cout << "ResultA, was found: " << resultA.wasIntersectFound << std::endl;
	std::cout << "ResultB, was found: " << resultB.wasIntersectFound << std::endl;

	if
		(
		(resultA.wasIntersectFound != 0)
			&&
			(resultB.wasIntersectFound != 0)
			)
	{
		returnResult = resultA;
	}
	//
	//else
	//{
	//}
	return returnResult;
}

IntersectionResult MassZoneBoxBoundarySPolySet::checkIfRayIntersectsTriangle(STriangle in_triangle, STriangleLine in_triangleLine)
{
	//int someVal = 0;
	IntersectionResult returnResult;
	glm::vec3 intersect_candidate;

	std::cout << "triangle, point 0: " << in_triangle.triangleLines[0].pointA.x << ", " << in_triangle.triangleLines[0].pointA.y << ", " << in_triangle.triangleLines[0].pointA.z << std::endl;
	std::cout << "triangle, point 1: " << in_triangle.triangleLines[1].pointA.x << ", " << in_triangle.triangleLines[1].pointA.y << ", " << in_triangle.triangleLines[1].pointA.z << std::endl;
	std::cout << "triangle, point 2: " << in_triangle.triangleLines[2].pointA.x << ", " << in_triangle.triangleLines[2].pointA.y << ", " << in_triangle.triangleLines[2].pointA.z << std::endl;

	std::cout << "Checking if this line intersects: pointA: " << in_triangleLine.pointA.x << ", " << in_triangleLine.pointA.y << ", " << in_triangleLine.pointA.z << " | pointB: " << in_triangleLine.pointB.x << ", " << in_triangleLine.pointB.y << ", " << in_triangleLine.pointB.z << std::endl;
	std::cout << "=========" << std::endl;

	glm::vec3 point0 = in_triangle.triangleLines[0].pointA;
	glm::vec3 point1 = in_triangle.triangleLines[1].pointA;
	glm::vec3 point2 = in_triangle.triangleLines[2].pointA;


	int matchCount = 0;

	// attempt match of point A of the triangleLine to all 3 points:
	for (int x = 0; x < 3; x++)
	{
		if (in_triangleLine.pointA == in_triangle.triangleLines[x].pointA)
		{
			matchCount++;
		}
	}

	// ...of point B
	for (int x = 0; x < 3; x++)
	{
		if (in_triangleLine.pointB == in_triangle.triangleLines[x].pointA)
		{
			matchCount++;
		}
	}

	//std::cout << "##### Matchcount is: " << matchCount << std::endl;

	// condition one, matchCount is 2
	if (matchCount == 2)
	{
		//std::cout << "~~~~~~~~~~~~~~~~ note special case; the triangleLine matches a line exactly in the incoming triangle. flagging as 2." << std::endl;
		//int someVal = 3;
		//std::cin >> someVal;
		returnResult.setResult(3);		// set flag indicatiing that the passed in STriangleLine is equal to a STriangleLine that belongs to the passed in STriangle.
	}

	// condition two, matchCount is 1
	else if (matchCount == 1)
	{
		//std::cout << "!! Match count 1 hit.  " << std::endl;

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
	else if (matchCount == 0)
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


		glm::vec3 dir = in_triangleLine.pointB - in_triangleLine.pointA;
		glm::vec3 w0 = in_triangleLine.pointA - point0;

		//std::cout << "Normal: " << N.x << ", " << N.y << ", " << N.z << std::endl;
		//std::cout << "Dir: " << dir.x << ", " << dir.y << ", " << dir.z << std::endl;

		float a = -dot(N, w0);
		float b = dot(N, dir);
		double doubleb = doubledot(N, dir);


		//std::cout <<std::setprecision(9);
		//std::cout << "(a) is: " << a << std::endl;
		//std::cout << "(b) is: " << b << std::endl;
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
				std::cout << "::> Line is lies within triangle. " << std::endl;
				returnResult.setResult(2);
			}
			//else return 0;              // ray disjoint from plane
			else returnResult.setResult(0);
		}

		float r = a / b;
		if (r < 0.0)
		{
			//std::cout << "!! Note: r is  less than 0. " << std::endl;
			//return 0;
			returnResult.setResult(0);
		}

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
		s = float(floor(s * 1000 + 0.5) / 1000);

		std::cout << "--> Value of s: " << s << std::endl;
		if (s < 0.0 || s > 1.0)         // I is outside S
			//return 0;
		{
			//std::cout << "!! Note: I is outside S. " << std::endl;
			returnResult.setResult(0);
		}
		t = (uv * wu - uu * wv) / D;
		t = float(floor(t * 1000 + 0.5) / 1000);

		std::cout << "--> Value of t: " << t << std::endl;
		if (t < 0.0 || (s + t) > 1.0)  // I is outside T
			//return 0;
		{
			std::cout << "!! Note: I is outside T. " << std::endl;
			returnResult.setResult(0);
		}
		//return 1;
		returnResult.setResult(1);
		returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
		returnResult.borderLineID = in_triangleLine.borderLineID;
		returnResult.intersectedPoint = roundPointToHundredths(intersect_candidate);					// the intercept point should be rounded to hundredths!



		//std::cout << "Resulting intersect attempt value: " << returnResult.wasIntersectFound << std::endl;
		//std::cout << "### Intersected point for targeted debug point is: " << returnResult.intersectedPoint.x << ", " << returnResult.intersectedPoint.y << ", " << returnResult.intersectedPoint.z << std::endl;

		//if (returnResult.wasIntersectFound != 0)
		//{
			//std::cout << "### Intersected point for targeted debug point is: " << returnResult.intersectedPoint.x << ", " << returnResult.intersectedPoint.y << ", " << returnResult.intersectedPoint.z << std::endl;
		//}

		//if (tempDebug == 1)
		//{
			//std::cout << "### Intersected point for targeted debug point is: " << returnResult.intersectedPoint.x << ", " << returnResult.intersectedPoint.y << ", " << returnResult.intersectedPoint.z << std::endl;
			//int someVal = 3;
			//std::cin >> someVal;
		//}
	}
	return returnResult;
}

glm::vec3 MassZoneBoxBoundarySPolySet::roundPointToHundredths(glm::vec3 in_point)
{
	glm::vec3 returnPoint;
	returnPoint.x = float(floor(in_point.x * 100 + 0.5) / 100);
	returnPoint.y = float(floor(in_point.y * 100 + 0.5) / 100);
	returnPoint.z = float(floor(in_point.z * 100 + 0.5) / 100);
	return returnPoint;
}

double MassZoneBoxBoundarySPolySet::doubledot(glm::vec3 in_A, glm::vec3 in_B)
{
	double x_multiplied = in_A.x * in_B.x;
	double y_multiplied = in_A.y * in_B.y;
	double z_multiplied = in_A.z * in_B.z;
	return x_multiplied + y_multiplied + z_multiplied;
}