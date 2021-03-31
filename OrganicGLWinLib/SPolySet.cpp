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
		if (secondaryPolys[x].sequenceFactory.doesFactoryContainLines() == true)
		{
			zoneMaster.disqualifyMeshMatterMeta(x);
		}
		std::cout << ">>>>>>> Building cleave sequences, for SPoly with ID: " << x << std::endl;
		secondaryPolys[x].buildCleaveSequences(CleaveSequenceMergeMode::MERGE);		
		//int buildVal = 3;
		//std::cin >> buildVal;

		//secondaryPolys[x].printAllCleaveLines();
		//std::cout << "+++++ Enter number to go to next poly. " << std::endl;
		//int someVal = 3;
		//std::cin >> someVal;
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
				CategorizedLineColinearTester tester(reactionResult.resultingLine, *hostTrianglePtr, coplanarTestDebugLevel);
				if (tester.colinearToBorderLineDetected == false)		// the categorized line isn't colinear to any line in the host triangle (remember, context is from host triangle)
				{
					in_hostPolyPtr->sequenceFactory.addCategorizedLine(reactionResult.resultingLine);
					numberOfIntersections++;
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

IntersectionResult SPolySet::checkIfLineIntersectsTriangle(STriangle in_triangle, STriangleLine in_line)
{
	
	IntersectionResult returnResult;

	// do typical compare:
	IntersectionResult resultA = checkIfRayIntersectsTriangle(in_triangle, in_line);

	// do a compare where the in_line is swapped
	STriangleLine swappedLine = in_line;
	swappedLine.pointA = in_line.pointB;
	swappedLine.pointB = in_line.pointA;

	// do the swapped compare
	IntersectionResult resultB = checkIfRayIntersectsTriangle(in_triangle, swappedLine);

	//std::cout << "!!!! Bi-dirrectional comparison results: " << std::endl;
	//std::cout << "ResultA, was found: " << resultA.wasIntersectFound << std::endl;
	//std::cout << "ResultB, was found: " << resultB.wasIntersectFound << std::endl;

	comparisonLogger.log("!!!! Bi-dirrectional comparison results: ", "\n");
	comparisonLogger.log("ResultA, was found: ", resultA.wasIntersectFound, "\n");
	comparisonLogger.log("ResultB, was found: ", resultB.wasIntersectFound, "\n");

	if
	(
		(resultA.wasIntersectFound != 0)
		&&
		(resultB.wasIntersectFound != 0)
	)
	{
		returnResult = resultA;
	}
	//
	//else
	//{
	//}
	return returnResult;
}

IntersectionResult SPolySet::checkIfRayIntersectsTriangle(STriangle in_triangle, STriangleLine in_triangleLine)
{
	//int someVal = 0;
	IntersectionResult returnResult;
	glm::vec3 intersect_candidate;

	std::cout << "triangle, point 0: " << in_triangle.triangleLines[0].pointA.x << ", " << in_triangle.triangleLines[0].pointA.y << ", " << in_triangle.triangleLines[0].pointA.z << std::endl;
	std::cout << "triangle, point 1: " << in_triangle.triangleLines[1].pointA.x << ", " << in_triangle.triangleLines[1].pointA.y << ", " << in_triangle.triangleLines[1].pointA.z << std::endl;
	std::cout << "triangle, point 2: " << in_triangle.triangleLines[2].pointA.x << ", " << in_triangle.triangleLines[2].pointA.y << ", " << in_triangle.triangleLines[2].pointA.z << std::endl;
	comparisonLogger.log("triangle, point 0: ", in_triangle.triangleLines[0].pointA.x, ", ", in_triangle.triangleLines[0].pointA.y, ", ", in_triangle.triangleLines[0].pointA.z, "\n");
	comparisonLogger.log("triangle, point 1: ", in_triangle.triangleLines[1].pointA.x, ", ", in_triangle.triangleLines[1].pointA.y, ", ", in_triangle.triangleLines[1].pointA.z, "\n");
	comparisonLogger.log("triangle, point 2: ", in_triangle.triangleLines[2].pointA.x, ", ", in_triangle.triangleLines[2].pointA.y, ", ", in_triangle.triangleLines[2].pointA.z, "\n");

	/*
	for (int x = 0; x < 3; x++)
	{
		if (in_triangle.triangleLines[x].isBorderLine == 1)
		{
			std::cout << "!! line at index " << x << " in this triangle, is a border line; border line value is: " << int(in_triangle.triangleLines[x].borderLineID) << std::endl;
		}
	}
	*/

	std::cout << "Checking if this line intersects: pointA: " << in_triangleLine.pointA.x << ", " << in_triangleLine.pointA.y << ", " << in_triangleLine.pointA.z << " | pointB: " << in_triangleLine.pointB.x << ", " << in_triangleLine.pointB.y << ", " << in_triangleLine.pointB.z << std::endl;
	//std::cout << "=========" << std::endl;
	comparisonLogger.log("Checking if this line intersects: pointA: ", in_triangleLine.pointA.x, ", ", in_triangleLine.pointA.y, ", ", in_triangleLine.pointA.z, " | pointB: ", in_triangleLine.pointB.x, ", ", in_triangleLine.pointB.y, ", ", in_triangleLine.pointB.z, "\n");
	comparisonLogger.log("=========", "\n");

	glm::vec3 point0 = in_triangle.triangleLines[0].pointA;
	glm::vec3 point1 = in_triangle.triangleLines[1].pointA;
	glm::vec3 point2 = in_triangle.triangleLines[2].pointA;


	int matchCount = 0;

	// attempt match of point A of the triangleLine to all 3 points:
	for (int x = 0; x < 3; x++)
	{
		if (in_triangleLine.pointA == in_triangle.triangleLines[x].pointA)
		{
			matchCount++;
		}
	}

	// ...of point B
	for (int x = 0; x < 3; x++)
	{
		if (in_triangleLine.pointB == in_triangle.triangleLines[x].pointA)
		{
			matchCount++;
		}
	}

	//std::cout << "##### Matchcount is: " << matchCount << std::endl;

	// condition one, matchCount is 2
	if (matchCount == 2)
	{
		//std::cout << "~~~~~~~~~~~~~~~~ note special case; the triangleLine matches a line exactly in the incoming triangle. flagging as 2." << std::endl;
		//int someVal = 3;
		//std::cin >> someVal;
		returnResult.setResult(3);		// set flag indicatiing that the passed in STriangleLine is equal to a STriangleLine that belongs to the passed in STriangle.
	}

	// condition two, matchCount is 1
	else if (matchCount == 1)
	{
		std::cout << "!! Match count 1 hit.  " << std::endl;

		if
			(
			(in_triangleLine.pointA == point0)
				||
				(in_triangleLine.pointA == point1)
				||
				(in_triangleLine.pointA == point2)

				||
				(in_triangleLine.pointB == point0)
				||
				(in_triangleLine.pointB == point1)
				||
				(in_triangleLine.pointB == point2)
				)
		{
			if (in_triangleLine.pointA == point0)
			{
				//std::cout << "!-> Intersection notice: point A of the line, equals point0." << std::endl;
				returnResult.setResult(1);
				returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
				returnResult.borderLineID = in_triangleLine.borderLineID;
				returnResult.intersectedPoint = roundPointToHundredths(point0);					// the intercept point should be rounded to hundredths!
			}
			else if (in_triangleLine.pointA == point1)
			{
				//std::cout << "!-> Intersection notice: point A of the line, equals point1." << std::endl;
				returnResult.setResult(1);
				returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
				returnResult.borderLineID = in_triangleLine.borderLineID;
				returnResult.intersectedPoint = roundPointToHundredths(point1);					// the intercept point should be rounded to hundredths!
			}

			else if (in_triangleLine.pointA == point2)
			{
				//std::cout << "!-> Intersection notice: point A of the line, equals point2." << std::endl;
				returnResult.setResult(1);
				returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
				returnResult.borderLineID = in_triangleLine.borderLineID;
				returnResult.intersectedPoint = roundPointToHundredths(point2);					// the intercept point 
			}
			else if (in_triangleLine.pointB == point0)
			{
				//std::cout << "!-> Intersection notice: point B of the line, equals point0." << std::endl;
				returnResult.setResult(1);
				returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
				returnResult.borderLineID = in_triangleLine.borderLineID;
				returnResult.intersectedPoint = roundPointToHundredths(point0);					// the intercept point 
			}
			else if (in_triangleLine.pointB == point1)
			{
				//std::cout << "!-> Intersection notice: point B of the line, equals point1." << std::endl;
				returnResult.setResult(1);
				returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
				returnResult.borderLineID = in_triangleLine.borderLineID;
				returnResult.intersectedPoint = roundPointToHundredths(point1);					// the intercept point 
			}
			else if (in_triangleLine.pointB == point2)
			{
				//std::cout << "!-> Intersection notice: point B of the line, equals point2." << std::endl;
				returnResult.setResult(1);
				returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
				returnResult.borderLineID = in_triangleLine.borderLineID;
				returnResult.intersectedPoint = roundPointToHundredths(point2);					// the intercept point 
			}
		}
	}
	else if (matchCount == 0)
	{
		int tempDebug = 0;
	
		/*
		std::cout << "##########################################" << std::endl;
		std::cout << "triangle, point 0: " << point0.x << ", " << point0.y << ", " << point0.z << std::endl;
		std::cout << "triangle, point 1: " << point1.x << ", " << point1.y << ", " << point1.z << std::endl;
		std::cout << "triangle, point 2: " << point2.x << ", " << point2.y << ", " << point2.z << std::endl;
		std::cout << "intersecting line, point A: " << in_triangleLine.pointA.x << ", " << in_triangleLine.pointA.y << ", " << in_triangleLine.pointA.z << std::endl;
		std::cout << "intersecting line, point B: " << in_triangleLine.pointB.x << ", " << in_triangleLine.pointB.y << ", " << in_triangleLine.pointB.z << std::endl;
		std::cout << "##########################################" << std::endl;
		*/

		glm::vec3 u = point1 - point0;	// u vector
		glm::vec3 v = point2 - point0;	// v " 

		//std::cout << "U: " << u.x << ", " << u.y << ", " << u.z << std::endl;
		//std::cout << "V: " << v.x << ", " << v.y << ", " << v.z << std::endl;
		glm::vec3 N = cross(u, v);		// the normal of the triangle
		//N.z = -1.0f;


		glm::vec3 dir = in_triangleLine.pointB - in_triangleLine.pointA;
		glm::vec3 w0 = in_triangleLine.pointA - point0;

		//std::cout << "Normal: " << N.x << ", " << N.y << ", " << N.z << std::endl;
		//std::cout << "Dir: " << dir.x << ", " << dir.y << ", " << dir.z << std::endl;

		float a = -dot(N, w0);
		float b = dot(N, dir);
		double doubleb = doubledot(N, dir);


		//std::cout <<std::setprecision(9);
		std::cout << "(a) is: " << a << std::endl;
		std::cout << "(b) is: " << b << std::endl;
		//std::cout << "(doubleb) is: " << doubleb << std::endl;

		// remember, if SMALL_NUM is too sensitive (i.e., too precise in the IEEE float), there may be a "miss" on detecting when
		// the passed-in STriangleLine's two points exactly match two points in the STriangle. 
		if (fabs(b) < SMALL_NUM) {     // ray is  parallel to triangle plane

			//std::cout << "!!! Logic for less than SMALL_NUM entered....enter number to continue. " << std::endl;
			//int someValAwesome = 3;
			//std::cin >> someValAwesome;

			//if (a == 0)                 // ray lies in triangle plane
			//if (a < SMALL_NUM)

			if							  // ray lies in triangle plane
			(
				(a < SMALL_NUM)
				&&
				(a > (SMALL_NUM*-1.0f))
			)
			{
				std::cout << "::> Line is lies within triangle. " << std::endl;
				//comparisonLogger.log("::> Line is lies within triangle. ", "\n");
				returnResult.setResult(2);
			}
			//else return 0;              // ray disjoint from plane
			else returnResult.setResult(0);
		}

		float r = a / b;
		if (r < 0.0)
		{
			//std::cout << "!! Note: r is  less than 0. " << std::endl;
			//return 0;
			returnResult.setResult(0);
		}

		intersect_candidate = in_triangleLine.pointA + (r * dir);

		// is I inside T?
		glm::vec3 w;
		float    uu, uv, vv, wu, wv, D;
		uu = dot(u, u);
		uv = dot(u, v);
		vv = dot(v, v);
		w = intersect_candidate - point0;
		wu = dot(w, u);
		wv = dot(w, v);
		D = uv * uv - uu * vv;

		// get and test parametric coords
		float s, t;
		s = (uv * wv - vv * wu) / D;
		s = float(floor(s * 1000 + 0.5) / 1000);

		//std::cout << "--> Value of s: " << s << std::endl;
		comparisonLogger.log("--> Value of s: ", s, "\n");
		if (s < 0.0 || s > 1.001f)         // I is outside S
			//return 0;
		{
			//std::cout << "!! Note: I is outside S. " << std::endl;
			returnResult.setResult(0);
		}
		t = (uv * wu - uu * wv) / D;
		t = float(floor(t * 1000 + 0.5) / 1000);

		//std::cout << "--> Value of t: " << t << std::endl;
		comparisonLogger.log("--> Value of t: ", t, "\n");
		if (t < 0.0 || (s + t) > 1.001f)  // I is outside T
			//return 0;
		{
			//std::cout << "!! Note: I is outside T. " << std::endl;
			comparisonLogger.log("!! Note: I is outside T. ", "\n");
			returnResult.setResult(0);
		}
		//return 1;
		returnResult.setResult(1);
		returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
		returnResult.borderLineID = in_triangleLine.borderLineID;
		returnResult.intersectedPoint = roundPointToHundredths(intersect_candidate);					// the intercept point should be rounded to hundredths!



		//std::cout << "Resulting intersect attempt value: " << returnResult.wasIntersectFound << std::endl;
		//std::cout << "### Intersected point for targeted debug point is: " << returnResult.intersectedPoint.x << ", " << returnResult.intersectedPoint.y << ", " << returnResult.intersectedPoint.z << std::endl;

		//if (returnResult.wasIntersectFound != 0)
		//{
			//std::cout << "### Intersected point for targeted debug point is: " << returnResult.intersectedPoint.x << ", " << returnResult.intersectedPoint.y << ", " << returnResult.intersectedPoint.z << std::endl;
		//}

		//if (tempDebug == 1)
		//{
			//std::cout << "### Intersected point for targeted debug point is: " << returnResult.intersectedPoint.x << ", " << returnResult.intersectedPoint.y << ", " << returnResult.intersectedPoint.z << std::endl;
			//int someVal = 3;
			//std::cin >> someVal;
		//}
	}
	return returnResult;
}

IntersectionResult SPolySet::checkIfRayIntersectsTriangleSpecial(STriangle in_triangle, STriangleLine in_triangleLine)
{
	//int someVal = 0;
	IntersectionResult returnResult;
	glm::vec3 intersect_candidate;

	//std::cout << "Checking if this line intersects: pointA: " << in_triangleLine.pointA.x << ", " << in_triangleLine.pointA.y << ", " << in_triangleLine.pointA.z << " | pointB: " << in_triangleLine.pointB.x << ", " << in_triangleLine.pointB.y << ", " << in_triangleLine.pointB.z << std::endl;

	glm::vec3 point0 = in_triangle.triangleLines[0].pointA;
	glm::vec3 point1 = in_triangle.triangleLines[1].pointA;
	glm::vec3 point2 = in_triangle.triangleLines[2].pointA;

	int tempDebug = 0;
	/*
	if
	(
		//(in_triangleLine.pointA.x == 4.0f)
		//&&
		//(in_triangleLine.pointA.y == 0.0f)
		//&&
		//(in_triangleLine.pointA.z == 0.0f)

		//&&

		//(in_triangleLine.pointB.x == 4.0f)
		//&&
		//(in_triangleLine.pointB.y == 4.0f)
		//&&
		//(in_triangleLine.pointB.z == 0.0f)
	)
	{
		std::cout << "######## checkIfRayIntersectsTriangle, targeted point found! ##################-----------------> " << std::endl;
		std::cout << "Points of triangle line are: " << std::endl;
		std::cout << "Point A: " << in_triangleLine.pointA.x << ", " << in_triangleLine.pointA.y << ", " << in_triangleLine.pointA.z << std::endl;
		std::cout << "Point B: " << in_triangleLine.pointB.x << ", " << in_triangleLine.pointB.y << ", " << in_triangleLine.pointB.z << std::endl;
		tempDebug = 1;
	}
	*/

	/*
	std::cout << "##########################################" << std::endl;
	std::cout << "triangle, point 0: " << point0.x << ", " << point0.y << ", " << point0.z << std::endl;
	std::cout << "triangle, point 1: " << point1.x << ", " << point1.y << ", " << point1.z << std::endl;
	std::cout << "triangle, point 2: " << point2.x << ", " << point2.y << ", " << point2.z << std::endl;
	std::cout << "intersecting line, point A: " << in_triangleLine.pointA.x << ", " << in_triangleLine.pointA.y << ", " << in_triangleLine.pointA.z << std::endl;
	std::cout << "intersecting line, point B: " << in_triangleLine.pointB.x << ", " << in_triangleLine.pointB.y << ", " << in_triangleLine.pointB.z << std::endl;
	std::cout << "##########################################" << std::endl;
	*/

	glm::vec3 u = point1 - point0;	// u vector
	glm::vec3 v = point2 - point0;	// v " 

	//std::cout << "U: " << u.x << ", " << u.y << ", " << u.z << std::endl;
	//std::cout << "V: " << v.x << ", " << v.y << ", " << v.z << std::endl;
	glm::vec3 N = cross(u, v);		// the normal of the triangle
	//N.z = -1.0f;
	//std::cout << "Normal: " << N.x << ", " << N.y << ", " << N.z << std::endl;

	glm::vec3 dir = in_triangleLine.pointB - in_triangleLine.pointA;
	glm::vec3 w0 = in_triangleLine.pointA - point0;

	float a = -dot(N, w0);
	float b = dot(N, dir);
	if (fabs(b) < SMALL_NUM) {     // ray is  parallel to triangle plane

		std::cout << "!! Warning, B is less than SMALL_NUM. " << std::endl;
		if (a == 0)                 // ray lies in triangle plane
			//return 2;
			returnResult.setResult(2);
		//else return 0;              // ray disjoint from plane
		else returnResult.setResult(0);
	}

	std::cout << "A is: " << a << std::endl;
	std::cout << "B is: " << b << std::endl;

	float r = a / b;
	if (r < 0.0)
		//return 0;
		returnResult.setResult(0);

	std::cout << "R is: " << r << std::endl;
	std::cout << "Dir is: " << dir.x << ", " << dir.y << ", " << dir.z << std::endl;

	intersect_candidate = in_triangleLine.pointA + (r * dir);

	// is I inside T?
	glm::vec3 w;
	float    uu, uv, vv, wu, wv, D;
	uu = dot(u, u);
	uv = dot(u, v);
	vv = dot(v, v);
	w = intersect_candidate - point0;
	wu = dot(w, u);
	wv = dot(w, v);
	D = uv * uv - uu * vv;

	// get and test parametric coords
	float s, t;
	s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0)         // I is outside T
		//return 0;
		returnResult.setResult(0);
	t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		//return 0;
		returnResult.setResult(0);
	//return 1;
	returnResult.setResult(1);
	returnResult.wasIntersectOnBorderLine = in_triangleLine.isBorderLine;
	returnResult.borderLineID = in_triangleLine.borderLineID;
	returnResult.intersectedPoint = roundPointToHundredths(intersect_candidate);					// the intercept point should be rounded to hundredths!


	std::cout << "### Was intersect found result will be: " << returnResult.wasIntersectFound << std::endl;
	std::cout << "### Intersected point for targeted debug point is: " << returnResult.intersectedPoint.x << ", " << returnResult.intersectedPoint.y << ", " << returnResult.intersectedPoint.z << std::endl;
	int someVal = 3;
	std::cin >> someVal;
	

	return returnResult;
}

