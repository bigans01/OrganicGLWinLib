#include "stdafx.h"
#include "SPolyShellProducer.h"

void SPolyShellProducer::addInputSPoly(SPoly in_inputSPoly)
{
	// Get the values to determine the condition branching that follows.
	BoundaryOrientation currentOrientation = in_inputSPoly.sPolyBoundaryIndicator.getBoundaryIndicatorValue();
	bool isScabParent = in_inputSPoly.sPolyBoundaryIndicator.isScabParentIndicatorSet();
	bool isScabChild = in_inputSPoly.sPolyBoundaryIndicator.isScabChildIndicatorSet();

	// CONDITION 1: no boundary indicators, no scabs, -- a perfectly normal SPoly.
	// These normal SPolys are responsible for producing all other SPolys that should exist on each
	// BoundaryOrientation (i.e, POS_X, NEG_Y). It is possible however that these normal SPolys cause abnormal
	// SPolys to be produced on the borders, which requires the use of an SPolyResolution. 
	if
	(
		currentOrientation == BoundaryOrientation::NONE
		&&
		isScabParent == false
		&&
		isScabChild == false
	)
	{
		// 2022_4_002 (1) -> make sure to uncomment/remove the inputSPolys call above
		inputSPolys[numberOfInputSPolys++] = in_inputSPoly;
	}

	// CONDITION 2: boundary indicators, but no scabs. These would be any boundary polys that are produced by normal means (i.e, no errors/exceptions); 
	// SPolySupergroup::buildSPolyBorderLines() is called on these supergroups, in Step 3-B of the MassZone::createMassZoneShell function.
	// These kinds of SPolys, although normal, shouldn't need to be reconstructed -- they should go in the SPolySupergroup map,
	// (existingLegitimateBoundarySupergroups) that we pass as a reference to productionMassZone.
	else if
	(
		currentOrientation != BoundaryOrientation::NONE
		&&
		isScabParent == false
		&&
		isScabChild == false
	)
	{
		// always: insert the excluded boundary, since it is not NONE.
		auto currentSPolyBoundaryOrientation = in_inputSPoly.getBoundaryIndicatorOrientation();
		excludedOrientations.insert(currentSPolyBoundaryOrientation);

		// since it's a legitimate SPoly with a valid BoundaryOrientation value, add it to the invalidBoundaries SPolySuperogroup map.
		existingLegitimateBoundarySupergroups[currentSPolyBoundaryOrientation].insertSPoly(in_inputSPoly);
	}

	// CONDITION 3: any scabs; MAY need to call SPolySupergroup::buildSPolyBorderLines(), but not sure yet.
	// A SCAB_PARENT or SCAB_CHILD CAN have a boundary flag set, but this is not mandatory.
	// It is important to remember that any SPolys produced by an SPolyResolution should have a SCAB_PARENT and/or SCAB_CHILD flag
	// on them. Additionally, all SCAB_PARENTS should have an border indicator flag set (i.e, POS_X), in addition to the SCAB_PARENT/SCAB_CHILD
	// flags. Note that each SPoly's mode member to be inserted should be in SPolyMode::MALFORMED_MITIGATION, since it's a SCAB_PARENT or SCAB_CHILD;
	// Doing this will prevent it from being used to generate the list in the function MassZoneBox::generateTouchedBoxFacesList.
	//
	// It needs to be noted here, that SCAB_PARENT or SCAB_CHILD SPolys cannot be relied upon to generate tertiary SPolys; this is 
	// the reason they are inserted at the end of the SPolyShellProducer::runSPolyShellConstruction function, 
	// but not before the call to MassZone::runFirstTertiaryProductionPassInZoneBox, as doing so could cause weird tertiary production artifacts.
	// 
	// any SPoly that is a SCAB_PARENT or SCAB_CHILD, regardless of its BoundaryOrientation, can be put into one SPolySupergroup.
	// These SPolys will be moved into the outputSPolySupergroups member or non-boundary supergroup, by calling relocateScabPolysToOutput() at
	// the end of the SPolyShellProducer::runSPolyShellConstruction function call.
	else if
	(
		isScabParent == true
		||
		isScabChild == true
	)
	{
		// sometimes, a scab isn't ALWAYS on a boundary. But if it is, exclude it.
		auto currentSPolyBoundaryOrientation = in_inputSPoly.getBoundaryIndicatorOrientation();
		if (currentSPolyBoundaryOrientation != BoundaryOrientation::NONE)
		{
			excludedOrientations.insert(currentSPolyBoundaryOrientation);
		}

		in_inputSPoly.setMode(SPolyMode::MALFORMED_MITIGATION);
		existingScabSupergroups.insertSPoly(in_inputSPoly);
	}
		
}

