#include "stdafx.h"
#include "SPolyFracturer.h"

SPolyFracturer::SPolyFracturer(int in_originalPolyID, SPoly* in_sPolyRef, SPolyMorphTracker* in_morphTrackerRef, SPolyFracturerOptionEnum in_option, PolyDebugLevel in_polyDebugLevel)
{
	// set the debug level
	fracturerLoggerDebugLevel = in_polyDebugLevel;
	fracturerLogger.setDebugLevel(in_polyDebugLevel);

	//auto truestart = std::chrono::high_resolution_clock::now();

	originalPolyID = in_originalPolyID;
	polyRef = in_sPolyRef;
	//std::cout << "|||| PRE-ROTATE Prime POINTS: " << std::endl;
	//fracturerLogger.log("(SPolyFracturer) |||| Starting print out of PRE-ROTATE Prime Points: " << std::endl;
	//std::cout << "********************************************************* Printing cleave sequence values, prior to runFracturing " << std::endl;
	if (fracturerLogger.isLoggingSet() == true)
	{
		fracturerLogger.log("(SPolyFracturer) |||| Starting print out of PRE-ROTATE Prime Points: ", "\n");
		polyRef->printAllCleaveLines();
		fracturerLogger.log("(SPolyFracturer) |||| Ended print out of PRE-ROTATE Prime Points: ", "\n");
	}
	//int stopVal = 3;
	//std::cin >> stopVal;


	//std::cout << "0: " << polyRef->primePoint0.x << ", " << polyRef->primePoint0.y << ", " << polyRef->primePoint0.z << std::endl;
	//std::cout << "1: " << polyRef->primePoint1.x << ", " << polyRef->primePoint1.y << ", " << polyRef->primePoint1.z << std::endl;
	//std::cout << "2: " << polyRef->primePoint2.x << ", " << polyRef->primePoint2.y << ", " << polyRef->primePoint2.z << std::endl;
	morphTrackerRef = in_morphTrackerRef;
	//generatePlanarNormalsForPoly();	// generate the planar normal for this poly before fracturing begins

	// if the SPoly isn't aligned to the XY plane (that is, Z = 0), rotate/translate it to Z.
	// The proper rotations/translations are stored in rotationManager, which will then be reverse-applied 
	// in the call to SPolyFracturer::checkForCleaveIntersections().
	//
	// If runFracturing() isn't called, the rotationManager will be empty, but will still be checked in checkForCleaveIntersections().
	rotationManager.setDebugFlag(polyRef->debugFlag);	// set the debug flag
	if (in_option == SPolyFracturerOptionEnum::ROTATE_TO_Z)
	{
		runFracturing();					// rotate points to same Z coordinates
	}
	generatePlanarNormalsForPoly();		// generate the planar normals
	//printPointMetaData();				// show the points before we run the weave

	auto truestart = std::chrono::high_resolution_clock::now();
	checkForCleaveIntersections();		// check for any lines in each cleave sequence that intersect with other lines in another cleave sequence; produce the results and store them.

	auto trueend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> trueelapsed2 = trueend - truestart;
	//std::cout << "#-> (After SPolyFracturer constructor) Fracturing Time !!  > " << std::fixed << trueelapsed2.count() << std::endl;
}

void SPolyFracturer::generatePlanarNormalsForPoly()
{
	polyRef->determinePlanarVectors();	// calculate the planar normals for each border line of the poly
}