glm::vec3 SPolySet::cross(glm::vec3 in_A, glm::vec3 in_B)
{
	glm::vec3 returnVec;

	returnVec.x = (in_A.y * in_B.z) - (in_A.z * in_B.y);
	returnVec.y = (in_A.z * in_B.x) - (in_A.x * in_B.z);
	returnVec.z = (in_A.x * in_B.y) - (in_A.y * in_B.x);

	return returnVec;
}

float SPolySet::dot(glm::vec3 in_A, glm::vec3 in_B)
{
	float x_multiplied = in_A.x * in_B.x;
	float y_multiplied = in_A.y * in_B.y;
	float z_multiplied = in_A.z * in_B.z;
	return x_multiplied + y_multiplied + z_multiplied;
}

double SPolySet::doubledot(glm::vec3 in_A, glm::vec3 in_B)
{
	double x_multiplied = in_A.x * in_B.x;
	double y_multiplied = in_A.y * in_B.y;
	double z_multiplied = in_A.z * in_B.z;
	return x_multiplied + y_multiplied + z_multiplied;
}

glm::vec3 SPolySet::roundPointToHundredths(glm::vec3 in_point)
{
	glm::vec3 returnPoint;
	returnPoint.x = float(floor(in_point.x * 100 + 0.5) / 100);
	returnPoint.y = float(floor(in_point.y * 100 + 0.5) / 100);
	returnPoint.z = float(floor(in_point.z * 100 + 0.5) / 100);
	return returnPoint;
}

