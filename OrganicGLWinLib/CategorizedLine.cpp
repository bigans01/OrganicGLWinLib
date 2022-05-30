#include "stdafx.h"
#include "CategorizedLine.h"
#include "SPolyBorderLines.h"

void CategorizedLine::convertLineToPartialBound(IntersectionLine in_lineA, IntersectionLine in_lineB, glm::vec3 in_newPointForLineA)
{
	type = IntersectionType::PARTIAL_BOUND;
	line.numberOfBorderLines = 1;		// PARTIAL_BOUND always has one border line, and a non-bound point.
	line.isPointAOnBorder = 1;											// point A should always be the point on a border in a PARTIAL_BOUND type (need to verify, 8/2/2019)
	line.pointA = in_lineA.pointA;										// set point A
	line.pointB = in_newPointForLineA;									// set point B, set in the previous function
	//line.intersectedSecondaryID = in_lineA.intersectedSecondaryID;		// set the secondary ID, which is the value of the polygon that caused the intersection
}

void CategorizedLine::convertLineToPartialBound(IntersectionLine in_lineA, IntersectionLine in_lineB)
{
	type = IntersectionType::PARTIAL_BOUND;		// TWIN always has exactly one point on a border line, and a non-bound point.
	line.numberOfBorderLines = 1;
	line.isPointAOnBorder = 1;
	line.pointA = in_lineA.pointA;
	line.pointB = in_lineB.pointA;
	//line.intersectedSecondaryID = in_lineA.intersectedSecondaryID;
}

void CategorizedLine::convertLineToNonbound(IntersectionLine in_engulfedLine)
{
	//std::cout << ":::: Engulfed NON_BOUND detected..." << std::endl;


	type = IntersectionType::NON_BOUND;
	line.numberOfBorderLines = 0;		// NON_BOUND will never have any border lines...
	line.pointA = in_engulfedLine.pointA;		// ...but it's points are equal to the "engulfed" line
	line.pointB = in_engulfedLine.pointB;

	//std::cout << ">> Engulfed, point A: " << line.pointA.x << ", " << line.pointA.y << ", " << line.pointA.z << std::endl;
	//std::cout << ">> Engulfed, point B: " << line.pointB.x << ", " << line.pointB.y << ", " << line.pointB.z << std::endl;

	//line.intersectedSecondaryID = in_engulfedLine.intersectedSecondaryID;
}

void CategorizedLine::convertLinesToNonbound(IntersectionLine in_lineA, IntersectionLine in_lineB)
{
	type = IntersectionType::NON_BOUND;
	line.numberOfBorderLines = 0;
	line.pointA = in_lineA.pointA;
	line.pointB = in_lineB.pointA;
	//line.intersectedSecondaryID = in_lineA.intersectedSecondaryID;
}

void CategorizedLine::convertLinesToInterceptsPointPrecise(IntersectionLine in_lineA, IntersectionLine in_lineB)
{
	// if in_lineA is the invalid one, use the points in  in_lineB.
	if 
	(
		(in_lineA.lineValidity == IntersectionLineValidity::INVALID)
		&& (in_lineB.lineValidity == IntersectionLineValidity::VALID)
	)
	{
		//std::cout << "::::: Line categorization, INTERCEPT_POINT_PRECISE, lineA " << std::endl;

		type = IntersectionType::INTERCEPTS_POINT_PRECISE;

		
		line.pointA = in_lineB.pointA;
		line.pointB = in_lineB.pointB;
		line.numberOfBorderLines = 1;
		
		// set both intercepted border lines that were in line A; we'll use these later.
		line.pointABorder = in_lineA.pointABorder;
		line.pointBBorder = in_lineA.pointBBorder;
	}
	else if 
	(
		(in_lineB.lineValidity == IntersectionLineValidity::INVALID)
		&& (in_lineA.lineValidity == IntersectionLineValidity::VALID)
	)
	{
		//std::cout << "::::: Line categorization, INTERCEPT_POINT_PRECISE, lineB " << std::endl;

		type = IntersectionType::INTERCEPTS_POINT_PRECISE;

		line.pointA = in_lineA.pointA;
		line.pointB = in_lineA.pointB;
		line.numberOfBorderLines = 1;

		// set both intercepted border lines that were in line A; we'll use these later.
		line.pointABorder = in_lineB.pointABorder;
		line.pointBBorder = in_lineB.pointBBorder;
	}
	else
	{
		std::cout << "!!!!!!!! ++++ > WARNING, both lines considered invalid! " << std::endl;
	}
	// do for in_lineB? maybe.
}

