#include "stdafx.h"
#include "SPolySet.h"

//#define SMALL_NUM   0.00000001 // anything that avoids division overflow
#define SMALL_NUM   0.000001 // anything that avoids division overflow

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
			produceCategorizedLinesForHostPoly(polyA, x, polyB, currentIndex);

		}
	}
	*/

	//std::cout << "########### Poly comparison, checking line 2 of second STriangle in second SPoly: " << std::endl;
	//std::cout << secondaryPolys[1].triangles[1].triangleLines[2].pointA.x << ", " << secondaryPolys[1].triangles[1].triangleLines[2].pointA.y << ", " << secondaryPolys[1].triangles[1].triangleLines[2].pointA.z << std::endl;
	//std::cout << secondaryPolys[1].triangles[1].triangleLines[2].pointB.x << ", " << secondaryPolys[1].triangles[1].triangleLines[2].pointB.y << ", " << secondaryPolys[1].triangles[1].triangleLines[2].pointB.z << std::endl;

	//int someval = 3;
	//std::cin >> someval;

	int generationCounter = 0;

	int compCount2 = numberOfPolys;
	for (int x = 0; x < compCount2; x++)
	{
		int currentIndex = 0;						// index begins at 0, increments every tick of the y loop that follows.
		for (int y = 0; y < numberOfPolys; y++)
		{
			if (!(x == y))	// never compare the poly to itself
			{
				SPoly* polyA = &secondaryPolys[x];
				SPoly* polyB = &secondaryPolys[currentIndex];

				if (polyA->groupID != polyB->groupID)		// don't compare polys in the same group to each other
				{

					// determine if we need to do the special case, where polyA and polyB are coplanar.
					CoplanarChecker checker(polyA, polyB);

					// if they are coplanar, check polyA to see if it's group 1. If it is, set into the coplanar comparsion list.
					if (checker.coplanarityDetected == true)	
					{
						// if they are coplanar AND polyA is a group 1 poly, do this
						if (polyA->groupID == 1)	
						{
							std::cout << "!!!! Inserting record into coplanar relationships... " << std::endl;
							coplanarTracker.insertCoplanarRelationship(x, polyA, currentIndex, polyB);
						}
					}
					// otherwise, if they are not coplanar, do the following.
					else
					{
					



							//std::cout << "############## Comparing poly ID " << x << " to poly ID " << currentIndex << std::endl;
							//std::cout << "!!! Number of polys is: " << numberOfPolys << std::endl;

							//std::cout << "##### Current index is: " << currentIndex << std::endl;
							//std::cout << "########### Poly comparison, checking line 2 of first STriangle in SPoly: (Loop)" << std::endl;
							//std::cout << secondaryPolys[1].triangles[0].triangleLines[2].pointA.x << ", " << secondaryPolys[1].triangles[0].triangleLines[2].pointA.y << ", " << secondaryPolys[1].triangles[0].triangleLines[2].pointA.z << std::endl;
							//std::cout << secondaryPolys[1].triangles[0].triangleLines[2].pointB.x << ", " << secondaryPolys[1].triangles[0].triangleLines[2].pointB.y << ", " << secondaryPolys[1].triangles[0].triangleLines[2].pointB.z << std::endl;
							std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||| --> Generating next set of categorized lines... (" << generationCounter++ << ") " << std::endl;
							std::cout << "|||||||||| Comparing 'host' spoly with index " << x << " to 'guest' poly with index " << currentIndex << std::endl;
							produceCategorizedLinesForHostPoly(polyA, x, polyB, currentIndex);		// PHASE 1
							std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||| Passed 1 set of categorized lines. " << std::endl;
					}
				}
			}
			currentIndex++;
		}

		std::cout << "|||| Finished for this SPoly" << std::endl;

		// when the SPoly at x has been compared to all other SPolys, we should check for any coplanar relationships for x.
		// it's completely possible for a SPoly to have categorized lines from a coplanar relationship AND a non-coplanar replationship.
		coplanarTracker.buildCategorizedLinesForCoplanarRelationship(x);

		std::cout << "|||| Finished coplanar check..." << std::endl;

		secondaryPolys[x].sequenceFactory.printLinesInPool();

		std::cout << "|||| Finished print lines in pool..." << std::endl;

		secondaryPolys[x].buildCleaveSequences();
		//secondaryPolys[x].printAllCleaveLines();
		std::cout << "+++++ Enter number to go to next poly. " << std::endl;
		int someVal = 3;
		std::cin >> someVal;
	}

	//std::cout << "########### Poly comparison, checking line 2 of first STriangle in SPoly: (2) " << std::endl;
	//std::cout << secondaryPolys[1].triangles[0].triangleLines[2].pointA.x << ", " << secondaryPolys[1].triangles[0].triangleLines[2].pointA.y << ", " << secondaryPolys[1].triangles[0].triangleLines[2].pointA.z << std::endl;
	//std::cout << secondaryPolys[1].triangles[0].triangleLines[2].pointB.x << ", " << secondaryPolys[1].triangles[0].triangleLines[2].pointB.y << ", " << secondaryPolys[1].triangles[0].triangleLines[2].pointB.z << std::endl;

	//someval = 3;
	//std::cin >> someval;
}

