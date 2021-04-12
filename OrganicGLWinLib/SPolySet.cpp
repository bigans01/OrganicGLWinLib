#include "stdafx.h"
#include "SPolySet.h"

//#define SMALL_NUM   0.00000001 // anything that avoids division overflow
#define SMALL_NUM   0.000001 // anything that avoids division overflow

int SPolySet::addPoly(SPoly in_sPoly)
{
	int indexToUse = numberOfPolys;	// indexToUse will be used as the return value, but must be set before numberOfPolys get incremented at the end of the function.
	secondaryPolys[numberOfPolys] = in_sPoly;		// add the poly, and increment it
	polyMorphTracker.morphablePolys[numberOfPolys] = in_sPoly;
	numberOfPolys++;
	return indexToUse;
}

void SPolySet::setOption(DebugOption in_option)
{

	// "Main" level debug options
	if (in_option == DebugOption::SPOLYSET_TYPICAL)
	{
		comparisonLogger.setDebugLevel(PolyDebugLevel::DEBUG);
		std::cout << "!!! Will set TYPICAL operations to debug mode. " << std::endl;
	}
	else if (in_option == DebugOption::SPOLYSET_BOUNDARIES)
	{
		std::cout << "!!! Will set BOUNDARY operations to debug mode. " << std::endl;
	}
	else if (in_option == DebugOption::SPOLYSET_ALL_SPOLY_MAIN)
	{
		std::cout << "!!! Will set option for all main-level SPolys: MAIN" << std::endl;
		debugOptionsAllSPolys.insert(SPolyDO::MAIN);
	}
	else if (in_option == DebugOption::SPOLYSET_ALL_SPOLY_FACTORY)
	{
		std::cout << "!!! Will set option for all main-level SPolys: FACTORY" << std::endl;
		debugOptionsAllSPolys.insert(SPolyDO::FACTORY);
	}
	else if (in_option == DebugOption::SPOLYSET_ALL_SPOLY_FACTORY_MERGER)
	{
		std::cout << "!!! Will set option for all main-level SPolys: FACTORY_MERGER" << std::endl;
		debugOptionsAllSPolys.insert(SPolyDO::FACTORY_MERGER);
	}
	else if (in_option == DebugOption::SPOLYSET_ALL_FRACTURER)
	{
		std::cout << "!!! Will set option for all main-level SPolys: FACTORY_FRACTURER" << std::endl;
		//debugOptionsAllSPolys.insert(SPolyDO::FACTORY_FRACTURER);
		mainFracturerDebugLevel = PolyDebugLevel::DEBUG;
	}
	else if (in_option == DebugOption::SPOLYSET_ALL_COPLANAR_RELATIONSHIPS_BASIC)
	{
		coplanarRelationshipDebugLevel = PolyDebugLevel::DEBUG;
	}

	// All other options; for MassZoneMaster.
	else
	{
		zoneMaster.handleDebugOption(in_option);
	}

}

void SPolySet::setSpecificSPolyOption(int in_sPolyID, DebugOption in_option)
{
	std::cout << "!!! Successful call of setSpecificSPolyOption, for SPolyID: " << in_sPolyID << std::endl;
	if (in_option == DebugOption::SPECIFIC_SPOLY_MAIN)
	{
		specificSPolyOptionMap[in_sPolyID].insert(SPolyDO::MAIN);
	}
	else if (in_option == DebugOption::SPECIFIC_SPOLY_FACTORY)
	{
		specificSPolyOptionMap[in_sPolyID].insert(SPolyDO::FACTORY);
	}
	else if (in_option == DebugOption::SPECIFIC_SPOLY_FACTORY_MERGER)
	{
		specificSPolyOptionMap[in_sPolyID].insert(SPolyDO::FACTORY_MERGER);
	}
	else if (in_option == DebugOption::SPECIFIC_FRACTURER_EXECUTION)
	{
		specificSPolyOptionMap[in_sPolyID].insert(SPolyDO::FRACTURER);
	}
	else if (in_option == DebugOption::SPECIFIC_SPOLY_CATEGORIZED_LINES)
	{
		specificSPolyOptionMap[in_sPolyID].insert(SPolyDO::CATEGORIZED_LINES);
	}
	else if (in_option == DebugOption::SPECIFIC_SPOLY_CATEGORIZED_LINE_COPLANAR_TESTS)
	{
		specificSPolyOptionMap[in_sPolyID].insert(SPolyDO::CATEGORIZED_LINE_COPLANAR_TESTS);
	}
}

PolyDebugLevel SPolySet::checkForSPolyOptionInSpecificSPoly(int in_sPolyID, SPolyDO in_sPolyDO)
{
	PolyDebugLevel returnLevel = PolyDebugLevel::NONE;
	auto specificSPolyIter = specificSPolyOptionMap.find(in_sPolyID);
	if (specificSPolyIter != specificSPolyOptionMap.end())	// an entry was found, but must be checked to see if it contains the value we need.
	{
		auto specificOptionFinder = specificSPolyIter->second.debugOptions.find(in_sPolyDO);
		if (specificOptionFinder != specificSPolyIter->second.debugOptions.end())	// the option we needed was found.
		{
			returnLevel = PolyDebugLevel::DEBUG;
		}
	}
	return returnLevel;
}

void SPolySet::applyDebugOptionsToSpecificSPolys()
{
	auto polysBegin = secondaryPolys.begin();
	auto polysEnd = secondaryPolys.end();
	for (; polysBegin != polysEnd; polysBegin++)
	{
		auto optionsFinder = specificSPolyOptionMap.find(polysBegin->first);
		if (optionsFinder != specificSPolyOptionMap.end())	// it was found
		{
			std::cout << "(SPolySet) !! Found options for SPoly with ID " << polysBegin->first << std::endl;
			polysBegin->second.applyDebugOptions(specificSPolyOptionMap[polysBegin->first]);
		}
	}
}

void SPolySet::removeSPolysFlaggedAsPurgable()
{
	// old zone
	std::set<int> oldZonePurgables = zoneMaster.oldZone.clipper.sPolysToPurge;
	auto oldPurgablesBegin = oldZonePurgables.begin();
	auto oldPurgablesEnd = oldZonePurgables.end();
	for (; oldPurgablesBegin != oldPurgablesEnd; oldPurgablesBegin++)
	{
		polyFracturingResults.supergroupMap[*oldPurgablesBegin].sPolyMap.clear();
		//std::cout << "!! (OLD_ZONE purgables) Purgable poly SuperGroup having ID " << *oldPurgablesBegin << " has been cleared. " << std::endl;
	}

	// new zone
	std::set<int> newZonePurgables = zoneMaster.newZone.clipper.sPolysToPurge;
	auto newPurgablesBegin = newZonePurgables.begin();
	auto newPurgablesEnd = newZonePurgables.end();
	for (; newPurgablesBegin != newPurgablesEnd; newPurgablesBegin++)
	{
		polyFracturingResults.supergroupMap[*newPurgablesBegin].sPolyMap.clear();
		//std::cout << "!! (NEW_ZONE purgables) Purgable poly SuperGroup having ID " << *newPurgablesBegin << " has been cleared. " << std::endl;
	}

	// removable polys found in coplanarTracker
	auto coplanarTrackerRemovablesBegin = coplanarTracker.removableSPolys.intSet.begin();
	auto coplanarTrackerRemovablesEnd = coplanarTracker.removableSPolys.intSet.end();
	for (; coplanarTrackerRemovablesBegin != coplanarTrackerRemovablesEnd; coplanarTrackerRemovablesBegin++)
	{
		polyFracturingResults.supergroupMap[*coplanarTrackerRemovablesBegin].sPolyMap.clear();
	}
}

