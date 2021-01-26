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

void CategorizedLine::determineCyclingDirection(std::map<int, SPolyBorderLines> in_borderLineArrayRef)
//void CategorizedLine::determineCyclingDirection(SPolyBorderLines in_borderLineArrayRef)
{
	//std::cout << "Determining cycling direction for this line; the line's parent SPoly is: " << parentPoly << std::endl;

	// logic for partial lines.
	if (type == IntersectionType::PARTIAL_BOUND)
	{
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

		rotationPoints.pointsRefVector.push_back(&borderLineCopy.pointA);
		rotationPoints.pointsRefVector.push_back(&borderPoint);
		rotationPoints.pointsRefVector.push_back(&borderLineCopy.pointB);

		PointTranslationCheck pointTranslator;
		pointTranslator.performCheck(rotationPoints.getPointByIndex(1));	// get the "borderPoint", translate to it if necessary.
		if (pointTranslator.requiresTranslation == 1)
		{
			rotationPoints.applyTranslation(pointTranslator.getTranslationValue());
		}

		// now, add the normal at the end.
		rotationPoints.pointsRefVector.push_back(&emptyNormalCopy);

		QuatRotationManager rotationManager;
		//rotationManager.initializeAndRunForFindingBorderLine(&rotationPoints);
		rotationManager.initializeAndRunForCyclingDirectionFinderV2(&rotationPoints);

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

		rotationPoints.pointsRefVector.push_back(&borderLineCopy.pointA);
		rotationPoints.pointsRefVector.push_back(&borderPoint);
		rotationPoints.pointsRefVector.push_back(&borderLineCopy.pointB);

		PointTranslationCheck pointTranslator;
		pointTranslator.performCheck(rotationPoints.getPointByIndex(1));	// get the "borderPoint", translate to it if necessary.
		if (pointTranslator.requiresTranslation == 1)
		{
			rotationPoints.applyTranslation(pointTranslator.getTranslationValue());
		}

		// now, add the normal at the end.
		rotationPoints.pointsRefVector.push_back(&emptyNormalCopy);

		QuatRotationManager rotationManager;
		//rotationManager.initializeAndRunForFindingBorderLine(&rotationPoints);
		rotationManager.initializeAndRunForCyclingDirectionFinderV2(&rotationPoints);

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

		int someVal = 3;
		std::cin >> someVal;
	}

	// logic for intercepts_point_precise
	else if (type == IntersectionType::INTERCEPTS_POINT_PRECISE)
	{

		// fetching border lines...
		SPolyBorderLines borderLineA = in_borderLineArrayRef[line.pointABorder];
		SPolyBorderLines borderLineB = in_borderLineArrayRef[line.pointBBorder];

		// will set the direction of the line...
		generateCyclingDirectionForInterceptPointPrecise(borderLineA, line.pointABorder, borderLineB, line.pointBBorder, emptyNormal);
	}
	
}

void CategorizedLine::generateCyclingDirectionForInterceptPointPrecise(SPolyBorderLines in_borderLineA, int in_borderLineAID, SPolyBorderLines in_borderLineB, int in_borderLineBID, glm::vec3 in_categorizedLineNormal)
{
	SPolyBorderLines borderLineACopy = in_borderLineA;
	SPolyBorderLines borderLineBCopy = in_borderLineB;
	glm::vec3 emptyNormalCopy = in_categorizedLineNormal;

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


	//std::cout << "BorderLineACopy, point A, is: " << borderLineACopy.pointA.x << ", " << borderLineACopy.pointA.y << ", " << borderLineACopy.pointA.z << std::endl;
	//std::cout << "BorderLineACopy, point B, is: " << borderLineACopy.pointB.x << ", " << borderLineACopy.pointB.y << ", " << borderLineACopy.pointB.z << std::endl;

	//std::cout << "border line A: " << borderLineBCopy.pointA.x << ", " << borderLineBCopy.pointA.y << ", " << borderLineBCopy.pointA.z << std::endl;
	//std::cout << "border line B: " << borderLineBCopy.pointB.x << ", " << borderLineBCopy.pointB.y << ", " << borderLineBCopy.pointB.z << std::endl;

	if (borderLineACopy.pointB == borderLineBCopy.pointA)
	{
		//std::cout << "Line A links with Line B, at Line A's point B. " << std::endl;

		rotationPoints.pointsRefVector.push_back(&borderLineACopy.pointA);
		rotationPoints.pointsRefVector.push_back(&borderLineACopy.pointB);
		rotationPoints.pointsRefVector.push_back(&borderLineBCopy.pointB);

		pointToTranslateAgainst = borderLineACopy.pointB;

		//rotationPoints.pointsRefVector.push
	}
	// otherwise, it's the other way around.
	else if (borderLineBCopy.pointB == borderLineACopy.pointA)
	{
		//std::cout << "Line B links with Line A, at Line B's point B. " << std::endl;

		rotationPoints.pointsRefVector.push_back(&borderLineBCopy.pointA);
		rotationPoints.pointsRefVector.push_back(&borderLineBCopy.pointB);
		rotationPoints.pointsRefVector.push_back(&borderLineACopy.pointB);

		//borderLineBCopy.pointB

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
	rotationPoints.pointsRefVector.push_back(&emptyNormalCopy);
	//std::cout << ">>>> Rotation manager initialized (2)...." << std::endl;

	//std::cout << ":::: Printing points: " << std::endl;
	//rotationPoints.printPoints();

	QuatRotationManager rotationManager;
	//rotationManager.initializeAndRunForFindingBorderLine(&rotationPoints);
	rotationManager.initializeAndRunForCyclingDirectionFinderV2(&rotationPoints);

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

void CategorizedLine::setEmptyNormal(glm::vec3 in_emptyNormal)
{
	emptyNormal = in_emptyNormal;
}