bool CategorizedLine::determineCyclingDirection(std::map<int, SPolyBorderLines> in_borderLineArrayRef, PolyDebugLevel in_polyDebugLevel)
//void CategorizedLine::determineCyclingDirection(SPolyBorderLines in_borderLineArrayRef)
{
	//std::cout << "Determining cycling direction for this line; the line's parent SPoly is: " << parentPoly << std::endl;
	bool wasRunValid = true;	// assumes true.

	// logic for partial lines.
	if (type == IntersectionType::PARTIAL_BOUND)
	{
		PolyLogger partialLogger;
		partialLogger.setDebugLevel(in_polyDebugLevel);


		int categorizedLineBorderID = line.getBorderLineIDFromSingularBorderLineCount();
		glm::vec3 borderPoint = line.getBorderPointFromSingularBorderLineCount();
		glm::vec3 emptyNormalCopy = emptyNormal;		// get a copy of the normal, don't modify the original.

		//std::cout << "Border point is: " << borderPoint.x << ", " << borderPoint.y << ", " << borderPoint.z << std::endl;
		//std::cout << "Empty normal is: " << emptyNormal.x << ", " << emptyNormal.y << ", " << emptyNormal.z << std::endl;
		//std::cout << "Border point is on border line: " << categorizedLineBorderID << std::endl;



		QuatRotationPoints rotationPoints;
		SPolyBorderLines borderLineCopy = in_borderLineArrayRef[categorizedLineBorderID];


		//std::cout << "!! (PARTIAL_BOUND) Border line point A: " << borderLineCopy.pointA.x << ", " << borderLineCopy.pointA.y << ", " << borderLineCopy.pointA.z << std::endl;
		//std::cout << "!! (PARTIAL_BOUND) Border line point B: " << borderLineCopy.pointB.x << ", " << borderLineCopy.pointB.y << ", " << borderLineCopy.pointB.z << std::endl;
		//std::cout << "!! (PARTIAL_BOUND) Border point: " << borderPoint.x << ", " << borderPoint.y << ", " << borderPoint.z << std::endl;
		//std::cout << "!! (PARTIAL_BOUND) Empty normal is: " << emptyNormal.x << ", " << emptyNormal.y << ", " << emptyNormal.z << std::endl;

		// add pointA, borderPoint, and pointB, to translate them.
		rotationPoints.insertPointRefs(&borderLineCopy.pointA, &borderPoint, &borderLineCopy.pointB);

		PointTranslationCheck pointTranslator;
		pointTranslator.performCheck(rotationPoints.getPointByIndex(1));	// get the "borderPoint", translate to it if necessary.
		if (pointTranslator.requiresTranslation == 1)
		{
			rotationPoints.applyTranslation(pointTranslator.getTranslationValue());
		}

		// now, add the normal at the end.
		rotationPoints.insertPointRefs(&emptyNormalCopy);

		if (partialLogger.isLoggingSet())
		{
			partialLogger.log("(CategorizedLine): determineCyclingDirection for PARTIAL_BOUND; printing points in rotationPoints.", "\n");
			rotationPoints.printPoints();
			partialLogger.waitForDebugInput();
		}

		//QuatRotationManager rotationManager;
		//rotationManager.initializeAndRunForFindingBorderLine(&rotationPoints);
		//rotationManager.initializeAndRunForCyclingDirectionFinderV2(&rotationPoints);
		QMVoidFindCyclingDirection directionFinder;
		directionFinder.solve(&rotationPoints, in_polyDebugLevel);

		// determine which point it is that is positive y (check the first and third points.)
		glm::vec3 candidateOne = rotationPoints.getPointByIndex(0);
		glm::vec3 candidateTwo = rotationPoints.getPointByIndex(2);

		//std::cout << "|||| Post quaternion values: " << std::endl;
		//std::cout << "(PARTIAL_BOUND) !-> Candidate one is: " << candidateOne.x << ", " << candidateOne.y << ", " << candidateOne.z << std::endl;
		//std::cout << "(PARTIAL_BOUND) !-> Candidate two is: " << candidateTwo.x << ", " << candidateTwo.y << ", " << candidateTwo.z << std::endl;
		//std::cout << "(PARTIAL_BOUND) Empty normal copy is: " << emptyNormalCopy.x << ", " << emptyNormalCopy.y << ", " << emptyNormalCopy.z << std::endl;
		//int someVal = 3;
		//std::cin >> someVal;

		glm::vec3 selectedPoint;
		if (candidateOne.y > 0)
		{
			selectedPoint = candidateOne;
		}
		else if (candidateTwo.y > 0)
		{
			selectedPoint = candidateTwo;
		}

		// if the selected point, is B of the border line, we go forward.
		if (borderLineCopy.pointB == selectedPoint)
		{
			//std::cout << "::: Direction is FORWARD, heading towards point B: " << in_borderLineArrayRef[categorizedLineBorderID].pointB.x << ", " << in_borderLineArrayRef[categorizedLineBorderID].pointB.y << ", " << in_borderLineArrayRef[categorizedLineBorderID].pointB.z << std::endl;
			direction = CyclingDirection::FORWARD;
		}
		else if (borderLineCopy.pointA == selectedPoint)
		{

			//std::cout << "::: Direction is REVERSE, heading towards point A: " << in_borderLineArrayRef[categorizedLineBorderID].pointA.x << ", " << in_borderLineArrayRef[categorizedLineBorderID].pointA.y << ", " << in_borderLineArrayRef[categorizedLineBorderID].pointA.z << std::endl;
			direction = CyclingDirection::REVERSE;
		}
		else
		{
			std::cout << "!!! NO DIRECTION FOUND, halting. " << std::endl;
			int someVal = 3;
			std::cin >> someVal;
		}

		//std::cout << ">>> Enter number to check next partial bound... " << std::endl;
		//int someVal = 3;
		//std::cin >> someVal;
	}
	else if (type == IntersectionType::A_SLICE)
	{
		//std::cout << "!! A_SLICE branch entered..." << std::endl;


		int categorizedLineBorderID = line.pointBBorder;	// doesn't matter which one we use, let's just use A
		glm::vec3 borderPoint = line.pointB;
		glm::vec3 emptyNormalCopy = emptyNormal;		// get a copy of the normal, don't modify the original.
		//std::cout << "!! Empty normal is: " << emptyNormalCopy.x << ", " << emptyNormalCopy.y << ", " << emptyNormalCopy.z << std::endl;

	
		QuatRotationPoints rotationPoints;
		SPolyBorderLines borderLineCopy = in_borderLineArrayRef[categorizedLineBorderID];

		//std::cout << "!! Border line point A: " << borderLineCopy.pointA.x << ", " << borderLineCopy.pointA.y << ", " << borderLineCopy.pointA.z << std::endl;
		//std::cout << "!! Border line point B: " << borderLineCopy.pointB.x << ", " << borderLineCopy.pointB.y << ", " << borderLineCopy.pointB.z << std::endl;
		//std::cout << "!! Border point: " << borderPoint.x << ", " << borderPoint.y << ", " << borderPoint.z << std::endl;

		// add pointA, borderPoint, and pointB, to translate them.
		rotationPoints.insertPointRefs(&borderLineCopy.pointA, &borderPoint, &borderLineCopy.pointB);

		PointTranslationCheck pointTranslator;
		pointTranslator.performCheck(rotationPoints.getPointByIndex(1));	// get the "borderPoint", translate to it if necessary.
		if (pointTranslator.requiresTranslation == 1)
		{
			rotationPoints.applyTranslation(pointTranslator.getTranslationValue());
		}

		// now, add the normal at the end.
		rotationPoints.insertPointRefs(&emptyNormalCopy);

		//QuatRotationManager rotationManager;
		//rotationManager.initializeAndRunForFindingBorderLine(&rotationPoints);
		//rotationManager.initializeAndRunForCyclingDirectionFinderV2(&rotationPoints);
		QMVoidFindCyclingDirection directionFinder;
		directionFinder.solve(&rotationPoints, in_polyDebugLevel);

		// determine which point it is that is positive y (check the first and third points.)
		glm::vec3 candidateOne = rotationPoints.getPointByIndex(0);
		glm::vec3 candidateTwo = rotationPoints.getPointByIndex(2);

		//std::cout << "!-> Candidate one is: " << candidateOne.x << ", " << candidateOne.y << ", " << candidateOne.z << std::endl;
		//std::cout << "!-> Candidate two is: " << candidateTwo.x << ", " << candidateTwo.y << ", " << candidateTwo.z << std::endl;

		//std::cout << "!! (Post run) Empty normal is: " << emptyNormalCopy.x << ", " << emptyNormalCopy.y << ", " << emptyNormalCopy.z << std::endl;

		glm::vec3 selectedPoint;
		if (candidateOne.y > 0)
		{
			selectedPoint = candidateOne;
		}
		else if (candidateTwo.y > 0)
		{
			selectedPoint = candidateTwo;
		}

		//std::cout << "Select point is: " << selectedPoint.x << ", " << selectedPoint.y << ", " << selectedPoint.z << std::endl;

		// if the selected point, is B of the border line, we go forward.
		if (borderLineCopy.pointB == selectedPoint)
		{
			//std::cout << "::: Direction is FORWARD, heading towards point B: " << in_borderLineArrayRef[categorizedLineBorderID].pointB.x << ", " << in_borderLineArrayRef[categorizedLineBorderID].pointB.y << ", " << in_borderLineArrayRef[categorizedLineBorderID].pointB.z << std::endl;
			direction = CyclingDirection::FORWARD;
		}
		else if (borderLineCopy.pointA == selectedPoint)
		{

			//std::cout << "::: Direction is REVERSE, heading towards point A: " << in_borderLineArrayRef[categorizedLineBorderID].pointA.x << ", " << in_borderLineArrayRef[categorizedLineBorderID].pointA.y << ", " << in_borderLineArrayRef[categorizedLineBorderID].pointA.z << std::endl;
			direction = CyclingDirection::REVERSE;
		}
		else
		{
			std::cout << "!!!! Warning: direction was NOT determined! " << std::endl;
			wasRunValid = false;
		}

		//int someVal = 3;
		//std::cin >> someVal;
	}

	// logic for intercepts_point_precise
	else if (type == IntersectionType::INTERCEPTS_POINT_PRECISE)
	{

		// fetching border lines...
		SPolyBorderLines borderLineA = in_borderLineArrayRef[line.pointABorder];
		SPolyBorderLines borderLineB = in_borderLineArrayRef[line.pointBBorder];

		//std::cout << "(CategorizedLine): line.pointABorder value is: " << line.pointABorder << std::endl;
		//std::cout << "(CategorizedLine): line.pointBBorder value is: " << line.pointBBorder << std::endl;

		// will set the direction of the line...
		generateCyclingDirectionForInterceptPointPrecise(borderLineA, line.pointABorder, borderLineB, line.pointBBorder, emptyNormal, in_polyDebugLevel);
	}

	// logic for A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE
	else if (type == IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE)
	{
		std::cout << "(CategorizedLine): preparing to enter logic for handling cycling directions for a CategorizedLine of type A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE" << std::endl;
		if (containsExtraData == true)
		{
			// get the border line IDs, from the optional data of Point A.
			auto pointAData = extraData->getPointASubData();
			SPolyBorderLines sliceSingleInterceptBorderLineA = in_borderLineArrayRef[pointAData.begin()->borderLineValue];
			SPolyBorderLines sliceSingleInterceptBorderLineB = in_borderLineArrayRef[pointAData.rbegin()->borderLineValue];
			std::cout << "(CategorizedLine): sliceSingleInterceptBorderLineA value is: " << pointAData.begin()->borderLineValue << std::endl;
			std::cout << "(CategorizedLine): sliceSingleInterceptBorderLineB value is: " << pointAData.rbegin()->borderLineValue << std::endl;

			generateCyclingDirectionForASliceSingleInterceptPointPrecise(sliceSingleInterceptBorderLineA,
				pointAData.begin()->borderLineValue,
				sliceSingleInterceptBorderLineB,
				pointAData.rbegin()->borderLineValue,
				emptyNormal,
				in_polyDebugLevel);

			std::cout << "::: >> current categorized line, point A: " << line.pointA.x << ", " << line.pointA.y << ", " << line.pointA.z << std::endl;
			std::cout << "::: >> current categorized line, point A border: " << line.pointABorder << std::endl;
			std::cout << "::: >> current categorized line, is point A on border: " << line.isPointAOnBorder << std::endl;

			std::cout << "::: >> current categorized line, point B: " << line.pointB.x << ", " << line.pointB.y << ", " << line.pointB.z << std::endl;
			std::cout << "::: >> current categorized line, point B border: " << line.pointBBorder << std::endl;
			std::cout << "::: >> current categorized line, is point B on border: " << line.isPointBOnBorder << std::endl;

			std::cout << "::: >> current categorized line, number of border lines: " << line.numberOfBorderLines << std::endl;

			/*
			int sillyLoop = 3;
			while (sillyLoop == 3)
			{

			}
			*/
		}
		else if (containsExtraData == false)
		{
			std::cout << "!!!!! NOTICE**** contains extra data is false..." << std::endl;

			glm::vec3 precisePoint = line.pointA;	// point A of an A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE should be what has the precise.
			std::cout << "Precise point is: " << precisePoint.x << ", " << precisePoint.y << ", " << precisePoint.z << std::endl;

			//std::cout << "::: >> current categorized line, point A: " << line.pointA.x << ", " << line.pointA.y << ", " << line.pointA.z << std::endl;
			//std::cout << "::: >> current categorized line, point A border: " << line.pointABorder << std::endl;
			//std::cout << "::: >> current categorized line, is point A on border: " << line.isPointAOnBorder << std::endl;

			//std::cout << "::: >> current categorized line, point B: " << line.pointB.x << ", " << line.pointB.y << ", " << line.pointB.z << std::endl;
			//std::cout << "::: >> current categorized line, point B border: " << line.pointBBorder << std::endl;
			//std::cout << "::: >> current categorized line, is point B on border: " << line.isPointBOnBorder << std::endl;

			//std::cout << "Empty normal of this categorized line is: " << emptyNormal.x << ", " << emptyNormal.y << ", " << emptyNormal.z << std::endl;

			std::map<int, int> returnMap;
			auto borderLinesBegin = in_borderLineArrayRef.begin();
			auto borderLinesEnd = in_borderLineArrayRef.end();
			for (; borderLinesBegin != borderLinesEnd; borderLinesBegin++)
			{
				//std::cout << "Border Line ID: " << borderLinesBegin->first << " | point A: " << borderLinesBegin->second.pointA.x << ", " << borderLinesBegin->second.pointA.y << ", " << borderLinesBegin->second.pointA.z
				//														   << " | point B: " << borderLinesBegin->second.pointB.x << ", " << borderLinesBegin->second.pointB.y << ", " << borderLinesBegin->second.pointB.z
				//														   << std::endl;

				if
				(
					(borderLinesBegin->second.pointA == precisePoint)
					||
					(borderLinesBegin->second.pointB == precisePoint)
				)
				{
					int currentMapIndex = int(returnMap.size());
					returnMap[currentMapIndex] = borderLinesBegin->first;	// insert the ID of the border line.
				}
			}

			generateCyclingDirectionForASliceSingleInterceptPointPrecise(in_borderLineArrayRef[returnMap[0]],
																		 returnMap[0],
																		 in_borderLineArrayRef[returnMap[1]],
																		 returnMap[1],
																		 emptyNormal,
																		 in_polyDebugLevel);

			//int falseVal = 3;
			//std::cin >> falseVal;


		}
	}
	return wasRunValid;
}

