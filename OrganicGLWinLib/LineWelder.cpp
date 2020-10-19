#include "stdafx.h"
#include "LineWelder.h"

void LineWelder::startWelding()
{
	// Step 1: get the first line, in the first cleave sequence, and determine how many border lines it has. 
	auto cleaveBegin = sPolyRef->cleaveMap.begin();
	auto sequenceBegin = cleaveBegin->second.cleavingLines.begin();
	auto sequenceEnd = cleaveBegin->second.cleavingLines.rbegin();
	if (sequenceBegin->second.line.numberOfBorderLines == 1)		// if there are multiple CategorizedLines in the cleave sequence, the first line in the sequence
																	// will have exactly one border line. The end of the sequence will have another border line.
	{
		currentBorderLineID = sequenceBegin->second.line.getBorderLineIDFromSingularBorderLineCount();
		endingBorderLineID = sequenceEnd->second.line.getBorderLineIDFromSingularBorderLineCount();
	}
	else if (sequenceBegin->second.line.numberOfBorderLines == 2)	// if there is just one line, it'll have two border lines; now we must
																	// get the borders from the pointA and pointB of this line.
	{
		BorderLineIDPair retrievedPair = sequenceBegin->second.line.getBorderLineIDPair();
		currentBorderLineID = retrievedPair.pointABorderLineID;
		endingBorderLineID = retrievedPair.pointBBorderLineID;
	}

	// Step 2: find the direction to cycle towards for the currentBorderLineID; i.e., do we start the cycling in the direction of point A or point B of this border line?
	// Remember, if the MassManipulationMode is DESTRUCTION, we must invert the empty normal of the CategorizedLine we are using to find the direction.
	BorderLineCycler lineCycler(sPolyRef);
	lineCycler.findCyclingDirection(&sPolyRef->borderLines[currentBorderLineID], currentBorderLineID, &sequenceBegin->second, sPolyRef->massManipulationSetting);
}