void SPolyShellProducer::handleBorderDebugOption(DebugOption in_debugOption)
{
	switch (in_debugOption)
	{
		case DebugOption::BORDERSPOLYPRODUCER_SHOW_EXTRACTABLE_SHELL_OUTPUT:
		{
			productionMassZone.extractableShellDebugLevel = PolyDebugLevel::DEBUG;
			break;
		}
		case DebugOption::BORDERSPOLYPRODUCER_PRINT_OUTPUT_SPOLYS:
		{
			printOutputSPolysDebugLevel = PolyDebugLevel::DEBUG;
			break;
		}
		case DebugOption::BORDERSPOLYPRODUCER_PRINT_MASSZONEBOX_TOUCHED_FACES:
		{
			productionMassZone.zoneBox.setTouchedGenerationDebugLevel(PolyDebugLevel::DEBUG);
			break;
		}
	}
}

void SPolyShellProducer::configurePolysWithoutNormalCalcs()
{
	for (int x = 0; x < numberOfInputSPolys; x++)
	{
		// PHASE 1: determine border lines, and planar normals; empty normals would already be calculated for these secondaries, if using this function as intended.
		inputSPolys[x].determinePrimalPoints();		// determine the primal points for the triangle
		inputSPolys[x].determineBorderLines();		// for each SPoly, determine its border lines.
	}
}

void SPolyShellProducer::relocateScabPolysToOutput()
{
	// put each scab poly back into it's appropriate orientation in outputSPolySuperGroups.
	for (auto& currentScabPoly : existingScabSupergroups.sPolyMap)
	{
		auto currentSPolyOrientation = currentScabPoly.second.getBoundaryIndicatorOrientation();

		// any SCAB_PARENT or SCAB_CHILD that has a BoundaryOrientation should go into the corresponding supergroup having 
		// the matching BoundaryOrientation.
		if (currentSPolyOrientation != BoundaryOrientation::NONE)
		{
			// insert into the corresponding boundary (POS_X, NEG_X, etc)
			outputSPolySuperGroups[currentSPolyOrientation].insertSPoly(currentScabPoly.second);
		}

		// ...however, a SCAB_CHILD SPoly that is NOT bound to any BoundaryOrientation will need to go back into it's own special supergroup, 
		// as this is a "Free" SCAB-type SPoly. SCAB_PARENTS should NEVER ever have a missing BoundaryOrientation, so we should only need to 
		// check for the SCAB_CHILD and BoundaryOrientation values.
		else if
		(
			(currentScabPoly.second.sPolyBoundaryIndicator.isScabChildIndicatorSet())
			&&
			currentSPolyOrientation == BoundaryOrientation::NONE
		)
		{
			existingFreeScabChildSupergroup.insertSPoly(currentScabPoly.second);	// insert into the "Free" SCAB_CHILD supergroup
		}

	}
}