MassManipulationMode SPolySet::getManipulationModeForSPolyGroup(int in_sPolyGroupID) // gets the MMM for a group of SPolys (passed in value should be 0 or 1
{
	MassManipulationMode returnMode = MassManipulationMode::NOVAL;
	auto sPolysBegin = secondaryPolys.begin();
	auto sPolysEnd = secondaryPolys.end();
	for (; sPolysBegin != sPolysEnd; sPolysBegin++)
	{
		if (sPolysBegin->second.groupID == in_sPolyGroupID)
		{
			returnMode = sPolysBegin->second.massManipulationSetting;
		}
	}
	return returnMode;
}

void SPolySet::reset()
{
	numberOfPolys = 0;
}

void SPolySet::configurePolys()
{
	for (int x = 0; x < numberOfPolys; x++)
	{
		// PHASE 1: determine border lines, empty normals, and planar normals
		secondaryPolys[x].determinePrimalPoints();		// determine the primal points for the triangle
		secondaryPolys[x].determineBorderLines();		// for each SPoly, determine its border lines.
		secondaryPolys[x].calculateEmptyNormal();		// find the empty normal for each poly
	}
}

void SPolySet::configurePolysWithoutNormalCalcs()
{
	for (int x = 0; x < numberOfPolys; x++)
	{
		// PHASE 1: determine border lines, and planar normals; empty normals would already be calculated for these secondaries, if using this function as intended.
		secondaryPolys[x].determinePrimalPoints();		// determine the primal points for the triangle
		secondaryPolys[x].determineBorderLines();		// for each SPoly, determine its border lines.
	}
}