void CategorizedLine::generateCyclingDirectionForInterceptPointPrecise(SPolyBorderLines in_borderLineA, 
	                                                                   int in_borderLineAID, 
	                                                                   SPolyBorderLines in_borderLineB, 
	                                                                   int in_borderLineBID, 
	                                                                   glm::vec3 in_categorizedLineNormal,
	                                                                   PolyDebugLevel in_polyDebugLevel)
{
	SPolyBorderLines borderLineACopy = in_borderLineA;
	SPolyBorderLines borderLineBCopy = in_borderLineB;
	glm::vec3 emptyNormalCopy = in_categorizedLineNormal;

	PolyLogger preciseLogger;
	preciseLogger.setDebugLevel(in_polyDebugLevel);

	// is in_borderLineA's point B equal to in_borderLineB's point A?

	/*
	std::cout << ":::: BEGIN ******************** Cycling direction and Border determination; border will be on point A********************************" << std::endl;
	std::cout << "(Pre-Alter) || ::: >> current categorized line, point A: " << line.pointA.x << ", " << line.pointA.y << ", " << line.pointA.z << std::endl;
	std::cout << "(Pre-Alter) || ::: >> current categorized line, point A border: " << line.pointABorder << std::endl;
	std::cout << "(Pre-Alter) || ::: >> current categorized line, is point A on border: " << line.isPointAOnBorder << std::endl;
				  
	std::cout << "(Pre-Alter) || ::: >> current categorized line, point B: " << line.pointB.x << ", " << line.pointB.y << ", " << line.pointB.z << std::endl;
	std::cout << "(Pre-Alter) || ::: >> current categorized line, point B border: " << line.pointBBorder << std::endl;
	std::cout << "(Pre-Alter) || ::: >> current categorized line, is point B on border: " << line.isPointBOnBorder << std::endl;
	*/

	QuatRotationPoints rotationPoints;
	glm::vec3 pointToTranslateAgainst;


	std::cout << "BorderLineACopy, point A, is: " << borderLineACopy.pointA.x << ", " << borderLineACopy.pointA.y << ", " << borderLineACopy.pointA.z << std::endl;
	std::cout << "BorderLineACopy, point B, is: " << borderLineACopy.pointB.x << ", " << borderLineACopy.pointB.y << ", " << borderLineACopy.pointB.z << std::endl;

	std::cout << "border line A: " << borderLineBCopy.pointA.x << ", " << borderLineBCopy.pointA.y << ", " << borderLineBCopy.pointA.z << std::endl;
	std::cout << "border line B: " << borderLineBCopy.pointB.x << ", " << borderLineBCopy.pointB.y << ", " << borderLineBCopy.pointB.z << std::endl;

	if (borderLineACopy.pointB == borderLineBCopy.pointA)
	{
		//std::cout << "Line A links with Line B, at Line A's point B. " << std::endl;
		rotationPoints.insertPointRefs(&borderLineACopy.pointA, &borderLineACopy.pointB, &borderLineBCopy.pointB);
		pointToTranslateAgainst = borderLineACopy.pointB;
	}
	// otherwise, it's the other way around.
	else if (borderLineBCopy.pointB == borderLineACopy.pointA)
	{
		//std::cout << "Line B links with Line A, at Line B's point B. " << std::endl;
		rotationPoints.insertPointRefs(&borderLineBCopy.pointA, &borderLineBCopy.pointB, &borderLineACopy.pointB);
		pointToTranslateAgainst = borderLineBCopy.pointB;
	}

	// do a translation check.
	PointTranslationCheck translationChecker;
	translationChecker.performCheck(pointToTranslateAgainst);
	if (translationChecker.requiresTranslation == 1)
	{
		rotationPoints.applyTranslation(translationChecker.getTranslationValue());
	}
	//std::cout << ">>>> Rotation manager initialized (1)...." << std::endl;


	// now, add the normal at the end.
	rotationPoints.insertPointRefs(&emptyNormalCopy);
	//std::cout << ">>>> Rotation manager initialized (2)...." << std::endl;

	//std::cout << ":::: Printing points: " << std::endl;
	//rotationPoints.printPoints();
	if (preciseLogger.isLoggingSet())
	{
		preciseLogger.log("(CategorizedLine): generateCyclingDirectionForInterceptPointPrecise, printing points in rotationPoints.", "\n");
		rotationPoints.printPoints();
		preciseLogger.waitForDebugInput();
	}

	//QuatRotationManager rotationManager;
	//rotationManager.initializeAndRunForFindingBorderLine(&rotationPoints);
	//rotationManager.initializeAndRunForCyclingDirectionFinderV2(&rotationPoints);
	QMVoidFindCyclingDirection directionFinder;
	directionFinder.solve(&rotationPoints, in_polyDebugLevel);

	//std::cout << ">>>> Rotation manager initialized (3)...." << std::endl;

	// determine which point it is that is positive y (check the first and third points.)
	glm::vec3 candidateOne = rotationPoints.getPointByIndex(0);
	glm::vec3 candidateTwo = rotationPoints.getPointByIndex(2);
	glm::vec3 selectedPoint;
	if (candidateOne.y > 0)
	{
		selectedPoint = candidateOne;
	}
	else if (candidateTwo.y > 0)
	{
		selectedPoint = candidateTwo;
	}

	// take the selected point, find out where it belongs; then set the appropriate forward/reverse IDs.
	int forwardID, reverseID;

	/*
	if
	(
		(borderLineACopy.pointA == selectedPoint)
		||
		(borderLineACopy.pointB == selectedPoint)
	)
	{
		std::cout << "Border Line A is the FORWARD line. " << std::endl;

		forwardID = in_borderLineAID;
		reverseID = in_borderLineBID;
	}

	else if
	(
		(borderLineBCopy.pointA == selectedPoint)
		||
		(borderLineBCopy.pointB == selectedPoint)
	)
	{
		std::cout << "Border Line B is the FORWARD line. " << std::endl;

		forwardID = in_borderLineBID;
		reverseID = in_borderLineAID;
	}
	*/

	/*
	if (line.isPointAOnBorder == 1)
	{
		std::cout << ":::NOTICE: point A flagged as on border. " << std::endl;
	}
	if (line.isPointBOnBorder == 1)
	{
		std::cout << ":::NOTICE: point B flagged as on border. " << std::endl;
	}
	*/

	if
		(
		(borderLineACopy.pointA == selectedPoint)
			)
	{
		//std::cout << "Border Line A is the FORWARD line, going towards point A (CyclingDirection REVERSE)" << std::endl;

		direction = CyclingDirection::REVERSE;
		
		// always set point A as the "one on the border"
		line.isPointAOnBorder = 1;
		line.pointABorder = in_borderLineAID;
		line.pointA = in_borderLineA.pointB;	// we go towards point A, so we'd store point B of Line A.

		forwardID = in_borderLineAID;
		reverseID = in_borderLineBID;
	}
	else if
		(
		(borderLineACopy.pointB == selectedPoint)
			)
	{
		//std::cout << "Border Line A is the FORWARD line, going towards point B (CyclingDirection FORWARD)" << std::endl;

		direction = CyclingDirection::FORWARD;

		line.isPointAOnBorder = 1;
		line.pointABorder = in_borderLineAID;
		line.pointA = in_borderLineA.pointA;	// we go towards point B, so we'd store point A of Line A.

		forwardID = in_borderLineAID;
		reverseID = in_borderLineBID;
	}
	else if
		(
		(borderLineBCopy.pointA == selectedPoint)
			)
	{
		//std::cout << "Border Line B is the FORWARD line, going towards point A (CyclingDirection REVERSE)" << std::endl;

		direction = CyclingDirection::REVERSE;

		line.isPointAOnBorder = 1;
		line.pointABorder = in_borderLineBID;
		line.pointA = in_borderLineB.pointB;	// we go towards point A, so we'd store point B of Line B.

		forwardID = in_borderLineBID;
		reverseID = in_borderLineAID;
	}
	else if
		(
		(borderLineBCopy.pointB == selectedPoint)
			)
	{
		//std::cout << "Border Line B is the FORWARD line, going towards point B (CyclingDirection FORWARD)" << std::endl;

		direction = CyclingDirection::FORWARD;

		line.isPointAOnBorder = 1;
		line.pointABorder = in_borderLineBID;
		line.pointA = in_borderLineB.pointA;	// we go towards point B, so we'd store point A of Line B.

		forwardID = in_borderLineBID;
		reverseID = in_borderLineAID;
	}

	// reset B border metadata, for clarity (also not needed.)
	line.isPointBOnBorder = 0;
	line.pointBBorder = 0;

	//std::cout << ">>::: Forward ID: " << forwardID << std::endl;
	//std::cout << ">>::: Reverse ID: " << reverseID << std::endl;

	/*
	std::cout << "|| ::: >> current categorized line, point A: " << line.pointA.x << ", " << line.pointA.y << ", " << line.pointA.z << std::endl;
	std::cout << "|| ::: >> current categorized line, point A border: " << line.pointABorder << std::endl;
	std::cout << "|| ::: >> current categorized line, is point A on border: " << line.isPointAOnBorder << std::endl;
				  
	std::cout << "|| ::: >> current categorized line, point B: " << line.pointB.x << ", " << line.pointB.y << ", " << line.pointB.z << std::endl;
	std::cout << "|| ::: >> current categorized line, point B border: " << line.pointBBorder << std::endl;
	std::cout << "|| ::: >> current categorized line, is point B on border: " << line.isPointBOnBorder << std::endl;

	std::cout << "Point A of line: " << line.pointA.x << ", " << line.pointA.y << ", " << line.pointA.z << std::endl;

	std::cout << ":::: END ******************** Cycling direction and Border determination; border will be on point A********************************" << std::endl;
	*/
}