int SPolySet::produceCategorizedLinesForHostPoly(SPoly* in_hostPolyPtr, int in_hostPolyAID, SPoly* in_guestPolyPtr, int in_guestPolyID)
{
	int numberOfIntersections = 0;

	int hostPolyTriangleCount = in_hostPolyPtr->numberOfTriangles;
	int guestPolyTriangleCount = in_guestPolyPtr->numberOfTriangles;

	//std::cout << "polyA count: " << hostPolyTriangleCount << std::endl;
	//std::cout << "polyB count: " << guestPolyTriangleCount << std::endl;

	std::cout << "++++++ :::: host poly ID: " << in_hostPolyAID << std::endl;
	std::cout << "++++++ :::: guest poly ID: " << in_guestPolyID << std::endl;

	//std::cout << "########################## ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> A lines are: " << std::endl;
	//std::cout << "0: " << in_polyAPtr->triangles[0].triangleLines[0].pointA.x << ", " << in_polyAPtr->triangles[0].triangleLines[0].pointA.y << ", " << in_polyAPtr->triangles[0].triangleLines[0].pointA.z << " ||| " << in_polyAPtr->triangles[0].triangleLines[0].pointB.x << ", " << in_polyAPtr->triangles[0].triangleLines[0].pointB.y << ", " << in_polyAPtr->triangles[0].triangleLines[0].pointB.z << std::endl;
	//std::cout << "1: " << in_polyAPtr->triangles[0].triangleLines[1].pointA.x << ", " << in_polyAPtr->triangles[0].triangleLines[1].pointA.y << ", " << in_polyAPtr->triangles[0].triangleLines[1].pointA.z << " ||| " << in_polyAPtr->triangles[0].triangleLines[1].pointB.x << ", " << in_polyAPtr->triangles[0].triangleLines[1].pointB.y << ", " << in_polyAPtr->triangles[0].triangleLines[1].pointB.z << std::endl;
	//std::cout << "2: " << in_polyAPtr->triangles[0].triangleLines[2].pointA.x << ", " << in_polyAPtr->triangles[0].triangleLines[2].pointA.y << ", " << in_polyAPtr->triangles[0].triangleLines[2].pointA.z << " ||| " << in_polyAPtr->triangles[0].triangleLines[2].pointB.x << ", " << in_polyAPtr->triangles[0].triangleLines[2].pointB.y << ", " << in_polyAPtr->triangles[0].triangleLines[2].pointB.z << std::endl;

	STriangle* tempBTrianglePtr = &in_guestPolyPtr->triangles[1];
	//std::cout << "2, point A: " << tempBTrianglePtr->triangleLines[2].pointA.x << ", " << tempBTrianglePtr->triangleLines[2].pointA.y << ", " << tempBTrianglePtr->triangleLines[2].pointA.z << std::endl;
	//std::cout << "2, point B: " << tempBTrianglePtr->triangleLines[2].pointB.x << ", " << tempBTrianglePtr->triangleLines[2].pointB.y << ", " << tempBTrianglePtr->triangleLines[2].pointB.z << std::endl;


	//int someVal = 3;
	//std::cout << "temp halt, enter integer. " << std::endl;
	//std::cin >> someVal;

	// perform a co-planar test here; if the two poly's arent' coplanar, proceed with normal operations.

	std::cout << "+++++++++ host poly Triangles: " << hostPolyTriangleCount << std::endl;
	for (int currentHostPolyTriangle = 0; currentHostPolyTriangle < hostPolyTriangleCount; currentHostPolyTriangle++)					// compare each of poly A's tertiaries...
	{

		std::cout << "::::::::::::::::::::::::::::::::::: ----------------------------------+++++++++>>>>>>>>>>> Running host poly Triangle comparison: " << std::endl;

		STriangle* hostTrianglePtr = &in_hostPolyPtr->triangles[currentHostPolyTriangle];	// " " 
		IntersectionLineGroup hostLineGroup;						// the line group for poly A.
		IntersectionLineGroup guestLineGroup;						// the line group for poly B.
		for (int y = 0; y < guestPolyTriangleCount; y++)					// .. to each of poly B's tertiaries...
		{
			// for each pair (that is, A's current STriangle to B's current STriangle in the iterations), we must:
			// 1. compare the lines of STriangle A to that of STriangle B
			// 2. compare the lines of STriangle B to that of STriangle A
			// 3. analyze the relationship between them


			// >>>>>>>>>>>>>>>>>>>>> STEP 1
			// compare the host triangle lines, to the guest triangles.

			STriangle* guestTrianglePtr = &in_guestPolyPtr->triangles[y]; // get the guest poly's triangle
			std::cout << "::::::::::::::::::::::::::::::::::: >>>>>>>>>>>>>>>>>>>>>>>>>>> Comparing lines of A to triangle of B " << std::endl;
			//std::cout << ">>> B triangle (Guest triangle) points are: " << std::endl;
			//std::cout << "0: " << guestTrianglePtr->triangleLines[0].pointA.x << ", " << guestTrianglePtr->triangleLines[0].pointA.y << ", " << guestTrianglePtr->triangleLines[0].pointA.z << std::endl;
			//std::cout << "1: " << guestTrianglePtr->triangleLines[1].pointA.x << ", " << guestTrianglePtr->triangleLines[1].pointA.y << ", " << guestTrianglePtr->triangleLines[1].pointA.z << std::endl;
			//std::cout << "2: " << guestTrianglePtr->triangleLines[2].pointA.x << ", " << guestTrianglePtr->triangleLines[2].pointA.y << ", " << guestTrianglePtr->triangleLines[2].pointA.z << std::endl;
			//std::cout << "2 (B): " << guestTrianglePtr->triangleLines[2].pointB.x << ", " << guestTrianglePtr->triangleLines[2].pointB.y << ", " << guestTrianglePtr->triangleLines[2].pointB.z << std::endl;

			for (int currentHostTriangleLine = 0; currentHostTriangleLine < 3; currentHostTriangleLine++)		// run the lines of A (the host) through triangle B (the guest)
			{
				// check if any of the host poly's lines intersected the guest poly...
				// if they did, we need to add them to the IntersectionLine
				IntersectionResult intersectResult = checkIfLineIntersectsTriangle(*guestTrianglePtr, in_hostPolyPtr->triangles[currentHostPolyTriangle].triangleLines[currentHostTriangleLine]);		
				

				IntersectionLine potentialHostLine;		// the line that will store the intersections.
				if (intersectResult.wasIntersectFound == 1)
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

				// for when the intersection line is found as being parallel to the the triangle it attempted to intersect.
				if (intersectResult.wasIntersectFound == 2)
				{
					std::cout << "!! ++++ NOTICE: ray lies in triangle plane! (host lines compared to guest triangle)" << std::endl;
				}
				//std::cout << "Current number of border lines is: " << potentialLineAtoB.numberOfBorderLines << std::endl;
			}
			// merge the found intersections for the candidate line, then make that result = potentialLineAtoB;
			IntersectionLine mergedHostLine = hostLineGroup.mergeLines();

			// >>>>>>>>>>>>>>>>>>>>> STEP 2
			// compare the GUEST triangle lines, to the host triangles.

			std::cout << "::::::::::::::::::::::::::::::::::: >>>>>>>>>>>>>>>>>>>>>>>>>>> Comparing lines of the guest to the host triangle" << std::endl;
			for (int z = 0; z < 3; z++)		// run the lines of B (the guest) through triangle A (the host)
			{
				IntersectionResult intersectResult = checkIfLineIntersectsTriangle(*hostTrianglePtr, guestTrianglePtr->triangleLines[z]);				// check if poly B's line intersected A...if they did, we need to add them to the IntersectionLine

				IntersectionLine potentialGuestLine;		// the line that will store the intersections.
				if (intersectResult.wasIntersectFound == 1)
				{
					//std::cout << "Line " << z << " intersects " << std::endl;
					if (intersectResult.wasIntersectOnBorderLine == 1)
					{
						//std::cout << "(Guest lines going through Host Triangle) Intersecting line was a border line. It's ID is: " << intersectResult.borderLineID << std::endl;
						//std::cout << "(Guest lines going through Host Triangle) The intersecting point is: " << intersectResult.intersectedPoint.x << ", " << intersectResult.intersectedPoint.y << ", " << intersectResult.intersectedPoint.z << std::endl;
						//std::cout << "(Guest lines going through Host Triangle) Line points: " << guestTrianglePtr->triangleLines[z].pointA.x << ", " << guestTrianglePtr->triangleLines[z].pointA.y << ", " << guestTrianglePtr->triangleLines[z].pointA.z << " || "
						//	<< guestTrianglePtr->triangleLines[z].pointB.x << ", " << guestTrianglePtr->triangleLines[z].pointB.y << ", " << guestTrianglePtr->triangleLines[z].pointB.z << std::endl;
						//potentialLineBtoA.numberOfBorderLines = 1;
					}
					else
					{
						//std::cout << "(Guest lines going through Host Triangle) Intersecting line was NOT a border line. (B to A)" << std::endl;
						//std::cout << "(Guest lines going through Host Triangle) The intersecting point is: " << intersectResult.intersectedPoint.x << ", " << intersectResult.intersectedPoint.y << ", " << intersectResult.intersectedPoint.z << std::endl;
						//std::cout << "(Guest lines going through Host Triangle) Line points: " << guestTrianglePtr->triangleLines[z].pointA.x << ", " << guestTrianglePtr->triangleLines[z].pointA.y << ", " << guestTrianglePtr->triangleLines[z].pointA.z << " || "
						//	<< guestTrianglePtr->triangleLines[z].pointB.x << ", " << guestTrianglePtr->triangleLines[z].pointB.y << ", " << guestTrianglePtr->triangleLines[z].pointB.z << std::endl;
						std::cout << "Intersecting line was NOT a border line. (B to A)" << std::endl;
					}
					potentialGuestLine.addIntersectionResult(intersectResult);		// add the result to the intersect line
					potentialGuestLine.intersectedSecondaryID = in_guestPolyID;			// store the ID of the secondary that was intersected; this should always be B
					guestLineGroup.addIntersectionLine(potentialGuestLine);			// only add a line to the group if the line intersection wtih poly B
				}
				// for when the intersection line is found as being parallel to the the triangle it attempted to intersect.
				if (intersectResult.wasIntersectFound == 2)
				{
					std::cout << "!! ++++ NOTICE: ray lies in triangle plane! (guest lines compared to host triangle)" << std::endl;
				}
				
			}
			// merge the found intersections for the candidate line, then make that result = potentialLineBtoA;
			IntersectionLine mergedGuestLine = guestLineGroup.mergeLines();

			// STEP 3
			// compare the IntersectionLines to determine the type of interect (if any) that was generated;
			// it is important to remember that, the "view" or "context" of the categorized line is at is viewed from the host triangle.
			CategorizedLine currentCategorizedLine = determineCategorizedLineThroughHostTriangleContext(mergedHostLine, mergedGuestLine, in_guestPolyPtr->groupID, in_guestPolyPtr->polyEmptyNormal);	// find out what type of line this is; assign the appropriate groupID to the line

			if (currentCategorizedLine.type == IntersectionType::NONE)	// only add the line to polygon A's map if it was a valid intersection.
			{
				//std::cout << "!!! Warning, line detected as NONE " << std::endl;
			}

			// STEP 4
			// add any CategorizedLine to polygonA's map that isn't NONE
			if (currentCategorizedLine.type != IntersectionType::NONE)	// only add the line to polygon A's map if it was a valid intersection.
			{
				//in_polyAPtr->addCategorizedLine(currentCategorizedLine);	// add the new line
				currentCategorizedLine.parentPoly = in_guestPolyID;

				std::cout << "___________________ Adding categorized line from this iteration: " << std::endl;
				std::cout << "Point A: " << currentCategorizedLine.line.pointA.x << ", " << currentCategorizedLine.line.pointA.y << ", " << currentCategorizedLine.line.pointA.z << std::endl;
				std::cout << "Point B: " << currentCategorizedLine.line.pointB.x << ", " << currentCategorizedLine.line.pointB.y << ", " << currentCategorizedLine.line.pointB.z << std::endl;
				int contVal = 3;
				std::cin >> contVal;

				in_hostPolyPtr->sequenceFactory.addCategorizedLine(currentCategorizedLine);
				// new code for adding to LineSequenceFactory goes here
				numberOfIntersections++;
			}

			hostLineGroup.reset();
			guestLineGroup.reset();

			//std::cout << "+++++++++ comparing next tertiary..." << std::endl;
		}

		//std::cout << "------------++ done comparing current tertiary in triangle A; moving to next tertiary in triangle A..." << std::endl;

	}
	//std::cout << "Current poly cleave size: " << in_polyAPtr->cleaveMap.size() << std::endl;
	//in_polyAPtr->printLines();
	//in_polyAPtr->moveLastCleave();		// move any remaining lines into a cleave, if they exist
	//std::cout << "Current poly cleave size: " << in_polyAPtr->cleaveMap.size() << std::endl;
	//in_polyAPtr->printAllCleaveLines();

	//std::cout << "########################## End intersection check. " << std::endl;
	
	return numberOfIntersections;
}