void SPolySet::runPolyComparison(MassZoneBoxType in_massZoneBoxType)
{
	// apply any debug options to existing SPolys.
	applyDebugOptionsToSpecificSPolys();

	// build the zone boundaries for the MassZones
	//zoneMaster.setMassZoneLogLevels(PolyDebugLevel::DEBUG);			// hard-coded for testing, for the time being. (1/11/2021)
	zoneMaster.createMassZoneBoxBoundaries(in_massZoneBoxType);
	zoneMaster.setZoneClipperReferences();

	// set the mass manipulation mode for each zone's clipper; we must sample the mmm from the SPoly group IDs.
	zoneMaster.setOldZoneClipperManipulationMode(getManipulationModeForSPolyGroup(0));	// old zone uses group 0's manipulation mode
	zoneMaster.setNewZoneClipperManipulationMode(getManipulationModeForSPolyGroup(1));  // new zone uses group 0's manipulation mode


	//zoneMaster.printMassZoneBorderLineCounts();
	
	//std::cout << "!!! Finished printing immediate values of border line counts. " << std::endl;
	//int waitVal = 3;
	//std::cin >> waitVal;

	// set the debug level for the coplanar tracker
	coplanarTracker.setRelationshipTrackerDebugLevel(coplanarRelationshipDebugLevel);

	int compCount2 = numberOfPolys;

	// Step 1: check for non-planar categorized lines to produce, but load any coplanar relationships we find into the
	// coplanarTracker.
	auto comparisonIterationsBegin = std::chrono::high_resolution_clock::now();
	for (int x = 0; x < compCount2; x++)
	{
		int currentIndex = 0;						// index begins at 0, increments every tick of the y loop that follows.
		for (int y = 0; y < numberOfPolys; y++)
		{
			if (!(x == y))	// never compare the poly to itself
			{
				SPoly* polyA = &secondaryPolys[x];
				SPoly* polyB = &secondaryPolys[currentIndex];

				if (polyA->groupID != polyB->groupID)		// don't compare polys in the same group to each other
				{

					// determine if we need to do the special case, where polyA and polyB are coplanar.
					CoplanarChecker checker(polyA, polyB, comparisonLogger.getLogLevel());

					// if they are coplanar, check polyA to see if it's group 1. If it is, set into the coplanar comparsion list.
					if (checker.coplanarityDetected == true)	
					{
						// if they are coplanar AND polyA is a group 1 poly, do this
						if (polyA->groupID == 1)	
						{
							//std::cout << "!!!! Inserting record into coplanar relationships... " << std::endl;
							//std::cout << "!!!! Tracked Poly ID: " << x << ", Related PolyID: " << currentIndex << std::endl;
							comparisonLogger.log("!!!! Inserting record into coplanar relationships... ", "\n");
							comparisonLogger.log("!!!! Tracked Poly ID: ", x, ", Related PolyID: ", currentIndex, "\n");
							//int someVal = 3;
							//std::cin >> someVal;

							coplanarTracker.insertCoplanarRelationship(x, polyA, currentIndex, polyB);
						}
					}
					// otherwise, if they are not coplanar, do the following.
					else
					{
							//std::cout << "############## Comparing poly ID " << x << " to poly ID " << currentIndex << std::endl;
							//std::cout << "##### Current index is: " << currentIndex << std::endl;
							//std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||| --> Generating next set of categorized lines... (" << generationCounter++ << ") " << std::endl;
							/*
							int prePass = 3;
							if (comparisonLogger.isLoggingSet() == true)
							{
								std::cout << "|||||||||| Comparing 'host' spoly with index " << x << " to 'guest' poly with index " << currentIndex << std::endl;
								std::cin >> prePass;
							}
							*/

							comparisonLogger.log("|||||||||| Comparing 'host' spoly with index ", x, " to 'guest' poly with index ", currentIndex, "\n");
							comparisonLogger.waitForDebugInput();

							produceCategorizedLinesForHostPoly(polyA, x, polyB, currentIndex);		// PHASE 1
							//std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||| Passed 1 set of categorized lines. (typical comparison)" << std::endl;
							//int compare = 3;
							//std::cin >> compare;
							/*
							if (comparisonLogger.isLoggingSet() == true)
							{
								std::cout << "|||||||||| COMPLETED, Comparing 'host' spoly with index " << x << " to 'guest' poly with index " << currentIndex << std::endl;
								std::cout << "||| Resulting lines in polyA's sequenceFactory are: " << std::endl;
								polyA->sequenceFactory.printLineCounts();
								polyA->sequenceFactory.printLinesInPool();
								std::cin >> prePass;
							}
							*/
							comparisonLogger.log("|||||||||| COMPLETED, Comparing 'host' spoly with index ", x, " to 'guest' poly with index ", currentIndex, "\n");
							comparisonLogger.log("||| Resulting lines in PolyA's sequenceFactory are: ", "\n");
							if (comparisonLogger.isLoggingSet() == true)
							{
								polyA->sequenceFactory.printLineCounts();
								polyA->sequenceFactory.printLinesInPool();
							}
							comparisonLogger.waitForDebugInput();
					}
				}
			}
			currentIndex++;
		}

		//std::cout << "|||| Finished for this SPoly ->" << x << std::endl;
		//coplanarTracker.buildCategorizedLinesForCoplanarRelationship(x);
		//zoneMaster.printMassZoneBorderLineCounts();

		// Use the MassZoneMaster to insert into either the old zone or new zone; each group that a MassZone represents must have all of the original SPolys of that group, at this point in time.
		if (secondaryPolys[x].groupID == 0)
		{
			zoneMaster.registerSPolyToMassZone(x, secondaryPolys[x], MassZoneType::OLD_ZONE);
		}
		else if (secondaryPolys[x].groupID == 1)
		{
			zoneMaster.registerSPolyToMassZone(x, secondaryPolys[x], MassZoneType::NEW_ZONE);
		}

		//zoneMaster.printMassZoneBorderLineCounts();
		
	}
	auto comparisonIterationsEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> comparisonIterationsElapsed = comparisonIterationsEnd - comparisonIterationsBegin;
	std::cout << "#-> Comparisons time  > " << comparisonIterationsElapsed.count() << std::endl;

	// Step 2:
	// Build the non-SPoly based MassSubZones, for each MassZone, once all SPolys have been copied into the appropriate MassZone. The combination of temporal or artificial subzones, plus the actual "material" sPolys, should form a "MassZoneShell."
	// 
	// To do this:
	// 1.) run the SPolys in each MassZone (use sPolyToSubZoneMap to find the key to use to find the appropriate SPoly in the appropriate MassSubZone, i.e.): 
	//			1-A:): for each entry in sPolySubZoneMap (member of MassZone class), the value of the key-value pair (we will call this the "fetcher key") represents the key in the subZoneMap that holds the corresponding SPoly to reference. 
	//			       Find the value for the "fetcher key", and use this to find correpsonding MassSubZone. This corresponding MassSubZone contains an SPoly that isn't "temporal", meaning that it actually has permanence and isn't temporary.
	//          1-B:): take the border lines of the SPoly we found from the "fetcher key" and see how they interact with any of the MassZoneBoxBoundaries in the MassZoneBox of the MassZone.
	auto massZoneShellStart = std::chrono::high_resolution_clock::now();
	zoneMaster.createMassZoneShells();
	auto massZoneShellEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> massZoneShellElapsed = massZoneShellEnd - massZoneShellStart;
	std::cout << "#-> Mass Zone Shell time  > " << massZoneShellElapsed.count() << std::endl;

	



	// Step 3: build the CleaveSequences
	for (int x = 0; x < compCount2; x++)
	{
		//secondaryPolys[x].sequenceFactory.printLinesInPool();
		//std::cout << "|||| Finished print lines in pool..." << std::endl;

		// if an SPoly contains CleaveSequences, it must be disqualified (meaning, the appropriate MassZone it belongs to doesn't need to compare it to another MassZone.)
		std::cout << ">>>>>>> Building cleave sequences, for SPoly with ID: " << x << std::endl;
		secondaryPolys[x].buildCleaveSequences(CleaveSequenceMergeMode::MERGE);		
		/*
		if (secondaryPolys[x].sequenceFactory.doesFactoryContainLines() == true)
		{
			//zoneMaster.disqualifyMeshMatterMeta(x);
		}
		*/

		//std::cout << "Size of CleaveMap for SPoly " << x << ": " << secondaryPolys[x].cleaveMap.size() << std::endl;
		//std::cout << "Has bad production: " << secondaryPolys[x].sequenceFactory.hasBadProduction << std::endl;

		
		if 
		(
			(secondaryPolys[x].cleaveMap.size() != 0)		// new code, as of 4/1/2021: an SPoly may only be disqualified if it actually managed to produce
															// valid CleaveSequences.
			//||
			//(secondaryPolys[x].sequenceFactory.hasBadProduction == true)
		)
		{
			//std::cout << "!!!! Disqualifying SPoly with ID: " << x << std::endl;
			zoneMaster.disqualifyMeshMatterMeta(x);
		}
		
		
		//int buildVal = 3;
		//std::cin >> buildVal;

		//secondaryPolys[x].printAllCleaveLines();
		//std::cout << "+++++ Enter number to go to next poly. " << std::endl;
		//int someVal = 3;
		//std::cin >> someVal;

		
		if (secondaryPolys[x].sequenceFactory.hasBadProduction == true)
		{
			std::cout << ">>>>>>>>>> SPoly with ID " << x << " had its production fail. " << std::endl;
		}
		
	}
	std::cout << "!!!!! Finished building CleaveSequences. " << std::endl;

	// After any disqualifications have been applied (that is, an SPoly has CleaveSequences in it), run the point clippers.
	zoneMaster.runPointClippers();

	// Step 4: execute the relationships found in the coplanarTracker, if any
	// when the SPoly at x has been compared to all other SPolys, we should check for any coplanar relationships for x.
	// it's completely possible for a SPoly to have categorized lines from a coplanar relationship AND a non-coplanar replationship.
	auto cuttingSequenceTestStart = std::chrono::high_resolution_clock::now();
	coplanarTracker.runAllCuttingSequenceTests();
	auto cuttingSequenceTestEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> cuttingSequenceTestElapsed = cuttingSequenceTestEnd - cuttingSequenceTestStart;
	std::cout << "#-> Cutting sequence test  time  > " << cuttingSequenceTestElapsed.count() << std::endl;
	//std::cout << "|||| Finished coplanar check..." << std::endl;		

	/*
	for (int x = 0; x < compCount2; x++)
	{
		// if an SPoly contains CleaveSequences, it must be disqualified (meaning, the appropriate MassZone it belongs to doesn't need to compare it to another MassZone.)
		if (secondaryPolys[x].sequenceFactory.doesFactoryContainLines() == true)
		{
			zoneMaster.disqualifyMeshMatterMeta(x);
		}
	}
	*/

	//std::cout << "########################################################################## " << std::endl;
	//std::cout << ".............> printing MassZoneMaster stats: " << std::endl;
	//zoneMaster.printQualifiedMeshMatterMetas();
	//int massZoneInput = 3;
	//std::cin >> massZoneInput;

	/*
	std::cout << "########################################################################## " << std::endl;
	std::cout << "############### Printing cleave map counts ############################### " << std::endl;
	std::cout << "########################################################################## " << std::endl;
	for (int x = 0; x < compCount2; x++)
	{
		std::cout << "SPoly at index " << x << " has " << secondaryPolys[x].cleaveMap.size() << " CleaveSequences; it's mode is:  ";
		if (secondaryPolys[x].massManipulationSetting == MassManipulationMode::CREATION)
		{
			std::cout << "CREATION " << std::endl;
		}
		else if (secondaryPolys[x].massManipulationSetting == MassManipulationMode::DESTRUCTION)
		{
			std::cout << "DESTRUCTION " << std::endl;
		}
	}
	int cleaveMapCountStop = 3;
	std::cin >> cleaveMapCountStop;
	*/
}

