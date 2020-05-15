#include "stdafx.h"
#include "SPolyFracturer.h"

SPolyFracturer::SPolyFracturer(SPoly* in_sPolyRef, SPolyMorphTracker* in_morphTrackerRef)
{
	polyRef = in_sPolyRef;
	std::cout << "|||| PRE-ROTATE Prime POINTS: " << std::endl;
	std::cout << "0: " << polyRef->primePoint0.x << ", " << polyRef->primePoint0.y << ", " << polyRef->primePoint0.z << std::endl;
	std::cout << "1: " << polyRef->primePoint1.x << ", " << polyRef->primePoint1.y << ", " << polyRef->primePoint1.z << std::endl;
	std::cout << "2: " << polyRef->primePoint2.x << ", " << polyRef->primePoint2.y << ", " << polyRef->primePoint2.z << std::endl;
	morphTrackerRef = in_morphTrackerRef;
	//generatePlanarNormalsForPoly();	// generate the planar normal for this poly before fracturing begins
	rotationManager.setDebugFlag(polyRef->debugFlag);	// set the debug flag
	runFracturing();					// rotate points to same Z coordinates
	generatePlanarNormalsForPoly();		// generate the planar normals
	printPointMetaData();				// show the points before we run the weave
	checkForCleaveIntersections();		// check for any lines in each cleave sequence that intersect with other lines in another cleave sequence

	int dumbVal = 3;
	std::cout << "!!! Waiting for dumb input..." << std::endl;
	std::cin >> dumbVal;
}

void SPolyFracturer::generatePlanarNormalsForPoly()
{
	polyRef->determinePlanarVectors();	// calculate the planar normals for each border line of the poly
}

void SPolyFracturer::checkForCleaveIntersections()
{
	CleaveSequenceIntersectFinder intersectFinder(polyRef);		// assumes that all coordinate's have been translated such that the coordinates of the poly to be fratured have their Z = 0.
}

void SPolyFracturer::runFracturing()
{
	PointTranslationCheck pointTranslator; // check for any translation
	pointTranslator.performCheck(polyRef->borderLines[0].pointA);
	if (pointTranslator.requiresTranslation == 1)	// almost 100% of the time, this will be run
	{
		applyTranslationToAllPoints(pointTranslator.getTranslationValue());	// apply the first translation value to all the points
	}
	populatePointsForQuaternions();	// populate the points before the quaternion is applied
	rotationManager.initializeAndRunForZFracture(&quatPoints);
	//printPointMetaData();
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
	int totalBorderLines = polyRef->numberOfBorderLines;
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
	quatPoints.pointsRef.push_back(&polyRef->primePoint0);
	quatPoints.pointsRef.push_back(&polyRef->primePoint1);
	quatPoints.pointsRef.push_back(&polyRef->primePoint2);


	// insert the cleave map points, and their normals
	auto cleaveMapBegin = polyRef->cleaveMap.begin();
	auto cleaveMapEnd = polyRef->cleaveMap.end();
	for (cleaveMapBegin; cleaveMapBegin != cleaveMapEnd; cleaveMapBegin++)
	{
		auto currentLinesBegin = cleaveMapBegin->second.cleavingLines.begin();
		auto currentLinesEnd = cleaveMapBegin->second.cleavingLines.end();
		for (currentLinesBegin; currentLinesBegin != currentLinesEnd; currentLinesBegin++)
		{
			quatPoints.pointsRef.push_back(&currentLinesBegin->second.line.pointA);	// push a refernce to point A into the vector
			quatPoints.pointsRef.push_back(&currentLinesBegin->second.line.pointB);	// push a refernce to point A into the vector
			quatPoints.pointsRef.push_back(&currentLinesBegin->second.emptyNormal);	// push a reference to the empty normal vector into the vector
		}
	}

	// insert the border line points
	int totalBorderLines = polyRef->numberOfBorderLines;
	for (int x = 0; x < totalBorderLines; x++)
	{
		quatPoints.pointsRef.push_back(&polyRef->borderLines[x].pointA);	// push back point A
		quatPoints.pointsRef.push_back(&polyRef->borderLines[x].pointB);	// ""
		quatPoints.pointsRef.push_back(&polyRef->borderLines[x].planarVector);
	}
}

void SPolyFracturer::printPointMetaData()
{
	if (polyRef->debugFlag == 1)
	{
		std::vector<glm::vec3*>::iterator pointsStart = quatPoints.pointsRef.begin();
		// print the points
		std::cout << (*pointsStart)->x << std::endl;
		std::cout << "--------------------------------------------- Printing point meta-data for Fracturing process -----------------------------------------------------" << std::endl;
		std::cout << "|| Group ID: " << polyRef->groupID << std::endl;
		std::cout << "|| Triangle points " << std::endl;
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
				//quatPoints.pointsRef.push_back(&currentLinesBegin->second.line.pointA);	// push a refernce to point A into the vector
				//quatPoints.pointsRef.push_back(&currentLinesBegin->second.line.pointB);	// push a refernce to point A into the vector
				//quatPoints.pointsRef.push_back(&currentLinesBegin->second.emptyNormal);	// push a reference to the empty normal vector into the vector
			}
		}

		// ptint the border line points
		std::cout << "|| Border line points " << std::endl;
		int totalBorderLines = polyRef->numberOfBorderLines;
		for (int x = 0; x < totalBorderLines; x++)
		{
			std::cout << "---Border line at " << x << " --- " << std::endl;
			//quatPoints.pointsRef.push_back(&polyRef->borderLines[x].pointA);	// push back point A
			//quatPoints.pointsRef.push_back(&polyRef->borderLines[x].pointB);	// ""
			//quatPoints.pointsRef.push_back(&polyRef->borderLines[x].planarVector);
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