void CategorizedLine::generateCyclingDirectionForASliceSingleInterceptPointPrecise(SPolyBorderLines in_borderLineA,
	int in_borderLineAID,
	SPolyBorderLines in_borderLineB,
	int in_borderLineBID,
	glm::vec3 in_categorizedLineNormal,
	PolyDebugLevel in_polyDebugLevel)
{
	SPolyBorderLines borderLineACopy = in_borderLineA;
	SPolyBorderLines borderLineBCopy = in_borderLineB;
	glm::vec3 emptyNormalCopy = in_categorizedLineNormal;

	PolyLogger preciseLogger;
	preciseLogger.setDebugLevel(in_polyDebugLevel);

	// is in_borderLineA's point B equal to in_borderLineB's point A?

	/*
	std::cout << ":::: BEGIN ******************** Cycling direction and Border determination; border will be on point A********************************" << std::endl;
	std::cout << "(Pre-Alter) || ::: >> current categorized line, point A: " << line.pointA.x << ", " << line.pointA.y << ", " << line.pointA.z << std::endl;
	std::cout << "(Pre-Alter) || ::: >> current categorized line, point A border: " << line.pointABorder << std::endl;
	std::cout << "(Pre-Alter) || ::: >> current categorized line, is point A on border: " << line.isPointAOnBorder << std::endl;

	std::cout << "(Pre-Alter) || ::: >> current categorized line, point B: " << line.pointB.x << ", " << line.pointB.y << ", " << line.pointB.z << std::endl;
	std::cout << "(Pre-Alter) || ::: >> current categorized line, point B border: " << line.pointBBorder << std::endl;
	std::cout << "(Pre-Alter) || ::: >> current categorized line, is point B on border: " << line.isPointBOnBorder << std::endl;
	*/

	QuatRotationPoints rotationPoints;
	glm::vec3 pointToTranslateAgainst;

	std::cout << "(A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE)" << std::endl;
	std::cout << "||||||||||||||||||| BEFORE APPLICATION **************" << std::endl;
	std::cout << "||||||| Border Line A ID: " << in_borderLineAID << std::endl;
	std::cout << "||||||| Border Line B ID: " << in_borderLineBID << std::endl;
	std::cout << "||||||| Empty normal is: " << emptyNormalCopy.x << ", " << emptyNormalCopy.y << ", " << emptyNormalCopy.z << std::endl;
	std::cout << "(A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE) ---> Line, Point A: " << line.pointA.x << ", " << line.pointA.y << ", " << line.pointA.z << std::endl;
	std::cout << "(A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE) ---> Line, Point B: " << line.pointB.x << ", " << line.pointB.y << ", " << line.pointB.z << std::endl;

	std::cout << "BorderLineACopy, point A, is: " << borderLineACopy.pointA.x << ", " << borderLineACopy.pointA.y << ", " << borderLineACopy.pointA.z << std::endl;
	std::cout << "BorderLineACopy, point B, is: " << borderLineACopy.pointB.x << ", " << borderLineACopy.pointB.y << ", " << borderLineACopy.pointB.z << std::endl;

	std::cout << "border line A: " << borderLineBCopy.pointA.x << ", " << borderLineBCopy.pointA.y << ", " << borderLineBCopy.pointA.z << std::endl;
	std::cout << "border line B: " << borderLineBCopy.pointB.x << ", " << borderLineBCopy.pointB.y << ", " << borderLineBCopy.pointB.z << std::endl;

	if (borderLineACopy.pointB == borderLineBCopy.pointA)
	{
		std::cout << "Line A links with Line B, at Line A's point B. " << std::endl;
		rotationPoints.insertPointRefs(&borderLineACopy.pointA, &borderLineACopy.pointB, &borderLineBCopy.pointB);
		pointToTranslateAgainst = borderLineACopy.pointB;
	}
	// otherwise, it's the other way around.
	else if (borderLineBCopy.pointB == borderLineACopy.pointA)
	{
		std::cout << "Line B links with Line A, at Line B's point B. " << std::endl;
		rotationPoints.insertPointRefs(&borderLineBCopy.pointA, &borderLineBCopy.pointB, &borderLineACopy.pointB);
		pointToTranslateAgainst = borderLineBCopy.pointB;
	}

	// do a translation check.
	PointTranslationCheck translationChecker;
	translationChecker.performCheck(pointToTranslateAgainst);
	if (translationChecker.requiresTranslation == 1)
	{
		rotationPoints.applyTranslation(translationChecker.getTranslationValue());
	}
	//std::cout << ">>>> Rotation manager initialized (1)...." << std::endl;


	// now, add the normal at the end.
	rotationPoints.insertPointRefs(&emptyNormalCopy);
	//std::cout << ">>>> Rotation manager initialized (2)...." << std::endl;

	//std::cout << ":::: Printing points: " << std::endl;
	//rotationPoints.printPoints();
	if (preciseLogger.isLoggingSet())
	{
		preciseLogger.log("(CategorizedLine): generateCyclingDirectionForInterceptPointPrecise, printing points in rotationPoints.", "\n");
		rotationPoints.printPoints();
		preciseLogger.waitForDebugInput();
	}

	//QuatRotationManager rotationManager;
	//rotationManager.initializeAndRunForFindingBorderLine(&rotationPoints);
	//rotationManager.initializeAndRunForCyclingDirectionFinderV2(&rotationPoints);
	QMVoidFindCyclingDirection directionFinder;
	directionFinder.solve(&rotationPoints, in_polyDebugLevel);

	//std::cout << ">>>> Rotation manager initialized (3)...." << std::endl;

	// determine which point it is that is positive y (check the first and third points.)
	glm::vec3 candidateOne = rotationPoints.getPointByIndex(0);
	glm::vec3 candidateTwo = rotationPoints.getPointByIndex(2);
	glm::vec3 adjustedNormal = rotationPoints.getPointByIndex(3);

	std::cout << "Candidate One is: " << candidateOne.x << ", " << candidateOne.y << ", " << candidateOne.z << std::endl;
	std::cout << "Candidate Two is: " << candidateTwo.x << ", " << candidateTwo.y << ", " << candidateTwo.z << std::endl;
	std::cout << "Adjusted normal was: " << adjustedNormal.x << ", " << adjustedNormal.y << ", " << adjustedNormal.z << std::endl;
	
	glm::vec3 selectedPoint;
	if (candidateOne.y > 0)
	{
		std::cout << ">>>> Selected CandidateOne. " << std::endl;
		selectedPoint = candidateOne;
	}
	else if (candidateTwo.y > 0)
	{
		std::cout << ">>>> Selected CandidateTwo. " << std::endl;
		selectedPoint = candidateTwo;
	}
	
	else
	{
		std::cout << "!!!!! NOTICE: couldn't find any point above y, doing special logic. " << std::endl;
		glm::vec3 candidateOneBase = candidateOne;
		glm::vec3 candidateOneAdjusted = candidateOneBase;
		candidateOneAdjusted.y = 0;
		float candidateOneDistanceToY0 = glm::distance(candidateOneBase, candidateOneAdjusted);
		std::cout << "Candidate one y dist: " << candidateOneDistanceToY0 << std::endl;


		glm::vec3 candidateTwoBase = candidateTwo;
		glm::vec3 candidateTwoAdjusted = candidateTwoBase;
		candidateTwoAdjusted.y = 0;
		float candidateTwoDistanceToY0 = glm::distance(candidateTwoBase, candidateTwoAdjusted);
		std::cout << "Candidate two y dist: " << candidateTwoDistanceToY0 << std::endl;

		float closestPoint = std::min(candidateOneDistanceToY0, candidateTwoDistanceToY0);

		if (closestPoint == candidateOneDistanceToY0)
		{
			selectedPoint = candidateOne;
		}
		else if (closestPoint == candidateTwoDistanceToY0)
		{
			selectedPoint = candidateTwo;
		}

	}
	

	// take the selected point, find out where it belongs; then set the appropriate forward/reverse IDs.

	/*
	if
	(
		(borderLineACopy.pointA == selectedPoint)
		||
		(borderLineACopy.pointB == selectedPoint)
	)
	{
		std::cout << "Border Line A is the FORWARD line. " << std::endl;

		forwardID = in_borderLineAID;
		reverseID = in_borderLineBID;
	}

	else if
	(
		(borderLineBCopy.pointA == selectedPoint)
		||
		(borderLineBCopy.pointB == selectedPoint)
	)
	{
		std::cout << "Border Line B is the FORWARD line. " << std::endl;

		forwardID = in_borderLineBID;
		reverseID = in_borderLineAID;
	}
	*/

	/*
	if (line.isPointAOnBorder == 1)
	{
		std::cout << ":::NOTICE: point A flagged as on border. " << std::endl;
	}
	if (line.isPointBOnBorder == 1)
	{
		std::cout << ":::NOTICE: point B flagged as on border. " << std::endl;
	}
	*/

	if
	(
		(borderLineACopy.pointA == selectedPoint)
	)
	{
		//std::cout << "(A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE) Border Line A is the FORWARD line, going towards point A (CyclingDirection REVERSE)" << std::endl;

		direction = CyclingDirection::REVERSE;

		// always set point A as the "one on the border"
		line.isPointAOnBorder = 1;
		line.pointABorder = in_borderLineAID;
		line.pointA = in_borderLineA.pointB;	// we go towards point A, so we'd store point B of Line A.
	}
	else if
	(
		(borderLineACopy.pointB == selectedPoint)
	)
	{
		//std::cout << "(A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE) Border Line A is the FORWARD line, going towards point B (CyclingDirection FORWARD)" << std::endl;

		direction = CyclingDirection::FORWARD;

		line.isPointAOnBorder = 1;
		line.pointABorder = in_borderLineAID;
		line.pointA = in_borderLineA.pointA;	// we go towards point B, so we'd store point A of Line A.

	}
	else if
	(
		(borderLineBCopy.pointA == selectedPoint)
	)
	{
		//std::cout << "(A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE) Border Line B is the FORWARD line, going towards point A (CyclingDirection REVERSE)" << std::endl;

		direction = CyclingDirection::REVERSE;

		line.isPointAOnBorder = 1;
		line.pointABorder = in_borderLineBID;
		line.pointA = in_borderLineB.pointB;	// we go towards point A, so we'd store point B of Line B.

	}
	else if
	(
		(borderLineBCopy.pointB == selectedPoint)
	)
	{
		//std::cout << "(A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE) Border Line B is the FORWARD line, going towards point B (CyclingDirection FORWARD)" << std::endl;

		direction = CyclingDirection::FORWARD;

		line.isPointAOnBorder = 1;
		line.pointABorder = in_borderLineBID;
		line.pointA = in_borderLineB.pointA;	// we go towards point B, so we'd store point A of Line B.

	}

	
	if (direction == CyclingDirection::FORWARD)
	{
		direction = CyclingDirection::REVERSE;
	}
	else if (direction == CyclingDirection::REVERSE)
	{
		direction = CyclingDirection::FORWARD;
	}
	
	//std::cout << ">>::: Forward ID: " << forwardID << std::endl;
	//std::cout << ">>::: Reverse ID: " << reverseID << std::endl;

	/*
	std::cout << "|| ::: >> current categorized line, point A: " << line.pointA.x << ", " << line.pointA.y << ", " << line.pointA.z << std::endl;
	std::cout << "|| ::: >> current categorized line, point A border: " << line.pointABorder << std::endl;
	std::cout << "|| ::: >> current categorized line, is point A on border: " << line.isPointAOnBorder << std::endl;

	std::cout << "|| ::: >> current categorized line, point B: " << line.pointB.x << ", " << line.pointB.y << ", " << line.pointB.z << std::endl;
	std::cout << "|| ::: >> current categorized line, point B border: " << line.pointBBorder << std::endl;
	std::cout << "|| ::: >> current categorized line, is point B on border: " << line.isPointBOnBorder << std::endl;

	std::cout << "Point A of line: " << line.pointA.x << ", " << line.pointA.y << ", " << line.pointA.z << std::endl;

	std::cout << ":::: END ******************** Cycling direction and Border determination; border will be on point A********************************" << std::endl;
	*/
}

