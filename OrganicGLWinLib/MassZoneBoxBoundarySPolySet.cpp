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


	//std::cout << "!!! Size of triangles: " << polyA->triangles.size() << std::endl;
	//std::cout << "!!! Size of border lines: " << polyA->borderLines.size() << std::endl;
	//std::cout << "!!! Size of border lines, direct: " << boundarySPolyRef->borderLines.size() << std::endl;

	//SPoly dumbPoly;
	//std::cout << "!!! Dumb poly size of border lines: " << dumbPoly.borderLines.size() << std::endl;


	SPoly* polyB = in_sPolyRef;
	//std::cout << ">>> Entering before coplanarity checker. " << std::endl;
	CoplanarChecker checker(polyA, polyB, boxBoundarySPolySetLogger.getLogLevel());
	//std::cout << ">>> Passed coplanarity checker. " << std::endl;

	//int waitVal;
	//std::cin >> waitVal;


	if (checker.coplanarityDetected == false)	// can only compare to a guest sPoly that is non-coplanar to the boundary SPoly.
	{
		//std::cout << " >>> Performing comparison. " << std::endl;
		insertCategorizedLinesFromNonboundarySPoly(in_sPolyRef);
	}

	if (boxBoundarySPolySetLogger.isLoggingSet() == true)
	{
		polyA->sequenceFactory.printLineCounts();
		polyA->sequenceFactory.printLinesInPool();
		boxBoundarySPolySetLogger.waitForDebugInput();
	}
	//int waitVal;
	//std::cin >> waitVal;
}