bool SPolyShellProducer::checkShellCompleteness()
{
	bool isComplete = false;
	std::set<BoundaryOrientation> touchedFaces;

	// if there are any scabs that were produced in the shell, the shell's mesh will very likely be incomplete.
	// So just return.
	if (!existingScabSupergroups.isSupergroupEmpty())
	{
		return isComplete;
	}


	SPolyShellValidityChecker tempChecker;

	// Make sure to enter the data from the original SPoly objects. 
	for (auto& currentInput : inputSPolys)
	{
		if (currentInput.second.getBoundaryIndicatorOrientation() == BoundaryOrientation::NONE)
		{
			for (auto& currentInputTriangle : currentInput.second.triangles)
			{
				for (int x = 0; x < 3; x++)
				{
					HashedSTriangleLine convertedLine(currentInputTriangle.second.triangleLines[x].fetchPointAGlmVec3Version(),
													  currentInputTriangle.second.triangleLines[x].fetchPointBGlmVec3Version());

					tempChecker.insertLine(convertedLine);
				}
			}
		}
	}
	
	// If we haven't returned yet, run all SPoly instances that are in outputSPolySuperGroups through an instance of SPolyShellValidityChecker.
	for (auto& currentGroup : outputSPolySuperGroups)
	{
		touchedFaces.insert(currentGroup.first);

		for (auto& currentSPoly : currentGroup.second.sPolyMap)
		{
			for (auto& currentTriangle : currentSPoly.second.triangles)
			{
				for (int x = 0; x < 3; x++)
				{
					HashedSTriangleLine convertedLine(currentTriangle.second.triangleLines[x].fetchPointAGlmVec3Version(),
													  currentTriangle.second.triangleLines[x].fetchPointBGlmVec3Version()
													  );

					tempChecker.insertLine(convertedLine);
				}
			}
		}
	}

	if (tempChecker.fetchValidity())
	{
		isComplete = true;
	}
	else
	{
		// it wasn't complete; so print the stats. First, show the touched faces.

		/*
		std::cout << "Touched faces of bad mesh are: " << std::endl;
		for (auto& currentFace : touchedFaces)
		{
			OrganicGLWinUtils::printBoundaryOrientationEnum(currentFace);
			std::cout << std::endl;
		}


		std::cout << "Printing lines of ""bad"" mesh " << std::endl;
		tempChecker.printLineStats();
		*/
	}

	return isComplete;
}