void CategorizedLine::testFunction()
{
	std::cout << "!! Test!! " << std::endl;
}


void CategorizedLine::convertLineToSlice(IntersectionLine in_slicingLine)
{
	type = IntersectionType::A_SLICE;
	line.numberOfBorderLines = 2;
	//std::cout << "Converting line to slice..." << std::endl;
	line.isPointAOnBorder = 1;
	line.pointA = in_slicingLine.pointA;
	line.pointABorder = in_slicingLine.pointABorder;
	line.isPointBOnBorder = 1;
	line.pointB = in_slicingLine.pointB;
	line.pointBBorder = in_slicingLine.pointBBorder;
	//line.intersectedSecondaryID = in_slicingLine.intersectedSecondaryID;
}

IRPointType CategorizedLine::checkIfPointIsInLine(glm::vec3 in_point)
{
	IRPointType returnType = IRPointType::NEITHER;	// default value
	if
		(
			line.pointA.x == in_point.x
			&&
			line.pointA.y == in_point.y
			&&
			line.pointA.z == in_point.z
			)
	{
		returnType = IRPointType::POINT_A;
	}

	else if
		(
			line.pointB.x == in_point.x
			&&
			line.pointB.y == in_point.y
			&&
			line.pointB.z == in_point.z
			)
	{
		returnType = IRPointType::POINT_B;
	}
	return returnType;
}

