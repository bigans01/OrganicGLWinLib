#include "stdafx.h"
#include "CoplanarMassCreator.h"

void CoplanarMassCreator::runMassManipulation()
{

	CategorizedLinePool persistentLinePool;
	SPoly trackedCopy = *trackedSPolyRef;	// make a copy of the original SPoly.

	//std::cout << "!! Ref map size is: " << sPolyRefMap.refMap.size() << std::endl;
	std::cout << "!! Reached logic for CoplanarMassCreator; proceed? " << std::endl;

	std::cout << "With translation/application of quaternion, the points of the first border line of tracked SPoly are: " << std::endl;
	std::cout << trackedCopy.borderLines[0].pointA.x << ", " << trackedCopy.borderLines[0].pointA.y << ", " << trackedCopy.borderLines[0].pointA.z << " | "
		<< trackedCopy.borderLines[0].pointB.x << ", " << trackedCopy.borderLines[0].pointB.y << ", " << trackedCopy.borderLines[0].pointB.z << std::endl;

	std::cout << "!!! Printing lines, prior to clearing: " << std::endl;
	trackedCopy.sequenceFactory.printLinesInPool();

	std::cout << "!!! Clearing line pools in sequence factory: " << std::endl;
	trackedCopy.sequenceFactory.clearLinePools();			// we don't need any already-existing lines from the copy.

	std::cout << "!!! Line counts: " << std::endl;
	trackedCopy.sequenceFactory.printLineCounts();


	int someVal = 3;
	std::cin >> someVal;


	// ########################################### METHOD 1 

	// first, find the total area of the tracked SPoly.
	float totalTrackedArea = calculateSPolyArea(trackedSPolyRef);

	// second, we need to find how each related SPoly intersects (if they do) with the tracked SPoly, in order to calculate the area that each related SPoly
	// "consumes" of the tracked SPoly.
	// To do this, we must do the following for EACH related SPoly:
	// 1.) Clear the COPY of the tracked SPoly that we are using (i.e., clear its CleaveSequences and BorderLineInterceptRecords)
	// 2.) (Done in CoplanarCategorizedLineProducer) Get the categorized lines that each related SPoly produces in the tracked SPoly.
	// 3.) (Done in CoplanarCategorizedLineProducer) Build a CleaveSequence in the tracked SPoly from these newly produced lines. (via SPoly::buildCleaveSequences())
	// 4.) Perform the fracturing against the tracked SPoly, using an instance of SPolyFracturer. SPolyFracturer may need a new constructor/function to handle this.
	//	   --OR-- needs a new class to handle the fracturing, such as SPolyCoplanarFracturer.
	//     NOTE: the copied SPoly will need to be set as MassManipulationMode::DESTRUCTION, since we want to get the area of the triangles that are "cut" out of 
	//     the tracked SPoly's area as a result of the related SPoly intersecting with it.
	// 5.) Acquire the total amount of area that the current related SPoly consumed on the copy of the tracked SPoly (trackedCopy)
	// 6.) Subtract totalTrackedArea by the amount from the previous step
	// 7.) Put the resulting CategorizedLines from this pass into a permanent pool of lines.
	// 8.) End the current tick, and start over again. Until the loop is done.
	// 9.) Reset the copied SPoly's MassManipulationMode back to CREATION.
	// 10.) Copy the lines from the persistentLinePool, after they've been cleared of mirror lines, back into the trackedCopy's cleaveSequenceFactory.
	// 11.) Build the appropriate CleaveSequence, with the proper categorized lines.
	// 12.) Run the SPolyFracturer (or the new class mentioned in step 4) to produce the correct result.
	
	CategorizedLinePool currentIterationPool;
	float remainingArea = totalTrackedArea;
	float totalRelatedArea = 0.0f;
	auto relatedSPolyBegin = sPolyRefMap.refMap.begin();
	auto relatedSPolyEnd = sPolyRefMap.refMap.end();
	for (; relatedSPolyBegin != relatedSPolyEnd; relatedSPolyBegin++)	// (Step 8.)
	{
		trackedCopy.cleaveMap.clear();			// (Step 1: ) clear it's CleaveSequences (just in case); the CleaveSequences must be cleared when comparing against each related SPoly.
		
		CoplanarCategorizedLineProducer lineProducer(&trackedCopy, relatedSPolyBegin->second, &currentIterationPool);		

		trackedCopy.sequenceFactory.copyCategorizedLinesFromLinePool(&currentIterationPool);

		std::cout << "!! Current iteration pool size: " << currentIterationPool.linePool.size() << std::endl;

		trackedCopy.massManipulationSetting = MassManipulationMode::DESTRUCTION;	// we need to do this, because we need the area of the piece that is cut out.
		trackedCopy.buildCleaveSequences();

		SPolyMorphTracker tempTracker;			// not sure if we ever even need this? (need to revisit, 12/9/2020)

		
		SPolyFracturer fracturer(0, &trackedCopy, &tempTracker, SPolyFracturerOptionEnum::NO_ROTATE_TO_Z);	// (Step 4: ) Perform the fracturing against the tracked SPoly, using an instance of SPolyFracturer. Do not rotate to Z, as this has been done already.
		
		// (Step 5: ) Get area.
		auto sPolySuperGroupBegin = fracturer.sPolySG.sPolyMap.begin();
		auto sPolySuperGroupEnd = fracturer.sPolySG.sPolyMap.end();
		for (; sPolySuperGroupBegin != sPolySuperGroupEnd; sPolySuperGroupBegin++)
		{
			std::cout << "!! checking sTriangles for sPolyID: " << sPolySuperGroupBegin->first << std::endl;

			auto sTrianglesBegin = sPolySuperGroupBegin->second.triangles.begin();
			auto sTrianglesEnd = sPolySuperGroupBegin->second.triangles.end();
			for (; sTrianglesBegin != sTrianglesEnd; sTrianglesBegin++)
			{
				std::cout << "!!! Acquiring total related area..." << std::endl;

				totalRelatedArea += calculateTriangleArea(sTrianglesBegin->second.triangleLines[0].pointA,
					sTrianglesBegin->second.triangleLines[1].pointA,
					sTrianglesBegin->second.triangleLines[2].pointA);
			}

			std::cout << "!! Iteration through STriangle complete, continue? " << std::endl;
			int someVal = 3;
			std::cin >> someVal;
		}										

		// just for fanciness: output the remaining area that isn't occuped in the tracked SPoly.
		remainingArea -= totalRelatedArea;		// (Step 6: ) Subtract from totalTrackedArea
		std::cout << "Percentage of remaining area: " << remainingArea / totalTrackedArea << std::endl;

		// (Step 7: ) Move the resulting pool of categorized lines (currentIterationPool) into a permanent pool of lines. (persistentLinePool); then,
		// clear the currentIterationPool.
		persistentLinePool.copyLinesFromOtherLinePool(&currentIterationPool);
		currentIterationPool.clearPool();
		
	}
	std::cout << "Total tracked area was: " << totalTrackedArea << std::endl;
	persistentLinePool.printLinesInPool();
	trackedCopy.cleaveMap.clear();

	std::cout << "Enter key to continue. " << std::endl;
	int testVal = 3;
	std::cin >> testVal;

	

	// 9.) Set back to CREATION
	// 10.) Copy persisent lines into the trackedCopy.sequenceFactory.


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
	/*
	auto relatedSPolyBegin = sPolyRefMap.refMap.begin();
	auto relatedSPolyEnd = sPolyRefMap.refMap.end();
	for (; relatedSPolyBegin != relatedSPolyEnd; relatedSPolyBegin++)	// 
	{
		CoplanarCategorizedLineProducer lineProducer(&trackedCopy, relatedSPolyBegin->second, &linePool);    // put the lines into the pool, if there are any
		
	}
	*/

	// 2.) need to remove duplicate lines in the linePool, before doing cleave sequencing.
	/*
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
	*/
}