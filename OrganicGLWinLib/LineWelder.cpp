#include "stdafx.h"
#include "LineWelder.h"

void LineWelder::startWelding()
{
	// Step 1: get the first line, in the first cleave sequence, and determine how many border lines it has. 
	auto cleaveBegin = sPolyRef->cleaveMap.begin();
	auto sequenceBegin = cleaveBegin->second.cleavingLines.begin();
	auto sequenceEnd = cleaveBegin->second.cleavingLines.rbegin();
	BorderLinePointPair pointPair;		// point A of this pair is where the welding will begin from; point B of this pair is the beginning point of the WeldedLine, and point A is the end point of the WeldedLine.
	if (sequenceBegin->second.line.numberOfBorderLines == 1)		// if there are multiple CategorizedLines in the cleave sequence, the first line in the sequence
																	// will have exactly one border line. The end of the sequence will have another border line.
	{
		pointPair = sequenceBegin->second.line.getBorderLinePointPair();
		currentBorderLineID = sequenceBegin->second.line.getBorderLineIDFromSingularBorderLineCount();
		endingBorderLineID = sequenceEnd->second.line.getBorderLineIDFromSingularBorderLineCount();

	}
	else if (sequenceBegin->second.line.numberOfBorderLines == 2)	// if there is just one line, it'll have two border lines; now we must
																	// get the borders from the pointA and pointB of this line.
	{
		BorderLineIDPair retrievedPair = sequenceBegin->second.line.getBorderLineIDPair();
		pointPair = sequenceBegin->second.line.getBorderLinePointPair();
		currentBorderLineID = retrievedPair.pointABorderLineID;
		endingBorderLineID = retrievedPair.pointBBorderLineID;
	}

	std::cout << "******** printing claave lines in the first sequence: " << std::endl;
	auto testSequenceBegin = cleaveBegin->second.cleavingLines.begin();
	auto testSequenceEnd = cleaveBegin->second.cleavingLines.end();
	for (; testSequenceBegin != testSequenceEnd; testSequenceBegin++)
	{
		std::cout << "Printing line " << testSequenceBegin->first << std::endl;
		std::cout << testSequenceBegin->second.line.pointA.x << ", " << testSequenceBegin->second.line.pointA.y << ", " << testSequenceBegin->second.line.pointA.z << std::endl;
		std::cout << testSequenceBegin->second.line.pointB.x << ", " << testSequenceBegin->second.line.pointB.y << ", " << testSequenceBegin->second.line.pointB.z << std::endl;
	}

	// Step 2: find the direction to cycle towards for the currentBorderLineID; i.e., do we start the cycling in the direction of point A or point B of this border line?
	// Remember, if the MassManipulationMode is DESTRUCTION, we must invert the empty normal of the CategorizedLine we are using to find the direction.
	// Also remember, we are going to use COPIES of the border lines and sequences, not the actual refs, when we need to find the CyclingDirection. (may need to change function parameters for this)
	BorderLineCycler lineCycler(sPolyRef);

	// send copies of the border line, and the corresponding categorized line in the CleaveMap that intercepted the border line, to find the direction.
	CyclingDirection foundDirection = lineCycler.findCyclingDirection(sPolyRef->borderLines[currentBorderLineID], currentBorderLineID, sequenceBegin->second, sPolyRef->massManipulationSetting);
	findWeldingLines(currentBorderLineID, cleaveBegin->first, sequenceBegin->first, foundDirection, pointPair); // pass in: the border line ID, the ID of the CleaveSequence in the CleaveMap, 
																										   // the ID of the CategorizedLine in the CleaveSequence, the direction

	// use the unused point of the categorized line to determine how to quat to Z = 0 (Z-planar).
}

void LineWelder::findWeldingLines(int in_startingBorderLineID, int in_startingCleaveSequenceID, int in_categorizedLineInCleaveSequenceID, CyclingDirection in_cyclingDirection, BorderLinePointPair in_beginningPointPair)
{
	// the point we start welding from, is equal to point A of the passed in parameter, in_beginningPointPair.
	glm::vec3 weldingStartPoint = in_beginningPointPair.pointA;
	std::cout << "Welding start point is: " << weldingStartPoint.x << ", " << weldingStartPoint.y << ", " << weldingStartPoint.z << std::endl;

	// determine the CleaveSequenceMeta for the CleaveSequence we are dealing with.
	auto cleaveBegin = sPolyRef->cleaveMap.begin();
	CleaveSequenceMeta foundSequenceMeta = cleaveBegin->second.getCleaveSequenceMeta();

	// find any neighboring cleave lines that exist on the same border line, if they exist. If they do exist, fetch the next
	NeighboringCleaveSequenceFinder nextCleaveSequenceFinder(in_startingBorderLineID, &sPolyRef->borderLines[in_startingBorderLineID], &sPolyRef->cleaveMap, in_cyclingDirection);
}