IRPointType CategorizedLine::checkIfPointIsNearbyPointInLine(glm::vec3 in_point, float in_searchBoxHalfDiameter)
{
	IRPointType returnType = IRPointType::NEITHER;	// default value

	float begin_x = in_point.x - in_searchBoxHalfDiameter;
	float end_x = in_point.x + in_searchBoxHalfDiameter;

	float begin_y = in_point.y - in_searchBoxHalfDiameter;
	float end_y = in_point.y + in_searchBoxHalfDiameter;

	float begin_z = in_point.z - in_searchBoxHalfDiameter;
	float end_z = in_point.z + in_searchBoxHalfDiameter;

	std::cout << "--------------" << std::endl;
	std::cout << "begin_x = " << begin_x << " | end_x = " << end_x << std::endl;
	std::cout << "begin_y = " << begin_y << " | end_y = " << end_y << std::endl;
	std::cout << "begin_z = " << begin_z << " | end_x = " << end_z << std::endl;

	std::cout << "line.pointA: " << line.pointA.x << ", " << line.pointA.y << ", " << line.pointA.z << std::endl;
	std::cout << "line.pointB: " << line.pointB.x << ", " << line.pointB.y << ", " << line.pointB.z << std::endl;

	if
	(
		(line.pointA.x >= begin_x)
		&&
		(line.pointA.x <= end_x)

		&&

		(line.pointA.y >= begin_y)
		&&
		(line.pointA.y <= end_y)

		&&

		(line.pointA.z >= begin_z)
		&&
		(line.pointA.z <= end_z)
	)
	{
		returnType = IRPointType::POINT_A;
	}

	else if
	(
		(line.pointB.x >= begin_x)
		&&		   
		(line.pointB.x <= end_x)
				   
		&&		   
				   
		(line.pointB.y >= begin_y)
		&&		   
		(line.pointB.y <= end_y)
				   
		&&		   
				   
		(line.pointB.z >= begin_z)
		&&		   
		(line.pointB.z <= end_z)
	)
	{
		returnType = IRPointType::POINT_B;
	}


	return returnType;
}