int SPolySet::produceCategorizedLinesForHostPoly(SPoly* in_hostPolyPtr, int in_hostPolyAID, SPoly* in_guestPolyPtr, int in_guestPolyID)
{
	int numberOfIntersections = 0;

	int hostPolyTriangleCount = in_hostPolyPtr->numberOfTriangles;
	int guestPolyTriangleCount = in_guestPolyPtr->numberOfTriangles;

	//std::cout << "polyA count: " << hostPolyTriangleCount << std::endl;
	//std::cout << "polyB count: " << guestPolyTriangleCount << std::endl;

	//std::cout << "++++++ :::: host poly ID: " << in_hostPolyAID << std::endl;
	//std::cout << "++++++ :::: guest poly ID: " << in_guestPolyID << std::endl;

	//std::cout << "########################## ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ >>> A lines are: " << std::endl;
	//std::cout << "0: " << in_polyAPtr->triangles[0].triangleLines[0].pointA.x << ", " << in_polyAPtr->triangles[0].triangleLines[0].pointA.y << ", " << in_polyAPtr->triangles[0].triangleLines[0].pointA.z << " ||| " << in_polyAPtr->triangles[0].triangleLines[0].pointB.x << ", " << in_polyAPtr->triangles[0].triangleLines[0].pointB.y << ", " << in_polyAPtr->triangles[0].triangleLines[0].pointB.z << std::endl;
	//std::cout << "1: " << in_polyAPtr->triangles[0].triangleLines[1].pointA.x << ", " << in_polyAPtr->triangles[0].triangleLines[1].pointA.y << ", " << in_polyAPtr->triangles[0].triangleLines[1].pointA.z << " ||| " << in_polyAPtr->triangles[0].triangleLines[1].pointB.x << ", " << in_polyAPtr->triangles[0].triangleLines[1].pointB.y << ", " << in_polyAPtr->triangles[0].triangleLines[1].pointB.z << std::endl;
	//std::cout << "2: " << in_polyAPtr->triangles[0].triangleLines[2].pointA.x << ", " << in_polyAPtr->triangles[0].triangleLines[2].pointA.y << ", " << in_polyAPtr->triangles[0].triangleLines[2].pointA.z << " ||| " << in_polyAPtr->triangles[0].triangleLines[2].pointB.x << ", " << in_polyAPtr->triangles[0].triangleLines[2].pointB.y << ", " << in_polyAPtr->triangles[0].triangleLines[2].pointB.z << std::endl;
	//std::cout << "2, point A: " << tempBTrianglePtr->triangleLines[2].pointA.x << ", " << tempBTrianglePtr->triangleLines[2].pointA.y << ", " << tempBTrianglePtr->triangleLines[2].pointA.z << std::endl;
	//std::cout << "2, point B: " << tempBTrianglePtr->triangleLines[2].pointB.x << ", " << tempBTrianglePtr->triangleLines[2].pointB.y << ", " << tempBTrianglePtr->triangleLines[2].pointB.z << std::endl;


	//int someVal = 3;
	//std::cout << "temp halt, enter integer. " << std::endl;
	//std::cin >> someVal;

	// perform a co-planar test here; if the two poly's arent' coplanar, proceed with normal operations.

	//std::cout << "+++++++++ host poly Triangles: " << hostPolyTriangleCount << std::endl;

	// acquire the debug level for CategorizedLines.
	//PolyDebugLevel categorizedLineDebugLevel = checkIfSpecificSPolyCategorizedLineDebugIsSet(in_hostPolyAID);
	PolyDebugLevel categorizedLineDebugLevel = checkForSPolyOptionInSpecificSPoly(in_hostPolyAID, SPolyDO::CATEGORIZED_LINES);
	PolyLogger currentComparisonLogger;
	currentComparisonLogger.setDebugLevel(categorizedLineDebugLevel);
	currentComparisonLogger.log("(SPolySet): ||||||||||||||||||||||||||||||||||||||||||| CategorizedLine debug option found for SPoly having ID ", in_hostPolyAID, "; comparing against SPoly with ID ", in_guestPolyID, ".", "\n");

	// acquire the debug level for CategorizedLine coplanar tests.
	PolyDebugLevel coplanarTestDebugLevel = checkForSPolyOptionInSpecificSPoly(in_hostPolyAID, SPolyDO::CATEGORIZED_LINE_COPLANAR_TESTS);


	for (int currentHostPolyTriangle = 0; currentHostPolyTriangle < hostPolyTriangleCount; currentHostPolyTriangle++)					// compare each of poly A's tertiaries...
	{

		if (currentComparisonLogger.isLoggingSet())
		{
			currentComparisonLogger.log("(SPolySet): Begin comparison of STriangle with index ", currentHostPolyTriangle, ", in SPoly ", in_hostPolyAID, "\n");
			currentComparisonLogger.log("(SPolySet): ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|||||| Points of this host STriangle are:", "\n");
			in_hostPolyPtr->triangles[currentHostPolyTriangle].printPoints();
		}
		//std::cout << "::::::::::::::::::::::::::::::::::: ----------------------------------+++++++++>>>>>>>>>>> Running host poly Triangle comparison: " << std::endl;

		STriangle* hostTrianglePtr = &in_hostPolyPtr->triangles[currentHostPolyTriangle];	// " " 
		IntersectionLineGroup hostLineGroup(currentComparisonLogger.getLogLevel());						// the line group for poly A.
		IntersectionLineGroup guestLineGroup(currentComparisonLogger.getLogLevel());						// the line group for poly B.
		hostLineGroup.setFusionAnalyzerSPolyRef(in_hostPolyPtr);
		guestLineGroup.setFusionAnalyzerSPolyRef(in_guestPolyPtr);
		for (int y = 0; y < guestPolyTriangleCount; y++)					// .. to each of poly B's tertiaries...
		{
			// for each pair (that is, A's current STriangle to B's current STriangle in the iterations), we must:
			// 1. compare the lines of STriangle A to that of STriangle B
			// 2. compare the lines of STriangle B to that of STriangle A
			// 3. analyze the relationship between them

			// >>>>>>>>>>>>>>>>>>>>> STEP 1
			// compare the host triangle lines, to the guest triangles. |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

			STriangle* guestTrianglePtr = &in_guestPolyPtr->triangles[y]; // get the guest poly's triangle
			if (currentComparisonLogger.isLoggingSet())
			{
				currentComparisonLogger.log("(SPolySet): ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~>>>>> Points of the current guest STriangle are:", "\n");
				guestTrianglePtr->printPoints();
			}

			//std::cout << "::::::::::::::::::::::::::::::::::: >>>>>>>>>>>>>>>>>>>>>>>>>>> Comparing lines of the host to the guest triangle " << std::endl;
			currentComparisonLogger.log("(SPolySet) [Comparison: ", in_hostPolyAID, " > ", in_guestPolyID, "] ::::::::::::::::::::::::::::::::::: >>>>>>>>>>>>>>>>>>>>>>>>>>> BEGIN Comparing lines of the host to the guest triangle ", "\n");

			//std::cout << ">>> B triangle (Guest triangle) points are: " << std::endl;
			//std::cout << "0: " << guestTrianglePtr->triangleLines[0].pointA.x << ", " << guestTrianglePtr->triangleLines[0].pointA.y << ", " << guestTrianglePtr->triangleLines[0].pointA.z << std::endl;
			//std::cout << "1: " << guestTrianglePtr->triangleLines[1].pointA.x << ", " << guestTrianglePtr->triangleLines[1].pointA.y << ", " << guestTrianglePtr->triangleLines[1].pointA.z << std::endl;
			//std::cout << "2: " << guestTrianglePtr->triangleLines[2].pointA.x << ", " << guestTrianglePtr->triangleLines[2].pointA.y << ", " << guestTrianglePtr->triangleLines[2].pointA.z << std::endl;
			//std::cout << "2 (B): " << guestTrianglePtr->triangleLines[2].pointB.x << ", " << guestTrianglePtr->triangleLines[2].pointB.y << ", " << guestTrianglePtr->triangleLines[2].pointB.z << std::endl;
			for (int currentHostTriangleLine = 0; currentHostTriangleLine < 3; currentHostTriangleLine++)		// run the lines of A (the host) through triangle B (the guest)
			{
				//FusionCandidateProducer hostCandidateProducer(currentComparisonLogger.getLogLevel());					// initialize with debug level 
				FusionCandidateProducer hostCandidateProducer(PolyDebugLevel::NONE);					// initialize with debug level 
				FusionCandidate hostFusedCandidate = hostCandidateProducer.produceCandidate(*guestTrianglePtr, in_hostPolyPtr->triangles[currentHostPolyTriangle].triangleLines[currentHostTriangleLine]);
				if (hostFusedCandidate.candidateIntersectionResult.wasIntersectFound != 0)
				{
					hostLineGroup.insertFusionCandidateIntoAnalyzer(FusionCandidateOrigin::HOST, currentHostTriangleLine, hostFusedCandidate);
				}		
			}
			hostLineGroup.runFusionAnalysisAndDetermineClassifications();
			//std::cout << "::::::::::::::::::::::::::::::::::: >>>>>>>>>>>>>>>>>>>>>>>>>>> ENDED Comparing lines of the host to the guest triangle " << std::endl;
			currentComparisonLogger.log("(SPolySet) [Comparison: ", in_hostPolyAID, " > ", in_guestPolyID, "] ::::::::::::::::::::::::::::::::::: >>>>>>>>>>>>>>>>>>>>>>>>>>> ENDED Comparing lines of the host to the guest triangle ", "\n");


			// >>>>>>>>>>>>>>>>>>>>> STEP 2
			// compare the GUEST triangle lines, to the host triangles. ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
			//std::cout << "::::::::::::::::::::::::::::::::::: >>>>>>>>>>>>>>>>>>>>>>>>>>> Comparing lines of the guest to the host triangle" << std::endl;
			currentComparisonLogger.log("(SPolySet) [Comparison: ", in_hostPolyAID, " > ", in_guestPolyID, "] ::::::::::::::::::::::::::::::::::: >>>>>>>>>>>>>>>>>>>>>>>>>>> BEGIN Comparing lines of the guest to the host triangle ", "\n");
			for (int z = 0; z < 3; z++)																			// run the lines of B (the guest) through triangle A (the host)
			{
				//FusionCandidateProducer guestCandidateProducer(currentComparisonLogger.getLogLevel());					// initialize with debug level 
				FusionCandidateProducer guestCandidateProducer(PolyDebugLevel::NONE);					// initialize with debug level 
				FusionCandidate guestFusedCandidate = guestCandidateProducer.produceCandidate(*hostTrianglePtr, guestTrianglePtr->triangleLines[z]);
				if (guestFusedCandidate.candidateIntersectionResult.wasIntersectFound != 0)
				{
					guestLineGroup.insertFusionCandidateIntoAnalyzer(FusionCandidateOrigin::GUEST, z, guestFusedCandidate);
				}
				
			}
			guestLineGroup.runFusionAnalysisAndDetermineClassifications();
			//std::cout << "::::::::::::::::::::::::::::::::::: >>>>>>>>>>>>>>>>>>>>>>>>>>> ENDED Comparing lines of the guest to the host triangle" << std::endl;
			currentComparisonLogger.log("(SPolySet) [Comparison: ", in_hostPolyAID, " > ", in_guestPolyID, "] ::::::::::::::::::::::::::::::::::: >>>>>>>>>>>>>>>>>>>>>>>>>>> ENDED Comparing lines of the guest to the host triangle ", "\n");

			// >>>>>>>>>>>>>>>>>>>>> STEP 3: Execute FusedPointReactor
			FusedPointReactor reactor(&hostLineGroup.returnLine.completedAnalysis, &guestLineGroup.returnLine.completedAnalysis, currentComparisonLogger.getLogLevel());
			FusedPointReactorResult reactionResult = reactor.getReactorResult();
			reactionResult.resultingLine.parentPoly = in_guestPolyID;
			if (reactionResult.wasLineProduced == true)
			{	
				currentComparisonLogger.log("(SPolySet)	Reaction result produced a line! ", "\n");


				CategorizedLineColinearTester tester(reactionResult.resultingLine, *hostTrianglePtr, coplanarTestDebugLevel);
				if (tester.colinearToBorderLineDetected == false)		// the categorized line isn't colinear to any line in the host triangle (remember, context is from host triangle)
				{
					currentComparisonLogger.log("(SPolySet)	Adding a categorized line! ", "\n");
					in_hostPolyPtr->sequenceFactory.addCategorizedLine(reactionResult.resultingLine);
					numberOfIntersections++;
				}
				else
				{
					currentComparisonLogger.log("(SPolySet) Notice: CategorizedLine to add was detected as being colinear to host:", "\n");
					std::cout << "!!! Notice: CategorizedLine was detected as being colinear to a line in the host STriangle. " << std::endl;
					if (currentComparisonLogger.isLoggingSet())
					{
						currentComparisonLogger.log("(SPolySet): point A of categorized line: ", reactionResult.resultingLine.line.pointA.x, ", ", reactionResult.resultingLine.line.pointA.y, ", ", reactionResult.resultingLine.line.pointA.z, "\n");
						currentComparisonLogger.log("(SPolySet): point B of categorized line: ", reactionResult.resultingLine.line.pointB.x, ", ", reactionResult.resultingLine.line.pointB.y, ", ", reactionResult.resultingLine.line.pointB.z, "\n");
						currentComparisonLogger.log("(SPolySet): points of host STriangle: ", "\n");
						hostTrianglePtr->printPoints();
					}
				}
			}

			comparisonLogger.log("######>>>>>>> Comparison complete, enter number to continue to next comparison. ", "\n");
			comparisonLogger.waitForDebugInput();

			hostLineGroup.reset();
			guestLineGroup.reset();

			//std::cout << "+++++++++ comparing next tertiary..." << std::endl;
		}

		//std::cout << "------------++ done comparing current tertiary in triangle A; moving to next tertiary in triangle A..." << std::endl;

	}
	//std::cout << "Current poly cleave size: " << in_polyAPtr->cleaveMap.size() << std::endl;
	//in_polyAPtr->printLines();
	//in_polyAPtr->moveLastCleave();		// move any remaining lines into a cleave, if they exist
	//std::cout << "Current poly cleave size: " << in_polyAPtr->cleaveMap.size() << std::endl;
	//in_polyAPtr->printAllCleaveLines();
	//if (categorizedLineDebugLevel == PolyDebugLevel::DEBUG)
	//{
		//std::cout << "(SPolySet): (ENDED) CategorizedLine debug option found for SPoly having ID " << in_hostPolyAID << "; comparing against SPoly with ID " << in_guestPolyID << "." << std::endl;
		//int someVal = 3;
		//std::cin >> someVal;
	//}
	currentComparisonLogger.log("(SPolySet): (ENDED)  CategorizedLine debug option found for SPoly having ID ", in_hostPolyAID, "; comparing against SPoly with ID ", in_guestPolyID, ".", "\n");
	currentComparisonLogger.waitForDebugInput();

	//std::cout << "########################## End intersection check. " << std::endl;
	
	return numberOfIntersections;
}

