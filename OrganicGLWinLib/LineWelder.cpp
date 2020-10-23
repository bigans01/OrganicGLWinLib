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
		currentBorderLineID = sequenceBegin->second.line.getBorderLineIDFromSingularBorderLineCount();		// retrieve the border line to start at, from the first CleavingLine in the first CleavingSequence.
		endingBorderLineID = sequenceEnd->second.line.getBorderLineIDFromSingularBorderLineCount();

	}
	else if (sequenceBegin->second.line.numberOfBorderLines == 2)	// if there is just one line, it'll have two border lines; now we must
																	// get the borders from the pointA and pointB of this line.
	{
		BorderLineIDPair retrievedPair = sequenceBegin->second.line.getBorderLineIDPair();
		pointPair = sequenceBegin->second.line.getBorderLinePointPair();
		currentBorderLineID = retrievedPair.pointABorderLineID;												// retrieve the border line to start at, from the first CleavingLine in the first CleavingSequence.
		endingBorderLineID = retrievedPair.pointBBorderLineID;
	}

	std::cout << "******** printing cleave lines in the first sequence: " << std::endl;
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
	// store the direction in the LineWelder's foundDirection, so that we can use it throughout the process (should only ever need to find it once, and can be used 
	// without ever having to change it.)
	foundDirection = lineCycler.findCyclingDirection(sPolyRef->borderLines[currentBorderLineID], currentBorderLineID, sequenceBegin->second, sPolyRef->massManipulationSetting);

	getCleaveSequenceCandidateListMap();	// create the candidates map for each border line, by calling on the SPoly to do it
	getCleaveSequenceMetaTracker();			// builds the meta tracker, so that we may pass it to the NeighboringCleaveSequenceFinder

	// start welding from the first categorized line, in the first cleave, in the border line that the first categorized line is on.


	// for the below, pass in: 
	// -the border line ID
	// -the leading point
	// -a reference to the corresponding CleaveSequenceCandidateList
	// -the ID of the cleave sequence that the finder is starting its run from
	findRemainingWeldingLines(currentBorderLineID, pointPair.pointA, &candidateListMap.candidateMap[currentBorderLineID], cleaveBegin->first);	
	
																															

	//findWeldingLines(currentBorderLineID, cleaveBegin->first, sequenceBegin->first, foundDirection, pointPair); // pass in: the border line ID, the ID of the CleaveSequence in the CleaveMap, 
																										   // the ID of the CategorizedLine in the CleaveSequence, the direction


	// use the unused point of the categorized line to determine how to quat to Z = 0 (Z-planar).
}

void LineWelder::getCleaveSequenceCandidateListMap()
{
	/*
	int numberOfBorderLines = sPolyRef->numberOfBorderLines;
	for (int x = 0; x < numberOfBorderLines; x++)
	{
		std::cout << "|||| Candidate list for border line " << x << ": " << std::endl;
		SPolyBorderLines* borderLineRef = &sPolyRef->borderLines[x];
		auto recordsBegin = borderLineRef->intersectRecorder.records.begin();
		auto recordsEnd = borderLineRef->intersectRecorder.records.end();
		//candidateListMap.candidateMap[x].
		for (; recordsBegin != recordsEnd; recordsBegin++)
		{
			std::cout << "> " << recordsBegin->first << std::endl;
			candidateListMap.candidateMap[x].insertCandidate(recordsBegin->first);
		}
	}
	*/
	candidateListMap = sPolyRef->buildCleaveSequenceCandidateListMap();
}

void LineWelder::getCleaveSequenceMetaTracker()
{
	metaTracker = sPolyRef->buildCleaveSequenceMetaTracker();
}

void LineWelder::findWeldingLines(int in_startingBorderLineID, int in_startingCleaveSequenceID, int in_categorizedLineInCleaveSequenceID, CyclingDirection in_cyclingDirection, BorderLinePointPair in_beginningPointPair)
{
	// the point we start welding from, is equal to point A of the passed in parameter, in_beginningPointPair.
	glm::vec3 weldingStartPoint = in_beginningPointPair.pointA;
	std::cout << "Welding start point is: " << weldingStartPoint.x << ", " << weldingStartPoint.y << ", " << weldingStartPoint.z << std::endl;

	// determine the CleaveSequenceMeta for the CleaveSequence we are dealing with.
	auto cleaveBegin = sPolyRef->cleaveMap.begin();
	CleaveSequenceMeta foundSequenceMeta = cleaveBegin->second.getCleaveSequenceMeta();

}

void LineWelder::findRemainingWeldingLines(int in_currentBorderLineID, glm::vec3 in_leadingPoint, CleaveSequenceCandidateList* in_cleaveSequenceCandidateListRef, int in_finderStartingCleaveSequenceID)
{
	std::cout << "Welding leading point for this line is: " << in_leadingPoint.x << ", " << in_leadingPoint.y << ", " << in_leadingPoint.z << std::endl;
	// find any neighboring cleave lines that exist on the same border line, if they exist. If they do exist, fetch the next
	// pass in these parameters:
	// -the border line that the finder needs to run on
	// -a reference to border line that the finder will run on
	// -a referenece to the SPoly's cleave map
	// -the CyclingDirection the LineWelder is running in
	// -a reference to a set indicating the candidates (that is, selectable CleaveSequences that haven't been consumed/used) that exist on the border line that the finder runs on
	// -the ID of the current CleaveSequence this finder will start from
	NeighboringCleaveSequenceFinder nextCleaveSequenceFinder(in_currentBorderLineID, &sPolyRef->borderLines[in_currentBorderLineID], &sPolyRef->cleaveMap, foundDirection, in_cleaveSequenceCandidateListRef, in_finderStartingCleaveSequenceID, &metaTracker, in_leadingPoint);
	if (nextCleaveSequenceFinder.wereNeighborsFound() == true)
	{
		FoundCleaveSequence discoveredSequence = nextCleaveSequenceFinder.getSelectedCleaveSequenceMeta();
		std::cout << "||||||||| neighboring cleave sequence was found, stats are: " << std::endl;
		std::cout << "| cleave sequence ID: " << discoveredSequence.cleaveSequenceID << std::endl;
		std::cout << "| distance:           " << discoveredSequence.distance << std::endl;
		std::cout << "| point:              " << discoveredSequence.cleaveSequenceTracingBeginPoint.x << ", " << discoveredSequence.cleaveSequenceTracingBeginPoint.y << ", " << discoveredSequence.cleaveSequenceTracingBeginPoint.z << std::endl;
	}
}