void MassZoneBoxBoundarySPolySet::insertCategorizedLinesFromNonboundarySPoly(SPoly* in_guestPolyPtr)
{
	SPoly* in_hostPolyPtr = boundarySPolyRef;
	int in_guestPolyID = currentComparableSPolyIndex;
	int hostPolyTriangleCount = in_hostPolyPtr->numberOfTriangles;
	int guestPolyTriangleCount = in_guestPolyPtr->numberOfTriangles;

	// acquire the debug level for CategorizedLines.
	PolyLogger currentComparisonLogger;
	currentComparisonLogger.setDebugLevel(categorizedLineDebugLevel);
	currentComparisonLogger.log("(MassZoneBoxBoundarySPolySet) Categorized line debug option found.", "\n");


	for (int currentHostPolyTriangle = 0; currentHostPolyTriangle < hostPolyTriangleCount; currentHostPolyTriangle++)					// compare each of poly A's tertiaries...
	{

		//std::cout << "::::::::::::::::::::::::::::::::::: ----------------------------------+++++++++>>>>>>>>>>> Running host poly Triangle comparison: " << std::endl;

		STriangle* hostTrianglePtr = &in_hostPolyPtr->triangles[currentHostPolyTriangle];	// " " 
		IntersectionLineGroup hostLineGroup(currentComparisonLogger.getLogLevel());						// the line group for poly A.
		IntersectionLineGroup guestLineGroup(currentComparisonLogger.getLogLevel());						// the line group for poly B.
		hostLineGroup.setFusionAnalyzerSPolyRef(in_hostPolyPtr);
		guestLineGroup.setFusionAnalyzerSPolyRef(in_guestPolyPtr);
		for (int y = 0; y < guestPolyTriangleCount; y++)					// .. to each of poly B's tertiaries...
		{
			// for each pair (that is, A's current STriangle to B's current STriangle in the iterations), we must:
			// 1. compare the lines of STriangle A to that of STriangle B
			// 2. compare the lines of STriangle B to that of STriangle A
			// 3. analyze the relationship between them

			// >>>>>>>>>>>>>>>>>>>>> STEP 1
			// compare the host triangle lines, to the guest triangles. |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

			STriangle* guestTrianglePtr = &in_guestPolyPtr->triangles[y]; // get the guest poly's triangle
			//std::cout << "::::::::::::::::::::::::::::::::::: (MassZoneBoxBoundarySPolyset) >>>>>>>>>>>>>>>>>>>>>>>>>>> Comparing lines of the host to the guest triangle " << std::endl;
			//std::cout << ">>> B triangle (Guest triangle) points are: " << std::endl;
			//std::cout << "0: " << guestTrianglePtr->triangleLines[0].pointA.x << ", " << guestTrianglePtr->triangleLines[0].pointA.y << ", " << guestTrianglePtr->triangleLines[0].pointA.z << std::endl;
			//std::cout << "1: " << guestTrianglePtr->triangleLines[1].pointA.x << ", " << guestTrianglePtr->triangleLines[1].pointA.y << ", " << guestTrianglePtr->triangleLines[1].pointA.z << std::endl;
			//std::cout << "2: " << guestTrianglePtr->triangleLines[2].pointA.x << ", " << guestTrianglePtr->triangleLines[2].pointA.y << ", " << guestTrianglePtr->triangleLines[2].pointA.z << std::endl;
			//std::cout << "2 (B): " << guestTrianglePtr->triangleLines[2].pointB.x << ", " << guestTrianglePtr->triangleLines[2].pointB.y << ", " << guestTrianglePtr->triangleLines[2].pointB.z << std::endl;
			for (int currentHostTriangleLine = 0; currentHostTriangleLine < 3; currentHostTriangleLine++)		// run the lines of A (the host) through triangle B (the guest)
			{
				//FusionCandidateProducer hostCandidateProducer(currentComparisonLogger.getLogLevel());
				FusionCandidateProducer hostCandidateProducer(PolyDebugLevel::NONE);
				FusionCandidate hostFusedCandidate = hostCandidateProducer.produceCandidate(*guestTrianglePtr, in_hostPolyPtr->triangles[currentHostPolyTriangle].triangleLines[currentHostTriangleLine]);
				if (hostFusedCandidate.candidateIntersectionResult.wasIntersectFound != 0)
				{
					hostLineGroup.insertFusionCandidateIntoAnalyzer(FusionCandidateOrigin::HOST, currentHostTriangleLine, hostFusedCandidate);
				}
			}
			hostLineGroup.runFusionAnalysisAndDetermineClassifications();
			//std::cout << "::::::::::::::::::::::::::::::::::: (MassZoneBoxBoundarySPolyset) >>>>>>>>>>>>>>>>>>>>>>>>>>> ENDED Comparing lines of the host to the guest triangle " << std::endl;
	

			// >>>>>>>>>>>>>>>>>>>>> STEP 2
			// compare the GUEST triangle lines, to the host triangles. ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
			//std::cout << "::::::::::::::::::::::::::::::::::: (MassZoneBoxBoundarySPolyset) >>>>>>>>>>>>>>>>>>>>>>>>>>> Comparing lines of the guest to the host triangle" << std::endl;
			for (int z = 0; z < 3; z++)		// run the lines of B (the guest) through triangle A (the host)
			{
				//FusionCandidateProducer guestCandidateProducer(currentComparisonLogger.getLogLevel());
				FusionCandidateProducer guestCandidateProducer(PolyDebugLevel::NONE);
				FusionCandidate guestFusedCandidate = guestCandidateProducer.produceCandidate(*hostTrianglePtr, guestTrianglePtr->triangleLines[z]);
				if (guestFusedCandidate.candidateIntersectionResult.wasIntersectFound != 0)
				{
					guestLineGroup.insertFusionCandidateIntoAnalyzer(FusionCandidateOrigin::GUEST, z, guestFusedCandidate);
				}
			}
			guestLineGroup.runFusionAnalysisAndDetermineClassifications();
			//std::cout << "::::::::::::::::::::::::::::::::::: (MassZoneBoxBoundarySPolyset) >>>>>>>>>>>>>>>>>>>>>>>>>>> ENDED Comparing lines of the guest to the host triangle" << std::endl;

			FusedPointReactor reactor(&hostLineGroup.returnLine.completedAnalysis, &guestLineGroup.returnLine.completedAnalysis, currentComparisonLogger.getLogLevel());
			FusedPointReactorResult reactionResult = reactor.getReactorResult();
			reactionResult.resultingLine.parentPoly = in_guestPolyID;
			if (reactionResult.wasLineProduced == true)
			{
				CategorizedLineColinearTester tester(reactionResult.resultingLine, *hostTrianglePtr, categorizedLineCoplanarTestsDebugLevel);
				if (tester.colinearToBorderLineDetected == false)		// the categorized line isn't colinear to any line in the host triangle (remember, context is from host triangle)
				{
					in_hostPolyPtr->sequenceFactory.addCategorizedLine(reactionResult.resultingLine);
				}
				//numberOfIntersections++;
			}
			
			currentComparisonLogger.log("######>>>>>>> (MassZoneBoxBoundarySPolyset) Comparison complete, enter number to continue to next comparison. ", "\n");
			currentComparisonLogger.waitForDebugInput();

			hostLineGroup.reset();
			guestLineGroup.reset();

			//std::cout << "+++++++++ comparing next tertiary..." << std::endl;
		}

		//std::cout << "------------++ done comparing current tertiary in triangle A; moving to next tertiary in triangle A..." << std::endl;

	}
	
	//std::cout << "::::::::::::::::::: MassZoneBoxBonudarySPolySet: done comparison. " << std::endl;
	//int stopVal = 3;
	//std::cin >> stopVal;
	currentComparableSPolyIndex++;	// this must be incremented, to set the appropriate index for the next SPoly (if there are any to compare against)
}

void MassZoneBoxBoundarySPolySet::buildBoundarySPolyFromFactory()
{
	boundarySPolyRef->buildCleaveSequences(CleaveSequenceMergeMode::MERGE);
	if (boundarySPolyRef->cleaveMap.size() != 0)
	{
		//std::cout << "(MassZoneBoxBoundarySPolyset) !!! Found cleave map values in Factory; processing..." << std::endl;
		SPolyMorphTracker morphTracker;
		SPolyFracturer fracturer(0, boundarySPolyRef, &morphTracker, SPolyFracturerOptionEnum::ROTATE_TO_Z, fracturerDebugLevel);

	}
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

	//std::cout << "!!!! Bi-dirrectional comparison results: " << std::endl;
	//std::cout << "ResultA, was found: " << resultA.wasIntersectFound << std::endl;
	//std::cout << "ResultB, was found: " << resultB.wasIntersectFound << std::endl;

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
		if (s < 0.0 || s > 1.001f)         // I is outside S
			//return 0;
		{
			//std::cout << "!! Note: I is outside S. " << std::endl;
			returnResult.setResult(0);
		}
		t = (uv * wu - uu * wv) / D;
		t = float(floor(t * 1000 + 0.5) / 1000);

		std::cout << "--> Value of t: " << t << std::endl;
		if (t < 0.0 || (s + t) > 1.001f)  // I is outside T
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