void SPolySet::performFracturing()
{
	/*
	std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
	for (int x = 0; x < secondaryPolys.size(); x++)
	{
		std::cout << "~~~ SPoly at index " << x << " has " << secondaryPolys[x].triangles.size() << " STriangles in it. " << std::endl;
	}
	std::cout << "!! enter number to continue with performFracturing function call. " << std::endl;
	int continueVal = 3;
	std::cin >> continueVal;
	*/

	// determine what needs to be fractured, by cycling through all secondaryPolys.
	 //std::cout << "|||||||||||||| ************* Beginning fracturing attempts for all polys ************** ||||||||||||||" << std::endl;
	auto truestart = std::chrono::high_resolution_clock::now();
	for (int x = 0; x < secondaryPolys.size(); x++)
	{
		
		//std::cout << "Cleave map size for poly " << x << ": " << secondaryPolys[x].cleaveMap.size() << std::endl;
		if (secondaryPolys[x].cleaveMap.size() != 0)	// fracture only if there are actual cleave map entries
		{
			// pass the secondary poly to the PolyFracturer
			
			std::cout << "########## Performing fracturing for poly with ID: " << x << std::endl;
			
			auto truestart = std::chrono::high_resolution_clock::now();
			//SPolyFracturer fracturer(x, &secondaryPolys[x], &polyMorphTracker, SPolyFracturerOptionEnum::ROTATE_TO_Z, checkIfSpecificSPolyFracturingDebugIsSet(x));
			
			SPolyFracturer fracturer(x, &secondaryPolys[x], &polyMorphTracker, SPolyFracturerOptionEnum::ROTATE_TO_Z, checkForSPolyOptionInSpecificSPoly(x, SPolyDO::FRACTURER));
			insertPolyFracturingResults(x, fracturer.sPolySG);

			auto trueend = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> trueelapsed = trueend - truestart;
			std::cout << "#-> Fracturing Time   > " << trueelapsed.count() << std::endl;

			int dumbVal = 5;
			//std::cout << "!!! Waiting for dumb input..." << std::endl;
			//std::cin >> dumbVal;
		}
		else if (secondaryPolys[x].cleaveMap.size() == 0)
		{
			insertOriginalPolyAsFracturingResult(x, secondaryPolys[x]);
			//std::cout << "!! No cleaveMaps found for this SPoly! [" << x << "] " << std::endl;
		}

	}

	// test only: let's make sure our resulting polys are grouped appropriately:
	/*
	std::cout << "################################## Printing out produced contents of the SPolySet: " << std::endl;
	auto polyResultsBegin = polyFracturingResults.begin();
	auto polyResultsEnd = polyFracturingResults.end();
	for (; polyResultsBegin != polyResultsEnd; polyResultsBegin++)
	{
		std::cout << ">> Parent SPoly ID: " << polyResultsBegin->first << ", child SPolys: " << std::endl;
		auto childSPolyVectorBegin = polyResultsBegin->second.begin();
		auto childSPolyVectorEnd = polyResultsBegin->second.end();
		int currentID = 0;
		for (; childSPolyVectorBegin != childSPolyVectorEnd; childSPolyVectorBegin++)
		{
			std::cout << "[" << currentID << "] " << std::endl;
			for (int x = 0; x < 3; x++)
			{
				std::cout << "point " << x << ": " << (*childSPolyVectorBegin).triangles[0].triangleLines[x].pointA.x << ", " << (*childSPolyVectorBegin).triangles[0].triangleLines[x].pointA.y << ", " << (*childSPolyVectorBegin).triangles[0].triangleLines[x].pointA.z << std::endl;
			}
			currentID++;
		}
	}
	*/

	auto trueend = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> trueelapsed2 = trueend - truestart;
	//std::cout << "#-> Total Fracturing Time !!  > " << trueelapsed2.count() << std::endl;

	//int continueVal = 3;
	//std::cout << "Fracturing for this poly complete; enter number to continue..." << std::endl;
	//std::cin >> continueVal;

	removeSPolysFlaggedAsPurgable();
	std::cout << "----> reached end of performFracturing()" << std::endl;
}

