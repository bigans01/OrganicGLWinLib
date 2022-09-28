#include "stdafx.h"
#include "SPolyShellProducer.h"

void SPolyShellProducer::addInputSPoly(SPoly in_inputSPoly)
{
	inputSPolys[numberOfInputSPolys++] = in_inputSPoly; // add the poly, and increment it
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
	auto addedSPolysBegin = inputSPolys.begin();
	auto addedSPolysEnd = inputSPolys.end();
	for (; addedSPolysBegin != addedSPolysEnd; addedSPolysBegin++)
	{
		returnVector.push_back(addedSPolysBegin->second);
	}

	// push back all SPolys from each generated SPoly super group.
	auto generatedSPolysBegin = outputSPolySuperGroups.begin();
	auto generatedSPolysEnd = outputSPolySuperGroups.end();
	for (; generatedSPolysBegin != generatedSPolysEnd; generatedSPolysBegin++)
	{
		auto currentGroupSPolysBegin = generatedSPolysBegin->second.sPolyMap.begin();
		auto currentGroupSPolysEnd = generatedSPolysBegin->second.sPolyMap.end();
		for (; currentGroupSPolysBegin != currentGroupSPolysEnd; currentGroupSPolysBegin++)
		{
			returnVector.push_back(currentGroupSPolysBegin->second);
		}
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