MessageContainer SPolyShellProducer::runSPolyShellConstruction(MassZoneBoxType in_massZoneBoxType)
{
	/*
	// for when debugging only, print out the SPolys 
	std::cout << "(SPolyShellProducer): printing out SPolys involved in shell construction:" << std::endl;
	auto polysToAddBegin = inputSPolys.begin();
	auto polysToAddEnd = inputSPolys.end();
	for (; polysToAddBegin != polysToAddEnd; polysToAddBegin++)
	{
		std::cout << "ID: " << polysToAddBegin->first << std::endl;
		auto trianglesBegin = polysToAddBegin->second.triangles.begin();
		auto trianglesEnd = polysToAddBegin->second.triangles.end();
		for (; trianglesBegin != trianglesEnd; trianglesBegin++)
		{
			std::cout << "Triangle " << trianglesBegin->first << std::endl;
			trianglesBegin->second.printPoints();
		}
	}
	*/

	MessageContainer productionErrors;	
	productionMassZone.setMassZoneRecorderRef(&producerExceptionRecorder);	// the recorder ref needs to be set before the boundaries are created.
	productionMassZone.createMassZoneBoxBoundary(in_massZoneBoxType);		// first, create the boundaries for the MassZone


	// ||||||||||||||||||| Do this if there are NOT any input SPolys;
	// || i.e., outputSPolySuperGroups will contain one SPolySupergroup for each of the 6 boundaries, and each SPolySupergroup will have
	// || exactly 1 SPoly, which will be equal to the value of MassZoneBoxBoundary::borderSPoly. The borderSPoly should have already been setup,
	// || during the call to createMassZoneBoxBoundary above.


	// ||||||||||||||||||| Do this if there are input SPolys.
	if (!inputSPolys.empty())
	{
		// then, add each input SPoly to the production mass zone.
		for (int x = 0; x < numberOfInputSPolys; x++)
		{
			productionMassZone.insertSPolyMassSubZone(x, inputSPolys[x]);
		}

		// NEW: need to give the productionMassZone a reference to the map that stores any SPolys that have boundary values,
		// so that it knows which faces to ignore production on and insert the already-generated boundary SPolys into.
		// Also need to give the productionMassZone a list of excluded boundaries that it can ignore producing SPolys on.
		productionMassZone.setExcludedBoundaries(excludedOrientations);
		productionMassZone.setLegitimateBoundaryGroupsRef(&existingLegitimateBoundarySupergroups);

		// enable "shell extraction mode" in the MassZoneShell before creating the shell; should be done after boundary creation, but before shell creation.
		productionMassZone.enableContestedCategorizedLineAnalysis();

		// now that the input SPolys have been added to the production zone, create the mass zone shell;
		// be sure to pass any tertiary extraction options.
		productionErrors = productionMassZone.createMassZoneShell(MassZoneType::COHERENT_ZONE);

		// produce the extractable shell SPolys; this would also produce any SPolys that are produced as a result of a contestation where it is determined that 
		// a SPoly needs to take up an entire boundary's face.
		productionMassZone.produceExtractableMassZoneShellSPolys(&outputSPolySuperGroups);


		// have the MassZoneBox in the MassZone, produce a list of faces that were "touched";
		// a "touched" face is any face whose enum is found in the face list that would be produced by the value of an ECBPPOrientationResults.
		// The secondary SPoly (secondary meaning the SPolys that are produced by the call to MassZoneBoxBoundary::generateSPolysFromPolySet()) will have
		// the ECBPolyPoints, each of which will be analyzed to determine their ECBPPOrientationResult, which can then be used to produce a face list, 
		// by using the call to IndependentUtils::getFaceList(ECBPPOrientationResults in_beginOrientation, BorderDataMap* in_borderDataMapRef).
		// 
		// The enum values for the faces are: (see the enum for ECBPPOrientations)
		//	WESTFACE,
		//  NORTHFACE,
		//  EASTFACE,
		//	SOUTHFACE,
		//	TOPFACE,
		//	BOTTOMFACE
		//
		// Each enum value corresponds to a BoundaryOrientation enum value:
		//	WESTFACE	->	NEG_X
		//  NORTHFACE	->	NEG_Z
		//  EASTFACE	->	POS_X
		//	SOUTHFACE	->	POS_Z
		//	TOPFACE		->	POS_Y
		//	BOTTOMFACE	->	NEG_Y	
		//
		// The values of the "touched" face list are put into a set that is built by the MassZoneBox. The MassZoneBox will then use the values in this set
		// to cycle through its boxBoundaries map, and check each MassZoneBoxBoundary to see whether or not 
		// that MassZoneBoxBoundary has its boundarySPolySet::wasLineProducedByReactor set to TRUE. If it is set to FALSE, 
		// we will return the boundary SPoly that takes up that face itself, as a new SPoly.
		auto fetchedList = productionMassZone.getTouchedBoxFacesList(in_massZoneBoxType);
		productionMassZone.runFirstTertiaryProductionPassInZoneBox(fetchedList, &outputSPolySuperGroups);


		if (printOutputSPolysDebugLevel == PolyDebugLevel::DEBUG)
		{
			std::cout << "(SPolyShellProducer): printing out produced SPolys in each mass zone box boundary..." << std::endl;
			auto outputsBegin = outputSPolySuperGroups.begin();
			auto outputsEnd = outputSPolySuperGroups.end();
			for (; outputsBegin != outputsEnd; outputsBegin++)
			{
				OrganicGLWinUtils::printBoundaryOrientationEnum(outputsBegin->first);
				std::cout << std::endl;
				outputsBegin->second.printSPolys();
			}
		}

		// after everything above is done, insert any SCAB_PARENT or SCAB_CHILD polys back into their appropriate zoneBox.boxBoundary, or other SPolySupergroup.
		// 2022_4_002 (4)
		relocateScabPolysToOutput();
	}

	// ||||||||||||||||||| Do this if there are NO input SPolys.
	else
	{
		std::cout << "(SPolyShellProducer): No input SPolys detected to form a shell; will extract the entire boundary shell. " << std::endl;
		productionMassZone.extractAllBoundarySPolys(&outputSPolySuperGroups);
	}

	return productionErrors;
}

