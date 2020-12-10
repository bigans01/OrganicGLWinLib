#include "stdafx.h"
#include "CoplanarMassCreator.h"

void CoplanarMassCreator::runMassManipulation()
{

	CategorizedLinePool linePool;
	SPoly trackedCopy = *trackedSPolyRef;	// make a copy of the original SPoly.




	// ########################################### METHOD 1 

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
	/*
	auto relatedSPolyBegin = sPolyRefMap.refMap.begin();
	auto relatedSPolyEnd = sPolyRefMap.refMap.end();
	for (; relatedSPolyBegin != relatedSPolyEnd; relatedSPolyBegin++)	// (Step 8.)
	{
		trackedCopy.cleaveMap.clear();			// (Step 1: ) clear it's CleaveSequences (just in case); the CleaveSequences must be cleared when comparing against each related SPoly.
		CoplanarCategorizedLineProducer lineProducer(&trackedCopy, relatedSPolyBegin->second, &linePool);		
												//            For each STriangle in the SPoly, acquire it's area, and get the total.

												// (Step 5: ) Get area.
												// (Step 6: ) Subtract from totalTrackedAr											// (Step 7: ) Move the resulting categorized lines into a permanent pool of lines.
	}
	*/

	// 9.) eliminate mirrored lines.
	// 10.) Set back to CREATION
	// 11.) Build appropriate CleaveSequence.
	// 12.) Profit.


	//   ########################################### METHOD 2
	// 1.) Have the CoplanarCategorizedLineProducer produce any categorized lines, and load them into the line pool; do this for each SPoly.
	// 2.) Clip/purge duplciate lines from the line pool.
	// 3.) Load the "cleaned" line pool into the sequenceFactory's categorized lines.
	// 4.) Set the MassManipulationMode on the trackedCopy to DESTRUCTION, and build the cleave the sequences.
	// 5.) Run a SPolyFracturer, without rotation to Z.
	// 6.) Acquire the total area that all of the STriangles produced by all of the related SPolys produce

	// 1.)
	auto relatedSPolyBegin = sPolyRefMap.refMap.begin();
	auto relatedSPolyEnd = sPolyRefMap.refMap.end();
	for (; relatedSPolyBegin != relatedSPolyEnd; relatedSPolyBegin++)	// 
	{
		CoplanarCategorizedLineProducer lineProducer(&trackedCopy, relatedSPolyBegin->second, &linePool);    // put the lines into the pool, if there are any
		
	}

	// 2.) need to remove duplicate lines in the linePool, before doing cleave sequencing.

	trackedCopy.sequenceFactory.transferCategorizedLinesFromLinePool(&linePool);	// 3.) load into the sequence factory.
	trackedCopy.massManipulationSetting = MassManipulationMode::DESTRUCTION;		// 4.)
	trackedCopy.buildCleaveSequences();												// ""

	SPolyMorphTracker tempTracker;	// not sure if we ever even need this? (need to revisit, 12/9/2020)
	SPolyFracturer fracturer(0, &trackedCopy, &tempTracker, SPolyFracturerOptionEnum::NO_ROTATE_TO_Z);		// 5.)
	//fracturer.sPolySG.printSPolys();

	// 6.) calculated the total area that all related SPolys occupy in the tracked SPoly
	float totalRelatedArea = 0.0f;
	auto sPolySuperGroupBegin = fracturer.sPolySG.sPolyMap.begin();
	auto sPolySuperGroupEnd = fracturer.sPolySG.sPolyMap.end();
	for (; sPolySuperGroupBegin != sPolySuperGroupEnd; sPolySuperGroupBegin++)
	{
		auto sTrianglesBegin = sPolySuperGroupBegin->second.triangles.begin();
		auto sTrianglesEnd = sPolySuperGroupBegin->second.triangles.end();
		for (; sTrianglesBegin != sTrianglesEnd; sTrianglesBegin++)
		{
			totalRelatedArea += calculateTriangleArea(sTrianglesBegin->second.triangleLines[0].pointA,
													sTrianglesBegin->second.triangleLines[1].pointA,
													sTrianglesBegin->second.triangleLines[2].pointA);
		}
	}

	// just for fanciness: output the remaining area that isn't occuped in the tracked SPoly.
	float remainingArea = totalTrackedArea - totalRelatedArea;
	std::cout << "Percentage of remaining area: " << remainingArea / totalTrackedArea << std::endl;

}