IntersectionResult SPolySet::checkIfLineIntersectsTriangle(STriangle in_triangle, STriangleLine in_triangleLine)
{
	//int someVal = 0;
	IntersectionResult returnResult;
	glm::vec3 intersect_candidate;

	std::cout << "triangle, point 0: " << in_triangle.triangleLines[0].pointA.x << ", " << in_triangle.triangleLines[0].pointA.y << ", " << in_triangle.triangleLines[0].pointA.z << std::endl;
	std::cout << "triangle, point 1: " << in_triangle.triangleLines[1].pointA.x << ", " << in_triangle.triangleLines[1].pointA.y << ", " << in_triangle.triangleLines[1].pointA.z << std::endl;
	std::cout << "triangle, point 2: " << in_triangle.triangleLines[2].pointA.x << ", " << in_triangle.triangleLines[2].pointA.y << ", " << in_triangle.triangleLines[2].pointA.z << std::endl;

	std::cout << "Checking if this line intersects: pointA: " << in_triangleLine.pointA.x << ", " << in_triangleLine.pointA.y << ", " << in_triangleLine.pointA.z << " | pointB: " << in_triangleLine.pointB.x << ", " << in_triangleLine.pointB.y << ", " << in_triangleLine.pointB.z << std::endl;

	glm::vec3 point0 = in_triangle.triangleLines[0].pointA;
	glm::vec3 point1 = in_triangle.triangleLines[1].pointA;
	glm::vec3 point2 = in_triangle.triangleLines[2].pointA;

	/*
	if
	(
		(in_triangleLine.pointA == point0)
		||
		(in_triangleLine.pointA == point1)
		||
		(in_triangleLine.pointA == point2)
	)
	{

	}
	*/


	int tempDebug = 0;
	/*
	if
	(
		//(in_triangleLine.pointA.x == 4.0f)
		//&&
		//(in_triangleLine.pointA.y == 0.0f)
		//&&
		//(in_triangleLine.pointA.z == 0.0f)

		//&&

		//(in_triangleLine.pointB.x == 4.0f)
		//&&
		//(in_triangleLine.pointB.y == 4.0f)
		//&&
		//(in_triangleLine.pointB.z == 0.0f)
	)
	{
		std::cout << "######## checkIfLineIntersectsTriangle, targeted point found! ##################-----------------> " << std::endl;
		std::cout << "Points of triangle line are: " << std::endl;
		std::cout << "Point A: " << in_triangleLine.pointA.x << ", " << in_triangleLine.pointA.y << ", " << in_triangleLine.pointA.z << std::endl;
		std::cout << "Point B: " << in_triangleLine.pointB.x << ", " << in_triangleLine.pointB.y << ", " << in_triangleLine.pointB.z << std::endl;
		tempDebug = 1;
	}
	*/

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

	std::cout << "Normal: " << N.x << ", " << N.y << ", " << N.z << std::endl;
	std::cout << "Dir: " << dir.x << ", " << dir.y << ", " << dir.z << std::endl;

	float a = -dot(N, w0);
	float b = dot(N, dir);
	double doubleb = doubledot(N, dir);


	//std::cout <<std::setprecision(9);
	std::cout << "(a) is: " << a << std::endl;
	std::cout << "(b) is: " << b << std::endl;
	std::cout << "(doubleb) is: " << doubleb << std::endl;

	// remember, if SMALL_NUM is too sensitive (i.e., too precise in the IEEE float), there may be a "miss" on detecting when
	// the passed-in STriangleLine's two points exactly match two points in the STriangle. 
	if (fabs(b) < SMALL_NUM) {     // ray is  parallel to triangle plane

		std::cout << "!!! Logic for less than SMALL_NUM entered....enter number to continue. " << std::endl;
		int someValAwesome = 3;
		std::cin >> someValAwesome;

		//if (a == 0)                 // ray lies in triangle plane
		if (a < SMALL_NUM)
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



	std::cout << "Resulting intersect attempt value: " << returnResult.wasIntersectFound << std::endl;
	std::cout << "### Intersected point for targeted debug point is: " << returnResult.intersectedPoint.x << ", " << returnResult.intersectedPoint.y << ", " << returnResult.intersectedPoint.z << std::endl;

	if (tempDebug == 1)
	{
		std::cout << "### Intersected point for targeted debug point is: " << returnResult.intersectedPoint.x << ", " << returnResult.intersectedPoint.y << ", " << returnResult.intersectedPoint.z << std::endl;
		int someVal = 3;
		std::cin >> someVal;
	}

	return returnResult;
	//return someVal;
}

IntersectionResult SPolySet::checkIfLineIntersectsTriangleSpecial(STriangle in_triangle, STriangleLine in_triangleLine)
{
	//int someVal = 0;
	IntersectionResult returnResult;
	glm::vec3 intersect_candidate;

	//std::cout << "Checking if this line intersects: pointA: " << in_triangleLine.pointA.x << ", " << in_triangleLine.pointA.y << ", " << in_triangleLine.pointA.z << " | pointB: " << in_triangleLine.pointB.x << ", " << in_triangleLine.pointB.y << ", " << in_triangleLine.pointB.z << std::endl;

	glm::vec3 point0 = in_triangle.triangleLines[0].pointA;
	glm::vec3 point1 = in_triangle.triangleLines[1].pointA;
	glm::vec3 point2 = in_triangle.triangleLines[2].pointA;

	int tempDebug = 0;
	/*
	if
	(
		//(in_triangleLine.pointA.x == 4.0f)
		//&&
		//(in_triangleLine.pointA.y == 0.0f)
		//&&
		//(in_triangleLine.pointA.z == 0.0f)

		//&&

		//(in_triangleLine.pointB.x == 4.0f)
		//&&
		//(in_triangleLine.pointB.y == 4.0f)
		//&&
		//(in_triangleLine.pointB.z == 0.0f)
	)
	{
		std::cout << "######## checkIfLineIntersectsTriangle, targeted point found! ##################-----------------> " << std::endl;
		std::cout << "Points of triangle line are: " << std::endl;
		std::cout << "Point A: " << in_triangleLine.pointA.x << ", " << in_triangleLine.pointA.y << ", " << in_triangleLine.pointA.z << std::endl;
		std::cout << "Point B: " << in_triangleLine.pointB.x << ", " << in_triangleLine.pointB.y << ", " << in_triangleLine.pointB.z << std::endl;
		tempDebug = 1;
	}
	*/

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
	//std::cout << "Normal: " << N.x << ", " << N.y << ", " << N.z << std::endl;

	glm::vec3 dir = in_triangleLine.pointB - in_triangleLine.pointA;
	glm::vec3 w0 = in_triangleLine.pointA - point0;

	float a = -dot(N, w0);
	float b = dot(N, dir);
	if (fabs(b) < SMALL_NUM) {     // ray is  parallel to triangle plane

		std::cout << "!! Warning, B is less than SMALL_NUM. " << std::endl;
		if (a == 0)                 // ray lies in triangle plane
			//return 2;
			returnResult.setResult(2);
		//else return 0;              // ray disjoint from plane
		else returnResult.setResult(0);
	}

	std::cout << "A is: " << a << std::endl;
	std::cout << "B is: " << b << std::endl;

	float r = a / b;
	if (r < 0.0)
		//return 0;
		returnResult.setResult(0);

	std::cout << "R is: " << r << std::endl;
	std::cout << "Dir is: " << dir.x << ", " << dir.y << ", " << dir.z << std::endl;

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


	std::cout << "### Was intersect found result will be: " << returnResult.wasIntersectFound << std::endl;
	std::cout << "### Intersected point for targeted debug point is: " << returnResult.intersectedPoint.x << ", " << returnResult.intersectedPoint.y << ", " << returnResult.intersectedPoint.z << std::endl;
	int someVal = 3;
	std::cin >> someVal;
	

	return returnResult;
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

double SPolySet::doubledot(glm::vec3 in_A, glm::vec3 in_B)
{
	double returnVal;
	double x_multiplied = in_A.x * in_B.x;
	double y_multiplied = in_A.y * in_B.y;
	double z_multiplied = in_A.z * in_B.z;
	return x_multiplied + y_multiplied + z_multiplied;
}

CategorizedLine SPolySet::determineCategorizedLineThroughHostTriangleContext(IntersectionLine in_hostLine, IntersectionLine in_guestLine, int in_groupID, glm::vec3 in_polyBEmptyNormal)
{
	CategorizedLine returnLine;
	// Remember, in_hostLine is the IntersectionLine belonging to the polygon we are adding to (the result of tracing A's line's through the triangle of B).
	// the CategorizedLine type returned should be calculated from polygon A's "view"; meaning, for example, that if A (which is what we are adding to) slices B completely, and A doesn't have any of it's border lines touched in 
	// the process, the line is NON_BOUND (meaning the categorized line exists only in the area of A, and not any border lines)

	//std::cout << ">>>>>>>>>>>>> Calling determine categorized line..." << std::endl;

	// ROOT CASE 1: Both lines are considered valid.
	if
	(
		(in_hostLine.lineValidity == IntersectionLineValidity::VALID)
		//&&
		//(in_guestLine.lineValidity == IntersectionLineValidity::VALID)
	)
	{

		// SPECIAL CASE: the guest line is invalid, but the host is not. The host line is a partial bound.
		if
		(
			(in_guestLine.lineValidity == IntersectionLineValidity::INVALID)
		)
		{
			/*
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

			int someVal = 3;
			std::cin >> someVal;
		}


		// CASE 1.1: The host is SLICED (A_SLICE); the host triangle line has two border lines in it
		//std::cout << "Line A, " << in_hostLine.numberOfBorderLines << std::endl;
		//std::cout << "Line B, " << in_guestLine.numberOfBorderLines << std::endl;
		else if (in_hostLine.numberOfBorderLines == 2)			// This means: polygon A had two border lines going through polygon B. That means it is SLICED.
		{


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
		)
		{
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
			//std::cout << "CASE 1.3: Triangle A has one border line hit by B; this is a PARTIAL_BOUND" << std::endl;

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
			//std::cout << "PARTIAL_BOUND via condition 1.4 detected. " << std::endl;
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
			//glm::vec3 newSecondPoint = findSecondPointForLine(in_hostLine.pointA, in_guestLine.pointA, in_guestLine.pointB);
			//returnLine.convertLineToPartialBound(in_hostLine, in_guestLine);		// convert to TWIN
			//returnLine.convertLineToPartialBound(in_hostLine, in_guestLine, newSecondPoint);
			//std::cout << "CASE 3.2: " << std::endl;
			//std::cout << "!!! Return line border is: " << returnLine.line.pointABorder << std::endl;

		}

		// CASE 1.5 PARTIAL_BOUND -- condition 2
		else if
		(
			(in_hostLine.numberOfBorderLines == 1)		// A hit's B, but it's only one border line
			&&
			(in_guestLine.numberOfBorderLines == 0)		// B "engulfs" A, but B has no border lines going through A
		)
		{


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

		/*
		else if
		(
			(in_guestLine.lineValidity == IntersectionLineValidity::INVALID)
		)
		{
			std::cout << ":::: testing of new categorized line case, halt. " << std::endl;
			int someVal = 3;
			std::cin >> someVal;
		
		}
		*/

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
			//std::cout << "CASE 1.6: twin-style PARTIAL_BOUND detected. " << std::endl;
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
			//std::cout << "CASE 1.7: NON-BOUND case 2 hit " << std::endl;
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
			//std::cout << "CASE 1.8: No intercept detected. " << std::endl;
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
			std::cout << "~~~~ The host line is INVALID. " << std::endl;
		}
		if (in_guestLine.lineValidity == IntersectionLineValidity::INVALID)
		{
			std::cout << "~~~~ The guest line is INVALID. " << std::endl;
		}

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


		std::cout << "line B, point A: " << in_guestLine.pointA.x << ", " << in_guestLine.pointA.y << ", " << in_guestLine.pointA.z << std::endl;
		std::cout << "line B, point B: " << in_guestLine.pointB.x << ", " << in_guestLine.pointB.y << ", " << in_guestLine.pointB.z << std::endl;
		std::cout << "line B, point A border: is on border? ->" << in_guestLine.isPointAOnBorder << "; " << in_guestLine.pointABorder << std::endl;
		std::cout << "line B, point B border: is on border? ->" << in_guestLine.isPointBOnBorder << "; " << in_guestLine.pointBBorder << std::endl;

		int someVal = 3;
		std::cin >> someVal;

		returnLine.convertLinesToInterceptsPointPrecise(in_hostLine, in_guestLine);
		returnLine.line.lineGroupID = in_groupID;
		returnLine.emptyNormal = in_polyBEmptyNormal;
	}
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
	 //std::cout << "|||||||||||||| ************* Beginning fracturing attempts for all polys ************** ||||||||||||||" << std::endl;
	auto truestart = std::chrono::high_resolution_clock::now();
	for (int x = 0; x < secondaryPolys.size(); x++)
	{
		
		//std::cout << "Cleave map size for poly " << x << ": " << secondaryPolys[x].cleaveMap.size() << std::endl;
		if (secondaryPolys[x].cleaveMap.size() != 0)	// fracture only if there are actual cleave map entries
		{
			// pass the secondary poly to the PolyFracturer
			
			//std::cout << "########## Performing fracturing for poly with ID: " << x << std::endl;
			
			auto truestart = std::chrono::high_resolution_clock::now();
			SPolyFracturer fracturer(x, &secondaryPolys[x], &polyMorphTracker);
			insertPolyFracturingResults(x, fracturer.sPolySG);

			auto trueend = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> trueelapsed = trueend - truestart;
			std::cout << "#-> Fracturing Time   > " << trueelapsed.count() << std::endl;

			int dumbVal = 5;
			//std::cout << "!!! Waiting for dumb input..." << std::endl;
			//std::cin >> dumbVal;
		}
		else if (secondaryPolys[x].cleaveMap.size() == 0)
		{
			insertOriginalPolyAsFracturingResult(x, secondaryPolys[x]);
			std::cout << "!! No cleaveMaps found for this SPoly! " << std::endl;
		}

	}

	// test only: let's make sure our resulting polys are grouped appropriately:
	/*
	std::cout << "################################## Printing out produced contents of the SPolySet: " << std::endl;
	auto polyResultsBegin = polyFracturingResults.begin();
	auto polyResultsEnd = polyFracturingResults.end();
	for (; polyResultsBegin != polyResultsEnd; polyResultsBegin++)
	{
		std::cout << ">> Parent SPoly ID: " << polyResultsBegin->first << ", child SPolys: " << std::endl;
		auto childSPolyVectorBegin = polyResultsBegin->second.begin();
		auto childSPolyVectorEnd = polyResultsBegin->second.end();
		int currentID = 0;
		for (; childSPolyVectorBegin != childSPolyVectorEnd; childSPolyVectorBegin++)
		{
			std::cout << "[" << currentID << "] " << std::endl;
			for (int x = 0; x < 3; x++)
			{
				std::cout << "point " << x << ": " << (*childSPolyVectorBegin).triangles[0].triangleLines[x].pointA.x << ", " << (*childSPolyVectorBegin).triangles[0].triangleLines[x].pointA.y << ", " << (*childSPolyVectorBegin).triangles[0].triangleLines[x].pointA.z << std::endl;
			}
			currentID++;
		}
	}
	*/

	auto trueend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> trueelapsed2 = trueend - truestart;
	//std::cout << "#-> Total Fracturing Time !!  > " << trueelapsed2.count() << std::endl;

	//int continueVal = 3;
	//std::cout << "Fracturing for this poly complete; enter number to continue..." << std::endl;
	//std::cin >> continueVal;
}

void SPolySet::insertPolyFracturingResults(int in_originalSPolyID, SPolySupergroup in_producedSupergroup)
{
	/*
	auto producedSPolyVectorBegin = (*in_producedSPolyVectorRef).begin();
	auto producedSPolyVectorEnd = (*in_producedSPolyVectorRef).end();
	for (; producedSPolyVectorBegin != producedSPolyVectorEnd; producedSPolyVectorBegin++)
	{
		polyFracturingResults[in_originalSPolyID].push_back(*producedSPolyVectorBegin);
	}
	*/
	polyFracturingResults.supergroupMap[in_originalSPolyID] = in_producedSupergroup;
}

void SPolySet::insertOriginalPolyAsFracturingResult(int in_originalSPolyID, SPoly in_sPoly)
{
	//polyFracturingResults[in_originalSPolyID].push_back(in_sPoly);
	polyFracturingResults.supergroupMap[in_originalSPolyID].insertSPoly(in_sPoly);
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
	polyB.addTriangle(polyBTriangle2);
	//polyB.calculateEmptyNormal();
	//polyB.setDebugFlag(1);

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

void SPolySet::runTest4()
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

	addPoly(polyA);
	//polyA.calculateEmptyNormal();


	// MRP for group 1
	glm::vec3 group_1_MRP;
	group_1_MRP.x = 1.0f;
	group_1_MRP.y = .5f;
	group_1_MRP.z = 0.0f;

	// first poly of group 1 (top poly)
	SPoly polyA_1;

	// poly A, in group 1, points 0 through 3...
	glm::vec3 pA_1_0, pA_1_1, pA_1_2, pA_1_3;
	pA_1_0.x = 1.0f;
	pA_1_0.y = .6f;
	pA_1_0.z = 0.1f;

	pA_1_1.x = 1.0f;
	pA_1_1.y = .6f;
	pA_1_1.z = -0.1f;

	pA_1_2.x = 0.9f;
	pA_1_2.y = .6f;
	pA_1_2.z = -0.1f;

	pA_1_3.x = 0.9f;
	pA_1_3.y = 0.6f;
	pA_1_3.z = 0.1f;

	STriangle polyA_1_triangle1(pA_1_0, pA_1_1, pA_1_2);
	STriangle polyA_1_triangle2(pA_1_0, pA_1_2, pA_1_3);
	polyA_1.groupID = 1;
	polyA_1.setMRP(group_1_MRP);
	polyA_1.addTriangle(polyA_1_triangle1);
	polyA_1.addTriangle(polyA_1_triangle2);

	addPoly(polyA_1);



	// second poly of group 1 (side poly)
	SPoly polyB_1;
	
	// poly B, in group 1, points 0 through 3...
	glm::vec3 pB_1_0, pB_1_1, pB_1_2, pB_1_3;
	pB_1_0.x = .9f;
	pB_1_0.y = .6f;
	pB_1_0.z = 0.1f;

	pB_1_1.x = .9f;
	pB_1_1.y = .6f;
	pB_1_1.z = -0.1f;

	pB_1_2.x = .9f;
	pB_1_2.y = .4f;
	pB_1_2.z = -0.1f;

	pB_1_3.x = .9f;
	pB_1_3.y = .4f;
	pB_1_3.z = 0.1f;

	STriangle polyB_1_triangle1(pB_1_0, pB_1_1, pB_1_2);
	STriangle polyB_1_triangle2(pB_1_0, pB_1_2, pB_1_3);
	polyB_1.groupID = 1;
	polyB_1.setMRP(group_1_MRP);
	polyB_1.addTriangle(polyB_1_triangle1);
	polyB_1.addTriangle(polyB_1_triangle2);

	addPoly(polyB_1);


	// third poly of group 1 (bottom poly)
	SPoly polyC_1;

	// poly C, in group 1, points 0 through 3...
	glm::vec3 pC_1_0, pC_1_1, pC_1_2, pC_1_3;
	pC_1_0.x = .9f;
	pC_1_0.y = .4f;
	pC_1_0.z = 0.1f;

	pC_1_1.x = 1.0f;
	pC_1_1.y = 0.4f;
	pC_1_1.z = 0.1f;

	pC_1_2.x = 1.0f;
	pC_1_2.y = 0.4f;
	pC_1_2.z = -0.1f;

	pC_1_3.x = 0.9f;
	pC_1_3.y = 0.4f;
	pC_1_3.z = -0.1f;

	STriangle polyC_1_triangle1(pC_1_0, pC_1_1, pC_1_2);
	STriangle polyC_1_triangle2(pC_1_0, pC_1_2, pC_1_3);
	polyC_1.groupID = 1;
	polyC_1.setMRP(group_1_MRP);
	polyC_1.addTriangle(polyC_1_triangle1);
	polyC_1.addTriangle(polyC_1_triangle2);

	addPoly(polyC_1);

	


	configurePolys();

	runPolyComparison();		// PHASE 1 (generate cleave lines) (see email, 8/20/2019)rc
	performFracturing();			// PHASE 3 run weave for new triangles

	reset();
}