int SPolySet::checkIfPointsMatch(glm::vec3 in_pointA, glm::vec3 in_pointB)
{
	int matchFound = 0;
	if
		(
		(in_pointA.x == in_pointB.x)
			&&
			(in_pointA.y == in_pointB.y)
			&&
			(in_pointA.z == in_pointB.z)
			)
	{
		matchFound = 1;
	}
	return matchFound;
}

float SPolySet::getRadiansForPosZViaY(glm::vec3 in_vec3)
{
	float degreesToRotateOnY = 0.0f;
	float fullRadian360 = 6.28319f;	// 360 degrees = this many radians


	// get the atan2 result, and analyze it
	float atan2result = atan2(in_vec3.z, in_vec3.x);
	float firstPassRotateRadians;
	std::cout << "Atan result is: " << atan2result << std::endl;
	if (atan2result > 0.0)
	{
		//firstPassRotateRadians = fullRadian360 - atan2result;
		firstPassRotateRadians = atan2result;
	}
	else if (atan2result < 0.0) // if a is less than 0, add the result to fullRadian360 to get the amount to rotate by. (the quat goes CW when the rotation axis is pointing in a positive direction)
	{
		//firstPassRotateRadians = abs(atan2result);
		firstPassRotateRadians = fullRadian360 + atan2result;
	}

	glm::vec3 rotationAngle;
	rotationAngle.y = 1;
	glm::quat rotationQuat = createQuaternion(firstPassRotateRadians, rotationAngle);
	glm::vec3 rotationResult = rotationQuat * in_vec3;

	glm::vec3 targetPosZ;
	targetPosZ.z = 1.0f;
	targetPosZ = rotationQuat * targetPosZ;

	std::cout << "Rotated point is: " << rotationResult.x << ", " << rotationResult.y << ", " << rotationResult.z << std::endl;
	std::cout << "Target pos z is at: " << targetPosZ.x << ", " << targetPosZ.y << ", " << targetPosZ.z << std::endl;
	std::cout << "Rotated by this many radians: " << firstPassRotateRadians << std::endl;

	std::cout << ">>>> Attempting second pass rotation... " << std::endl;

	float secondPassAtan = atan2(targetPosZ.z, targetPosZ.x);
	std::cout << ">>>> Seond pass a-tan is: " << secondPassAtan << std::endl;
	float secondPassRotateRadians;
	if (secondPassAtan > 0.0)
	{
		secondPassRotateRadians = secondPassAtan;
	}
	else if (secondPassAtan < 0.0)
	{
		secondPassRotateRadians = fullRadian360 + secondPassAtan;
	}
	float secondPassRotateFinal = fullRadian360 - secondPassRotateRadians;
	glm::quat secondRotationQuat = createQuaternion(secondPassRotateFinal, rotationAngle);
	glm::vec3 finalRotatedVec = secondRotationQuat * in_vec3;
	std::cout << "Final rotated vec is: " << finalRotatedVec.x << ", " << finalRotatedVec.y << ", " << finalRotatedVec.z << std::endl;
	std::cout << "It was rotated to pos Z, via this many radians: " << secondPassRotateFinal << std::endl;

	return degreesToRotateOnY;
}

