#include "stdafx.h"
#include "CoplanarMassCreator.h"

void CoplanarMassCreator::runMassManipulation()
{
	// first, find the total area of the tracked SPoly.
	float totalTrackedArea = calculateSPolyArea(trackedSPolyRef);

	// second, we need to find how each related SPoly intersects (if they do) with the tracked SPoly, in order to calculate the area that each related SPoly
	// "consumes" of the tracked SPoly.
	// To do this, we must do the following for EACH related SPoly:
	// 1.) Clear the COPY of the tracked SPoly that we are using (i.e., clear its CleaveSequences and BorderLineInterceptRecords)
	// 2.) (Done in CoplanarCategorizedLineProducer) Get the categorized lines that each related SPoly produces in the tracked SPoly.
	// 3.) (Done in CoplanarCategorizedLineProducer) Build a CleaveSequence in the tracked SPoly from these newly produced lines. (via SPoly::buildCleaveSequences())
	// 4.) (Done in CoplanarCategorizedLineProducer) Perform the fracturing against the tracked SPoly, using an instance of SPolyFracturer. SPolyFracturer may need a new constructor/function to handle this.
	//	   --OR-- needs a new class to handle the fracturing, such as SPolyCoplanarFracturer.
	//     NOTE: the copied SPoly will need to be set as MassManipulationMode::DESTRUCTION, since we want to get the area of the triangles that are "cut" out of 
	//     the tracked SPoly's area as a result of the related SPoly intersecting with it.
	// 5.) Acquire the total amount of area that the current related SPoly consumed on the copy of the tracked SPoly (trackedCopy)
	// 6.) Subtract totalTrackedArea by the amount from the previous step
	// 7.) Put the resulting CategorizedLines from this pass into a permanent pool of lines.
	// 8.) End the current tick, and start over again. Until the loop is done.
	// 9.) Cleanup the pool of permament lines, eliminating mirrored lines.
	// 10.) Reset the copied SPoly's MassManipulationMode back to CREATION.
	// 11.) Build the appropriate CleaveSequence, with the proper categorized lines.
	// 12.) Run the SPolyFracturer (or the new class mentioned in step 4) to produce the correct result.

	SPoly trackedCopy = *trackedSPolyRef;	// make a copy of the original SPoly.
	auto relatedSPolyBegin = sPolyRefMap.refMap.begin();
	auto relatedSPolyEnd = sPolyRefMap.refMap.end();
	for (; relatedSPolyBegin != relatedSPolyEnd; relatedSPolyBegin++)	// (Step 8.)
	{
		trackedCopy.cleaveMap.clear();			// (Step 1: ) clear it's CleaveSequences (just in case); the CleaveSequences must be cleared when comparing against each related SPoly.
		CoplanarCategorizedLineProducer lineProducer(&trackedCopy, relatedSPolyBegin->second);		
												//            For each STriangle in the SPoly, acquire it's area, and get the total.

												// (Step 5: ) Get area.
												// (Step 6: ) Subtract from totalTrackedArea.
												// (Step 7: ) Move the resulting categorized lines into a permanent pool of lines.
	}

	// 9.) eliminate mirrored lines.
	// 10.) Set back to CREATION
	// 11.) Build appropriate CleaveSequence.
	// 12.) Profit.

}