void SPolyFracturer::checkForCleaveIntersections()
{
	CleaveSequenceIntersectFinder intersectFinder(originalPolyID, polyRef, fracturerLoggerDebugLevel);		// assumes that all coordinate's have been translated such that the coordinates of the poly to be fratured have their Z = 0.
	quatPoints.clearPoints();															// clear out the quat points (which needs to be done if runFracturing() is called), so that we may insert the below.

	intersectFinder.triangleSupergroup.loadTrianglesIntoQuatRotationPoints(&quatPoints);

	// remember, work in reverse order: quaternions get reverse-applied, then reverse any translations.
	rotationManager.rotateToOriginalPosition();
	if (pointTranslator.requiresTranslation == 1)
	{
		quatPoints.applyTranslation(pointTranslator.getReverseTranslationValue());
	}

	// Remember: 1 WeldedTriangle = 1 STriangle
	//			 1 WeldedTriangleContainer = 1 SPoly 


	// after quaternions are reverse applied, and after any reverse translation is applied, produce the new SPolys; by putting them into 
	// an SPolySupergroup (sPolySG)

	auto triangleSupergroupBegin = intersectFinder.triangleSupergroup.triangleContainerMap.begin();
	auto triangleSupergroupEnd = intersectFinder.triangleSupergroup.triangleContainerMap.end();
	for (; triangleSupergroupBegin != triangleSupergroupEnd; triangleSupergroupBegin++)
	{
		SPoly newPoly;
		auto triangleContainerBegin = triangleSupergroupBegin->second.triangleMap.begin();
		auto triangleContainerEnd = triangleSupergroupBegin->second.triangleMap.end();
		for (; triangleContainerBegin != triangleContainerEnd; triangleContainerBegin++)
		{
			glm::vec3 weldedTrianglePoint0 = triangleContainerBegin->second.fetchTriangleLine(0).pointA;
			glm::vec3 weldedTrianglePoint1 = triangleContainerBegin->second.fetchTriangleLine(1).pointA;
			glm::vec3 weldedTrianglePoint2 = triangleContainerBegin->second.fetchTriangleLine(2).pointA;
			STriangle newTriangle(weldedTrianglePoint0, weldedTrianglePoint1, weldedTrianglePoint2);
			newPoly.polyEmptyNormal = polyRef->polyEmptyNormal;
			newPoly.addTriangle(newTriangle);
		}
		/*
		if (fracturerLogger.isLoggingSet() == true)
		{
			if (newPoly.isSPolyValid() == false)
			{
				std::cout << "!!!! Warning, SPoly is invalid! " << std::endl;
			}
		}
		*/

		// an SPoly should only be added to the SPolySuperGroup if it's actually valid (meaning, 3 unique points in all of its STriangles.)
		if
		(
			(newPoly.triangles.size() != 0)
			&&
			(newPoly.isSPolyValid() == true)
		)
		{
			sPolySG.insertSPoly(newPoly);	// insert the new poly when we are all done
		}
	}
	//sPolySG.printSPolys();


	//quatPoints.printPoints();
	//std::cout << "####################### Poly fracturing complete..... " << std::endl;
	//int someVal = 3;
	//std::cin >> someVal;

	if (fracturerLogger.isLoggingSet() == true)
	{
		fracturerLogger.log("(SPolyFracturer) >>>> Started printing of resulting sPolySG SPoly values.....  ", "\n");
		//quatPoints.printPoints();
		sPolySG.printSPolys();
		fracturerLogger.log("(SPolyFracturer) >>>> Ended printing of resulting sPolySG SPoly values.....  ", "\n");
		fracturerLogger.log("(SPolyFracturer) ####################### Poly fracturing complete.....  ", "\n");
		fracturerLogger.waitForDebugInput();
	}
}

void SPolyFracturer::runFracturing()
{
	//std::cout << " ######################################################################################### ******************************************* PRE-PRE PRINT " << std::endl;
	//printPointMetaData();

	auto truestart = std::chrono::high_resolution_clock::now();
	//PointTranslationCheck pointTranslator; // check for any translation
	pointTranslator.performCheck(polyRef->borderLines[0].pointA);
	if (pointTranslator.requiresTranslation == 1)	// almost 100% of the time, this will be run
	{
		//auto truestart = std::chrono::high_resolution_clock::now();
		
		//std::cout << "!! Border Line 0 points, prior to translation: " << std::endl;
		//std::cout << "!! Border Line 0, point A: " << polyRef->borderLines[0].pointA.x << ", " << polyRef->borderLines[0].pointA.y << ", " << polyRef->borderLines[0].pointA.z << std::endl;
		//std::cout << "!! Border Line 0, point B: " << polyRef->borderLines[0].pointB.x << ", " << polyRef->borderLines[0].pointB.y << ", " << polyRef->borderLines[0].pointB.z << std::endl;

		applyTranslationToAllPoints(pointTranslator.getTranslationValue());	// apply the first translation value to all the points
		//auto trueend = std::chrono::high_resolution_clock::now();
		//std::chrono::duration<double> trueelapsed = trueend - truestart;
		//std::cout << "#-> (SPolyFracturer) translation application Time !!  >              " << std::fixed << trueelapsed.count() << std::endl;

		


		//std::cout << "#- Border Line Analysis > Analyzing border line 0 point A for translation...Border line 0 point A required TRANSLATION..." << std::endl;
	}
	else if (pointTranslator.requiresTranslation == 0)
	{
		//std::cout << "#- Border Line Analysis > Analyzing border line 0 point A for translation...Border line 0 point A required NO TRANSLATION..." << std::endl;
	}
	//auto truestart = std::chrono::high_resolution_clock::now();
	populatePointsForQuaternions();	// populate the points before the quaternion is applied

	//std::cout << " ######################################################################################### ******************************************* PRE PRINT " << std::endl;
	//printPointMetaData();

	//auto trueend = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> trueelapsed = trueend - truestart;
	//std::cout << "#-> (SPolyFracturer) point population Time !!  >              " << std::fixed << trueelapsed.count() << std::endl;

	//auto truestart2 = std::chrono::high_resolution_clock::now();
	rotationManager.initializeAndRunForZFracture(&quatPoints);
	//auto trueend2 = std::chrono::high_resolution_clock::now();
	//std::chrono::duration<double> trueelapsed2 = trueend2 - truestart2;
	//std::cout << "#-> (SPolyFracturer) initialize and run Z-fracture Time !!  > " << std::fixed << trueelapsed2.count() << std::endl;

	//std::cout << "#- Quaternion Rotation  > All SPoly border line and cleave sequence points have been translated (Z is 0 for all border lines, aka \"Z-Planar\")" << std::endl;

	auto trueend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> trueelapsed = trueend - truestart;
	//std::cout << "#-> (SPolyFracturer) entire Time !!  >              " << std::fixed << trueelapsed.count() << std::endl;

	//std::cout << " ######################################################################################### ******************************************* POST PRINT " << std::endl;
	printPointMetaData();
	//std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||~~~~~~~~~~~~~~~~~~~~~~~~ finished printing meta data. " << std::endl;
	//int finish = 3;
	//std::cin >> finish;
}