int SPolyShellProducer::getNumberOfProducedSPolysInMassZoneBoundarySPoly(BoundaryOrientation in_targetBoundary)
{
	int returnValue = 0;
	auto orientationFinder = productionMassZone.zoneBox.boxBoundaries.find(in_targetBoundary);
	if (orientationFinder != productionMassZone.zoneBox.boxBoundaries.end())	// the target orientation was found.
	{
		returnValue = productionMassZone.zoneBox.boxBoundaries[in_targetBoundary].getProducedSPolyCount();
	}
	return returnValue;
}

void SPolyShellProducer::checkForPosZSquareBoundaryInOutput()
{
	auto orientationFinder = outputSPolySuperGroups.find(BoundaryOrientation::POS_Z);
	if (orientationFinder != outputSPolySuperGroups.end())	// the target orientation was found.
	{
		outputSPolySuperGroups[BoundaryOrientation::POS_Z].checkForAnyPosZ();
	}
	else
	{

	}
}

ExceptionRecorder SPolyShellProducer::getExceptions()
{
	return producerExceptionRecorder;
}

std::vector<SPoly> SPolyShellProducer::fetchAllSPolys()
{
	std::vector<SPoly> returnVector;

	// push back all SPolys that were inserted to produce the bordering SPolys (from the call to addInputSPoly)
	for (auto& currentInputSPoly : inputSPolys)
	{
		returnVector.push_back(currentInputSPoly.second);
	}

	// push back all SPolys from each oriented SPoly super group; this includes:
	//	-any normally produced SPolys that have don't have a BoundaryOrientation::NONE in their indicator, and were produced as a result of the call to the function MassZone::createMassZoneShell (see Step 3),
	//	 when this SPolyShellProducer did its run.
	//
	//	-any SCAB_PARENT -OR- SCAB_CHILD SPolys produced as a result of an SPolyResolution generating a "fix" during the call to MassZone::createMassZoneShell
	//
	//	-from a previous run of SPolyShellProducer, any SCAB_PARENT -OR- SCAB_CHILD that has doesn't have a BoundaryOrientation::NONE in its indicator;
	//	 these would be all of the SCAB_PARENT SPolys contained in existingScabSupergroups that would be moved to their appropriate orientation during the
	//	 call to relocateScabPolysToOutput()
	for (auto& currentOrientedSupergroup : outputSPolySuperGroups)
	{
		for (auto& currentOrientedSPoly : currentOrientedSupergroup.second.sPolyMap)
		{
			returnVector.push_back(currentOrientedSPoly.second);
		}
	}

	// push back any free scab children; this would be all of the SCAB_CHILD SPolys that were added during the call to addInputSPoly that DID
	// have BoundaryOrientation::NONE value
	for (auto& currentFreeScabChild : existingFreeScabChildSupergroup.sPolyMap)
	{
		returnVector.push_back(currentFreeScabChild.second);
	}

	return returnVector;
}