void SPolySet::insertPolyFracturingResults(int in_originalSPolyID, SPolySupergroup in_producedSupergroup)
{
	/*
	auto producedSPolyVectorBegin = (*in_producedSPolyVectorRef).begin();
	auto producedSPolyVectorEnd = (*in_producedSPolyVectorRef).end();
	for (; producedSPolyVectorBegin != producedSPolyVectorEnd; producedSPolyVectorBegin++)
	{
		polyFracturingResults[in_originalSPolyID].push_back(*producedSPolyVectorBegin);
	}
	*/
	polyFracturingResults.supergroupMap[in_originalSPolyID] = in_producedSupergroup;
}

void SPolySet::insertOriginalPolyAsFracturingResult(int in_originalSPolyID, SPoly in_sPoly)
{
	//polyFracturingResults[in_originalSPolyID].push_back(in_sPoly);
	polyFracturingResults.supergroupMap[in_originalSPolyID].insertSPoly(in_sPoly);
}

void SPolySet::runTest1()		// runs use case 1 
{
	// the first poly
	SPoly polyA;
	glm::vec3 pA_0, pA_1, pA_2, pA_3;
	glm::vec3 pA_MRP;		// mass reference point for polygon A

	pA_MRP.x = 0.5f;
	pA_MRP.y = 0.5f;
	pA_MRP.z = -0.5f;

	pA_0.x = 0.0f;
	pA_0.y = 0.0f;
	pA_0.z = 0.0f;

	pA_1.x = 1.0f;
	pA_1.y = 0.0f;
	pA_1.z = 0.0f;

	pA_2.x = 1.0f;
	pA_2.y = 1.0f;
	pA_2.z = 0.0f;

	pA_3.x = 0.0f;
	pA_3.y = 1.0f;
	pA_3.z = 0.0f;

	STriangle polyATriangle(pA_0, pA_1, pA_2);
	STriangle polyATriangle2(pA_0, pA_2, pA_3);
	polyA.groupID = 0;		// set the group ID to 0 (default anyway)
	polyA.setMRP(pA_MRP);
	polyA.addTriangle(polyATriangle);
	polyA.addTriangle(polyATriangle2);
	//polyA.calculateEmptyNormal();


	// the second poly
	SPoly polyB;
	glm::vec3 pB_0, pB_1, pB_2, pB_3;
	glm::vec3 pB_MRP;		// mass reference point for polygon B

	pB_MRP.x = 0.5f;
	pB_MRP.y = 0.2f;
	pB_MRP.z = 0.0f;

	pB_0.x = 1.0f;
	pB_0.y = .4f;
	pB_0.z = .5f;

	pB_1.x = 1.0f;
	pB_1.y = .5f;
	pB_1.z = -.5f;

	pB_2.x = 0.00f;
	pB_2.y = .5f;
	pB_2.z = -.5f;

	pB_3.x = 0.00f;
	pB_3.y = .4f;
	pB_3.z = .5f;




	//pB_1.x = 0.01f;
	//pB_1.y = .5f;
	//pB_1.z = .5f;

	//pB_2.x = 1.0f;
	//pB_2.y = .5f;
	//pB_2.z = -.5f;



	STriangle polyBTriangle(pB_0, pB_1, pB_2);
	STriangle polyBTriangle2(pB_0, pB_2, pB_3);
	polyB.groupID = 1;		// set the group ID to 1
	polyB.setMRP(pB_MRP);
	polyB.addTriangle(polyBTriangle);
	polyB.addTriangle(polyBTriangle2);
	//polyB.calculateEmptyNormal();
	//polyB.setDebugFlag(1);

	addPoly(polyA);
	addPoly(polyB);
	configurePolys();           // PHASE 1 (determine border lines, find empty normals)

	runPolyComparison(MassZoneBoxType::BLOCK);		// PHASE 2 (generate cleave lines)

	performFracturing();			// PHASE 3 run weave for new triangles thqat will be fractured

	reset();
}