void SPolyFracturer::applyTranslationToAllPoints(glm::vec3 in_translationOffset)
{
	// apply to the primal points
	polyRef->primePoint0 += in_translationOffset;
	polyRef->primePoint1 += in_translationOffset;
	polyRef->primePoint2 += in_translationOffset;

	// apply to the cleave map points
	if (polyRef->debugFlag == 1)
	{
		std::cout << "!!!! Translation required, applying..." << std::endl;
		std::cout << "!!!! Translation value to apply is: " << in_translationOffset.x << ", " << in_translationOffset.y << ", " << in_translationOffset.z << std::endl;
	}

	auto cleaveMapBegin = polyRef->cleaveMap.begin();
	auto cleaveMapEnd = polyRef->cleaveMap.end();
	for (cleaveMapBegin; cleaveMapBegin != cleaveMapEnd; cleaveMapBegin++)
	{
		auto currentLinesBegin = cleaveMapBegin->second.cleavingLines.begin();
		auto currentLinesEnd = cleaveMapBegin->second.cleavingLines.end();
		for (currentLinesBegin; currentLinesBegin != currentLinesEnd; currentLinesBegin++)
		{
			//std::cout << "!! Point A before translation is: " << currentLinesBegin->second.line.pointA.x << ", " << currentLinesBegin->second.line.pointA.y << ", " << currentLinesBegin->second.line.pointA.z << std::endl;
			//std::cout << "!! Point B before translation is: " << currentLinesBegin->second.line.pointB.x << ", " << currentLinesBegin->second.line.pointB.y << ", " << currentLinesBegin->second.line.pointB.z << std::endl;

			currentLinesBegin->second.line.pointA += in_translationOffset;
			currentLinesBegin->second.line.pointB += in_translationOffset;

			//std::cout << "Border Line " << currentLinesBegin->first << ":--> Point A (" << currentLinesBegin->second.line.pointA.x << ", " << currentLinesBegin->second.line.pointA.y << ", " << currentLinesBegin->second.line.pointA.z
																	//<< ") | Point B (" << currentLinesBegin->second.line.pointB.x << ", " << currentLinesBegin->second.line.pointB.y << ", " << currentLinesBegin->second.line.pointB.z << ") " <<  std::endl;
		}
	}

	// apply to the border lines
	int totalBorderLines = polyRef->numberOfSPolyBorderLines;
	for (int x = 0; x < totalBorderLines; x++)
	{
		polyRef->borderLines[x].pointA += in_translationOffset;
		polyRef->borderLines[x].pointB += in_translationOffset;

		if (polyRef->debugFlag == 1)
		{
			std::cout << "Border Line " << x << ":--> Point A (" << polyRef->borderLines[x].pointA.x << ", " << polyRef->borderLines[x].pointA.y << ", " << polyRef->borderLines[x].pointA.z
				<< ":--> point B (" << polyRef->borderLines[x].pointB.x << ", " << polyRef->borderLines[x].pointB.y << ", " << polyRef->borderLines[x].pointB.z << std::endl;
		}

	}
}