void CategorizedLine::setEmptyNormal(glm::vec3 in_emptyNormal)
{
	emptyNormal = in_emptyNormal;
}

void CategorizedLine::createCategorizedLineOptionals()
{
	containsExtraData = true;	// needs to be set to true before entering data.
	extraData.reset(new CategorizedLineOptionals);	// initialize the pointer.
}

void CategorizedLine::insertOptionalDataForPoint(IRPointType in_irPointType, std::vector<FusedPointSubData> in_fusedPointSubDataVector)
{
	extraData->insertFusedPointSubDataForPoint(in_irPointType, in_fusedPointSubDataVector);
}

std::string CategorizedLine::getIntersectionTypeString()
{
	std::string returnString;
	switch (type)
	{
		case (IntersectionType::NONE): { returnString = "NONE"; break; }
		case (IntersectionType::A_SLICE): { returnString = "A_SLICE"; break;}
		case (IntersectionType::A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE): { returnString = "A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE"; break; }
		case (IntersectionType::INTERCEPTS_POINT_PRECISE): { returnString = "INTERCEPTS_POINT_PRECISE"; break;}
		case (IntersectionType::INTERCEPTS_POINT_PRECISE_UNSOLVED_COPLANAR): { returnString = "INTERCEPTS_POINT_PRECISE_UNSOLVED_COPLANAR";  break;}
		case (IntersectionType::NON_BOUND): { returnString = "NON_BOUND"; break; }
		case (IntersectionType::PARTIAL_BOUND): { returnString = "PARTIAL_BOUND"; break; }
	}
	return returnString;
}