float SPolySet::getRadiansForPosYViaX(glm::vec3 in_vec3)
{
	float degreesToRotateOnX = 0.0f;
	float fullRadian360 = 6.28319f;	// 360 degrees = this many radians

	// get the atan2 result, and analyze it
	float atan2result = atan2(in_vec3.y, in_vec3.z);
	float firstPassRotateRadians;
	std::cout << "Atan result is: " << atan2result << std::endl;
	if (atan2result > 0.0)
	{
		//firstPassRotateRadians = fullRadian360 - atan2result;
		firstPassRotateRadians = atan2result;
	}
	else if (atan2result < 0.0) // if a is less than 0, add the result to fullRadian360 to get the amount to rotate by. (the quat goes CW when the rotation axis is pointing in a positive direction)
	{
		//firstPassRotateRadians = abs(atan2result);
		firstPassRotateRadians = fullRadian360 + atan2result;
	}

	glm::vec3 rotationAngle;
	rotationAngle.x = 1;
	glm::quat rotationQuat = createQuaternion(firstPassRotateRadians, rotationAngle);
	glm::vec3 rotationResult = rotationQuat * in_vec3;

	glm::vec3 targetPosY;
	targetPosY.y = 1.0f;
	targetPosY = rotationQuat * targetPosY;

	std::cout << "Rotated point is: " << rotationResult.x << ", " << rotationResult.y << ", " << rotationResult.z << std::endl;
	std::cout << "Target pos Y is at: " << targetPosY.x << ", " << targetPosY.y << ", " << targetPosY.z << std::endl;
	std::cout << "Rotated by this many radians: " << firstPassRotateRadians << std::endl;

	std::cout << ">>>> Attempting second pass rotation... " << std::endl;

	float secondPassAtan = atan2(targetPosY.y, targetPosY.z);
	std::cout << ">>>> Second pass a-tan is: " << secondPassAtan << std::endl;
	float secondPassRotateRadians;
	if (secondPassAtan > 0.0)
	{
		secondPassRotateRadians = secondPassAtan;
	}
	else if (secondPassAtan < 0.0)
	{
		secondPassRotateRadians = fullRadian360 + secondPassAtan;
	}
	float secondPassRotateFinal = fullRadian360 - secondPassRotateRadians;
	glm::quat secondRotationQuat = createQuaternion(secondPassRotateFinal, rotationAngle);
	glm::vec3 finalRotatedVec = secondRotationQuat * in_vec3;
	std::cout << "Final rotated vec is: " << finalRotatedVec.x << ", " << finalRotatedVec.y << ", " << finalRotatedVec.z << std::endl;
	std::cout << "It was rotated to pos Y, via this many radians: " << secondPassRotateFinal << std::endl;

	return degreesToRotateOnX;
}

glm::quat SPolySet::createQuaternion(float radians, glm::vec3 in_angle)
{
	glm::quat returnQuat;

	// a = angle of rotation, in radians
	// R = [cos(a/2), sin(a/2)*x, sin(a/2)*y, sin(a/2)*z] quaternion formula
	// 90 degrees = 1.5708 radians
	returnQuat = glm::quat(cos(radians / 2), sin(radians / 2)*in_angle.x, sin(radians / 2)*in_angle.y, sin(radians / 2)*in_angle.z);
	return returnQuat;
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
			
			//std::cout << "########## Performing fracturing for poly with ID: " << x << std::endl;
			
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