void SPolySet::runTest2()
{
	// the first poly. group 0 

	SPoly polyA;
	glm::vec3 pA_0, pA_1, pA_2, pA_3;
	pA_0.x = 0.0f;
	pA_0.y = 0.0f;
	pA_0.z = 0.0f;

	pA_1.x = 1.0f;
	pA_1.y = 0.0f;
	pA_1.z = 0.0f;

	pA_2.x = 1.0f;
	pA_2.y = 1.0f;
	pA_2.z = 0.0f;

	pA_3.x = 0.0f;
	pA_3.y = 1.0f;
	pA_3.z = 0.0f;

	STriangle polyATriangle(pA_0, pA_1, pA_2);
	STriangle polyATriangle2(pA_0, pA_2, pA_3);
	polyA.groupID = 0;		// set the group ID to 0 (default anyway)
	polyA.addTriangle(polyATriangle);
	polyA.addTriangle(polyATriangle2);






	// the second poly, group 1
	SPoly polyB;
	glm::vec3 pB_0, pB_1, pB_2, pB_3;

	pB_0.x = 1.0f;
	pB_0.y = .4f;
	pB_0.z = .5f;

	pB_1.x = 1.0f;
	pB_1.y = .4f;
	pB_1.z = -.5f;

	pB_2.x = 0.00f;
	pB_2.y = .4f;
	pB_2.z = -.5f;

	pB_3.x = 0.00f;
	pB_3.y = .4f;
	pB_3.z = .5f;

	STriangle polyBTriangle(pB_0, pB_1, pB_2);
	STriangle polyBTriangle2(pB_0, pB_2, pB_3);
	polyB.groupID = 1;		// set the group ID to 1
	polyB.addTriangle(polyBTriangle);
	polyB.addTriangle(polyBTriangle2);


	// second poly, group 2
	SPoly polyC;
	glm::vec3 pC_0, pC_1, pC_2, pC_3;
	pC_0.x = 1.0f;
	pC_0.y = .8f;
	pC_0.z = .5f;

	pC_1.x = 1.0f;
	pC_1.y = .8f;
	pC_1.z = -.5f;

	pC_2.x = 0.00f;
	pC_2.y = .8f;
	pC_2.z = -.5f;

	pC_3.x = 0.00f;
	pC_3.y = .8f;
	pC_3.z = .5f;

	STriangle polyCTriangle(pC_0, pC_1, pC_2);
	STriangle polyCTriangle2(pC_0, pC_2, pC_3);
	polyC.groupID = 2;
	polyC.addTriangle(polyCTriangle);
	polyC.addTriangle(polyCTriangle2);

	addPoly(polyA);
	addPoly(polyB);
	addPoly(polyC);
	configurePolys();

	runPolyComparison(MassZoneBoxType::BLOCK);		// PHASE 1 (generate cleave lines) -- (see email, 8/20/19)
	performFracturing();			// PHASE 3 run weave for new triangles

	reset();

	std::cout << "!!!!! ~~~~~ Test 2 output results, for polyA: " << std::endl;
	secondaryPolys[0].printAllCleaveLines();
}