void SPolyShellProducer::printSupergroupBoundaryIndicators()
{
	auto generatedSPolysBegin = outputSPolySuperGroups.begin();
	auto generatedSPolysEnd = outputSPolySuperGroups.end();
	for (; generatedSPolysBegin != generatedSPolysEnd; generatedSPolysBegin++)
	{
		auto currentGroupSPolysBegin = generatedSPolysBegin->second.sPolyMap.begin();
		auto currentValue = currentGroupSPolysBegin->second.sPolyBoundaryIndicator.getBoundaryIndicatorValue();

		OrganicGLWinUtils::printBoundaryOrientationEnum(generatedSPolysBegin->first);
		switch (currentValue)
		{
			case BoundaryOrientation::NONE: { std::cout << "-> NONE " << std::endl; break; }
			case BoundaryOrientation::POS_X: { std::cout << "-> POS_X " << std::endl; break; }
			case BoundaryOrientation::NEG_X: { std::cout << "-> NEG_X " << std::endl; break; }
			case BoundaryOrientation::POS_Y: { std::cout << "-> POS_Y " << std::endl; break; }
			case BoundaryOrientation::NEG_Y: { std::cout << "-> NEG_Y " << std::endl; break; }
			case BoundaryOrientation::POS_Z: { std::cout << "-> POS_Z " << std::endl; break; }
			case BoundaryOrientation::NEG_Z: { std::cout << "-> NEG_Z " << std::endl; break; }
		}
	}
}

void SPolyShellProducer::printAllProducedSPolys()
{
	std::cout << "(SPolyShellProducer): Printing all produced SPolys. " << std::endl;

	// cycle through the input SPolys.
	std::cout << "|| Input SPolys: " << std::endl;
	int currentInputsPolyID = 0;
	for (auto& currentInputSPoly : inputSPolys)
	{
		std::cout << "Input SPoly at index: " << currentInputsPolyID++ << std::endl;
		currentInputSPoly.second.printTriangleMetadata();
		
	}

	// cycle through the normal output SPolys.
	for (auto& currentOutputSG : outputSPolySuperGroups)
	{
		std::cout << "|| Output SPolys for orientation ";
		OrganicGLWinUtils::printBoundaryOrientationEnum(currentOutputSG.first);
		std::cout << ": " << std::endl;
		currentOutputSG.second.printSPolys();
	}

	// cycle through SCAB_CHILD free SPolys
	std::cout << "|| Free SCAB_CHILD SPolys: " << std::endl;
	existingFreeScabChildSupergroup.printSPolys();
}

std::set<BoundaryOrientation> SPolyShellProducer::fetchOutputBoundaries()
{
	std::set<BoundaryOrientation> boundaries;
	auto outputSPolysBegin = outputSPolySuperGroups.begin();
	auto outputSPolysEnd = outputSPolySuperGroups.end();
	for (; outputSPolysBegin != outputSPolysEnd; outputSPolysBegin++)
	{
		boundaries.insert(outputSPolysBegin->first);
	}
	return boundaries;
}

Message SPolyShellProducer::convertBoundarySPolySupergroupToMessage(BoundaryOrientation in_targetBoundary)
{
	Message meltedGroupData;

	// the returned message type is dependent on whether or not there were input SPolys.
	//
	//	-if the resulting SPolyGroups in the shell were produced was from at least 1 input SPoly, 
	//   then the Message should be of type MessageType::SPOLYSHELLPRODUCER_BOUNDARY_PRODUCED_FROM_INPUT.
	//  
	//	-otherwise, if the resulting produced shell was from no input SPolys, then all 6 sides were produced.
	//	 Thus, we will signify in the Message that they were produced from no Input (MessageType::SPOLYSHELLPRODUCER_BOUNDARY_PRODUCED_FROM_NO_INPUT)
	SPolySupergroup* targetGroupRef = &outputSPolySuperGroups[in_targetBoundary];
	meltedGroupData = SPolyUtils::meltSPolySupergroupIntoMessage(targetGroupRef);
	if (!inputSPolys.empty())
	{
		meltedGroupData.messageType = MessageType::SPOLYSHELLPRODUCER_BOUNDARY_PRODUCED_FROM_INPUT;
	}
	else
	{
		meltedGroupData.messageType = MessageType::SPOLYSHELLPRODUCER_BOUNDARY_PRODUCED_FROM_NO_INPUT;
	}
	
	return meltedGroupData;
}