void SPolyFracturer::populatePointsForQuaternions()
{
	if (polyRef->debugFlag == 1)
	{
		std::cout << "Populate points for quaternion rotation(s)..." << std::endl;
	}
	// insert the primal points -- these should be inserted first
	quatPoints.insertPointRefs(&polyRef->primePoint0, &polyRef->primePoint1, &polyRef->primePoint2);


	// insert the cleave map points, and their normals
	auto cleaveMapBegin = polyRef->cleaveMap.begin();
	auto cleaveMapEnd = polyRef->cleaveMap.end();
	for (cleaveMapBegin; cleaveMapBegin != cleaveMapEnd; cleaveMapBegin++)
	{
		auto currentLinesBegin = cleaveMapBegin->second.cleavingLines.begin();
		auto currentLinesEnd = cleaveMapBegin->second.cleavingLines.end();
		for (currentLinesBegin; currentLinesBegin != currentLinesEnd; currentLinesBegin++)
		{
			// insert references to point A, point B, and the empty normal
			quatPoints.insertPointRefs(&currentLinesBegin->second.line.pointA, &currentLinesBegin->second.line.pointB, &currentLinesBegin->second.emptyNormal);
		}
	}

	// insert the border line points
	int totalBorderLines = polyRef->numberOfSPolyBorderLines;
	for (int x = 0; x < totalBorderLines; x++)
	{
		quatPoints.insertPointRefs(&polyRef->borderLines[x].pointA, &polyRef->borderLines[x].pointB, &polyRef->borderLines[x].planarVector);
	}
}

void SPolyFracturer::printPointMetaData()
{
	if (polyRef->debugFlag == 1)
	{
		auto pointsStart = quatPoints.getPointsVectorBegin();

		// print the points
		std::cout << (*pointsStart)->x << std::endl;
		std::cout << "--------------------------------------------- Printing point meta-data for Fracturing process -----------------------------------------------------" << std::endl;
		std::cout << "|| Group ID: " << polyRef->groupID << std::endl;
		std::cout << "|| Prime Triangle points, after being translated to 2D X/Y plane: " << std::endl;
		for (int x = 0; x < 3; x++)
		{
			std::cout << (*pointsStart)->x << ", " << (*pointsStart)->y << ", " << (*pointsStart)->z << std::endl;
			pointsStart++;
		}

		// print the cleave map points, and their normals
		std::cout << "|| Cleave map elements " << std::endl;
		auto cleaveMapBegin = polyRef->cleaveMap.begin();
		auto cleaveMapEnd = polyRef->cleaveMap.end();
		for (cleaveMapBegin; cleaveMapBegin != cleaveMapEnd; cleaveMapBegin++)
		{
			std::cout << "---Cleave index at " << cleaveMapBegin->first << " --- " << std::endl;
			auto currentLinesBegin = cleaveMapBegin->second.cleavingLines.begin();
			auto currentLinesEnd = cleaveMapBegin->second.cleavingLines.end();
			for (currentLinesBegin; currentLinesBegin != currentLinesEnd; currentLinesBegin++)
			{
				std::cout << "--Point A: " << (*pointsStart)->x << ", " << (*pointsStart)->y << ", " << (*pointsStart)->z << std::endl;
				pointsStart++;
				std::cout << "--Point B: " << (*pointsStart)->x << ", " << (*pointsStart)->y << ", " << (*pointsStart)->z << std::endl;
				pointsStart++;
				std::cout << "--Empty normal: " << (*pointsStart)->x << ", " << (*pointsStart)->y << ", " << (*pointsStart)->z << std::endl;
				pointsStart++;
			}
		}

		// ptint the border line points
		std::cout << "|| Border line points " << std::endl;
		int totalBorderLines = polyRef->numberOfSPolyBorderLines;
		for (int x = 0; x < totalBorderLines; x++)
		{
			std::cout << "---Border line at " << x << " --- " << std::endl;
			std::cout << "--Point A: " << (*pointsStart)->x << ", " << (*pointsStart)->y << ", " << (*pointsStart)->z << std::endl;
			pointsStart++;
			std::cout << "--Point B: " << (*pointsStart)->x << ", " << (*pointsStart)->y << ", " << (*pointsStart)->z << std::endl;
			pointsStart++;
			std::cout << "--Planar vector: " << (*pointsStart)->x << ", " << (*pointsStart)->y << ", " << (*pointsStart)->z << std::endl;
			pointsStart++;
		}
		std::cout << std::endl;
	}
}