void SPolySet::runTest3()
{
	// the first poly
	SPoly polyA;
	glm::vec3 pA_0, pA_1, pA_2, pA_3;
	pA_0.x = 0.0f;
	pA_0.y = 0.0f;
	pA_0.z = 0.0f;

	pA_1.x = 1.0f;
	pA_1.y = 0.0f;
	pA_1.z = 0.0f;

	pA_2.x = 1.0f;
	pA_2.y = 1.0f;
	pA_2.z = 0.0f;

	pA_3.x = 0.0f;
	pA_3.y = 1.0f;
	pA_3.z = 0.0f;

	STriangle polyATriangle(pA_0, pA_1, pA_2);
	//STriangle polyATriangle2(pA_0, pA_2, pA_3);
	polyA.groupID = 0;		// set the group ID to 0 (default anyway)
	polyA.addTriangle(polyATriangle);
	//polyA.addTriangle(polyATriangle2);



	// the second poly
	SPoly polyB;
	glm::vec3 pB_0, pB_1, pB_2, pB_3;

	pB_0.x = 0.9f;
	pB_0.y = .5f;
	pB_0.z = .5f;

	pB_1.x = 0.9f;
	pB_1.y = .5f;
	pB_1.z = -.5f;

	pB_2.x = 0.40f;
	pB_2.y = .5f;
	pB_2.z = -.5f;

	pB_3.x = 0.00f;
	pB_3.y = .5f;
	pB_3.z = .5f;




	//pB_1.x = 0.01f;
	//pB_1.y = .5f;
	//pB_1.z = .5f;

	//pB_2.x = 1.0f;
	//pB_2.y = .5f;
	//pB_2.z = -.5f;



	STriangle polyBTriangle(pB_0, pB_1, pB_2);
	//STriangle polyBTriangle2(pB_0, pB_2, pB_3);
	polyB.groupID = 1;		// set the group ID to 1
	polyB.addTriangle(polyBTriangle);
	//polyB.addTriangle(polyBTriangle2);

	addPoly(polyA);
	addPoly(polyB);
	configurePolys();

	runPolyComparison(MassZoneBoxType::BLOCK);		// PHASE 1 (generate cleave lines) (see email, 8/20/2019)rc
	performFracturing();			// PHASE 3 run weave for new triangles

	reset();
}

void SPolySet::runTest4()
{
	// the first poly
	SPoly polyA;
	glm::vec3 pA_0, pA_1, pA_2, pA_3;
	glm::vec3 pA_MRP;		// mass reference point for polygon A

	pA_MRP.x = 0.5f;
	pA_MRP.y = 0.5f;
	pA_MRP.z = -0.5f;

	pA_0.x = 0.0f;
	pA_0.y = 0.0f;
	pA_0.z = 0.0f;

	pA_1.x = 1.0f;
	pA_1.y = 0.0f;
	pA_1.z = 0.0f;

	pA_2.x = 1.0f;
	pA_2.y = 1.0f;
	pA_2.z = 0.0f;

	pA_3.x = 0.0f;
	pA_3.y = 1.0f;
	pA_3.z = 0.0f;

	STriangle polyATriangle(pA_0, pA_1, pA_2);
	STriangle polyATriangle2(pA_0, pA_2, pA_3);
	polyA.groupID = 0;		// set the group ID to 0 (default anyway)
	polyA.setMRP(pA_MRP);
	polyA.addTriangle(polyATriangle);
	polyA.addTriangle(polyATriangle2);

	addPoly(polyA);
	//polyA.calculateEmptyNormal();


	// MRP for group 1
	glm::vec3 group_1_MRP;
	group_1_MRP.x = 1.0f;
	group_1_MRP.y = .5f;
	group_1_MRP.z = 0.0f;

	// first poly of group 1 (top poly)
	SPoly polyA_1;

	// poly A, in group 1, points 0 through 3...
	glm::vec3 pA_1_0, pA_1_1, pA_1_2, pA_1_3;
	pA_1_0.x = 1.0f;
	pA_1_0.y = .6f;
	pA_1_0.z = 0.1f;

	pA_1_1.x = 1.0f;
	pA_1_1.y = .6f;
	pA_1_1.z = -0.1f;

	pA_1_2.x = 0.9f;
	pA_1_2.y = .6f;
	pA_1_2.z = -0.1f;

	pA_1_3.x = 0.9f;
	pA_1_3.y = 0.6f;
	pA_1_3.z = 0.1f;

	STriangle polyA_1_triangle1(pA_1_0, pA_1_1, pA_1_2);
	STriangle polyA_1_triangle2(pA_1_0, pA_1_2, pA_1_3);
	polyA_1.groupID = 1;
	polyA_1.setMRP(group_1_MRP);
	polyA_1.addTriangle(polyA_1_triangle1);
	polyA_1.addTriangle(polyA_1_triangle2);

	addPoly(polyA_1);



	// second poly of group 1 (side poly)
	SPoly polyB_1;
	
	// poly B, in group 1, points 0 through 3...
	glm::vec3 pB_1_0, pB_1_1, pB_1_2, pB_1_3;
	pB_1_0.x = .9f;
	pB_1_0.y = .6f;
	pB_1_0.z = 0.1f;

	pB_1_1.x = .9f;
	pB_1_1.y = .6f;
	pB_1_1.z = -0.1f;

	pB_1_2.x = .9f;
	pB_1_2.y = .4f;
	pB_1_2.z = -0.1f;

	pB_1_3.x = .9f;
	pB_1_3.y = .4f;
	pB_1_3.z = 0.1f;

	STriangle polyB_1_triangle1(pB_1_0, pB_1_1, pB_1_2);
	STriangle polyB_1_triangle2(pB_1_0, pB_1_2, pB_1_3);
	polyB_1.groupID = 1;
	polyB_1.setMRP(group_1_MRP);
	polyB_1.addTriangle(polyB_1_triangle1);
	polyB_1.addTriangle(polyB_1_triangle2);

	addPoly(polyB_1);


	// third poly of group 1 (bottom poly)
	SPoly polyC_1;

	// poly C, in group 1, points 0 through 3...
	glm::vec3 pC_1_0, pC_1_1, pC_1_2, pC_1_3;
	pC_1_0.x = .9f;
	pC_1_0.y = .4f;
	pC_1_0.z = 0.1f;

	pC_1_1.x = 1.0f;
	pC_1_1.y = 0.4f;
	pC_1_1.z = 0.1f;

	pC_1_2.x = 1.0f;
	pC_1_2.y = 0.4f;
	pC_1_2.z = -0.1f;

	pC_1_3.x = 0.9f;
	pC_1_3.y = 0.4f;
	pC_1_3.z = -0.1f;

	STriangle polyC_1_triangle1(pC_1_0, pC_1_1, pC_1_2);
	STriangle polyC_1_triangle2(pC_1_0, pC_1_2, pC_1_3);
	polyC_1.groupID = 1;
	polyC_1.setMRP(group_1_MRP);
	polyC_1.addTriangle(polyC_1_triangle1);
	polyC_1.addTriangle(polyC_1_triangle2);

	addPoly(polyC_1);

	


	configurePolys();

	runPolyComparison(MassZoneBoxType::BLOCK);		// PHASE 1 (generate cleave lines) (see email, 8/20/2019)rc
	performFracturing();			// PHASE 3 run weave for new triangles

	reset();
}