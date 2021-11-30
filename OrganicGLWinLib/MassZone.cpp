#include "stdafx.h"
#include "MassZone.h"

void MassZone::setMassZoneLogLevel(PolyDebugLevel in_polyDebugLevel)
{
	massZoneLogLevel = in_polyDebugLevel;
}

void MassZone::insertSPolyMassSubZone(int in_sPolyID, SPoly in_sPolyCopy)
{
	// the SPoly-based sub zone should be from an SPoly that's fully fleshed out;
	// i.e, the SPoly has its primal points, border lines, and appropriate empty normal in place.

	int currentSubZoneIndex = int(subZoneMap.size());
	subZoneMap[currentSubZoneIndex].sPolyCopy = in_sPolyCopy;
	insertMeshMatterMeta(in_sPolyID, &subZoneMap[currentSubZoneIndex].sPolyCopy, subZoneMap[currentSubZoneIndex].sPolyCopy.massManipulationSetting);
	insertSPolyToSubZoneMapEntry(in_sPolyID, currentSubZoneIndex);
	insertSubZoneToSPolyMapEntry(in_sPolyID, currentSubZoneIndex);
}

void MassZone::insertSPolyToSubZoneMapEntry(int in_sPolyID, int in_subZoneID)
{
	sPolyToSubZoneMap[in_sPolyID] = in_subZoneID;
}

void MassZone::insertSubZoneToSPolyMapEntry(int in_sPolyID, int in_subZoneID)
{
	subZoneToSPolyMap[in_subZoneID] = in_sPolyID;
}

void MassZone::insertDOForClippableSPoly(int in_sPolyID, DebugOption in_debugOption)
{
	clipper.setClippableSPolyDO(in_sPolyID, in_debugOption);
}

void MassZone::insertMeshMatterMeta(int in_sPolyID, SPoly* in_massSPolyRef, MassManipulationMode in_originMassManipulationMode)
{
	MeshMatterMeta newMeta(in_sPolyID, in_massSPolyRef, in_originMassManipulationMode);
	meshMatterMetaMap[in_sPolyID] = newMeta;
}

void MassZone::wrapperInsertDOForClippableSPoly(int in_sPolyID, DebugOption in_debugOption)
{
	clipper.setClippableSPolyDO(in_sPolyID, in_debugOption);
}

void MassZone::removeMeshMatterMeta(int in_sPolyID)
{
	meshMatterMetaMap.erase(in_sPolyID);
}

void MassZone::printMeshMatterMeta()
{
	auto meshMatterMetaBegin = meshMatterMetaMap.begin();
	auto meshMatterMetaEnd = meshMatterMetaMap.end();
	for (; meshMatterMetaBegin != meshMatterMetaEnd; meshMatterMetaBegin++)
	{
		std::cout << "SPolyID: " << meshMatterMetaBegin->first << std::endl;
	}
}

void MassZone::printSubZoneSPolys()
{
	std::cout << "Sub zone map size: " << subZoneMap.size() << std::endl;

	/*
	auto subZoneMapBegin = sPolyToSubZoneMap.begin();
	auto subZoneMapEnd = sPolyToSubZoneMap.end();
	for (; subZoneMapBegin != subZoneMapEnd; subZoneMapBegin++)
	{
		std::cout << "SPoly ID " << subZoneMapBegin->first << ": " << std::endl;
		subZoneMap[subZoneMapBegin->second].sPolyCopy.printPoints();
	}
	*/

	auto subZoneMap2Begin = subZoneMap.begin();
	auto subZoneMap2End = subZoneMap.end();
	for (; subZoneMap2Begin != subZoneMap2End; subZoneMap2Begin++)
	{
		std::cout << "! SPoly ID " << subZoneMap2Begin->first << ": " << std::endl;
		std::cout << "! Number of triangles: " << subZoneMap2Begin->second.sPolyCopy.triangles.size() << std::endl;

		SPoly currentCopy = subZoneMap2Begin->second.sPolyCopy;
		currentCopy.determineBorderLines();
		currentCopy.printPoints();

		//subZoneMap2Begin->second.sPolyCopy.printPoints();
	}
}

void MassZone::printBoundaryErrors(MessageContainer* in_messageContainerRef)
{
	auto messageContainerBegin = in_messageContainerRef->begin();
	auto messageContainerEnd = in_messageContainerRef->end();
	for (; messageContainerBegin != messageContainerEnd; messageContainerBegin++)
	{
		Message* currentMessageRef = &(*messageContainerBegin);
		currentMessageRef->open();

		auto currentIntIter = currentMessageRef->intVectorIter;

		int currentSequenceID = currentMessageRef->readInt();

		std::cout << "(MassZone): printing out lines for sequence with ID " << currentSequenceID << std::endl;

		int currentNumberOfCategorizedLines = currentMessageRef->readInt();
		for (int x = 0; x < currentNumberOfCategorizedLines; x++)
		{
			int currentLineID = currentMessageRef->readInt();
			ECBPolyPoint currentLinePointA = currentMessageRef->readPoint();
			ECBPolyPoint currentLinePointB = currentMessageRef->readPoint();
			ECBPolyPoint currentLineEmptyNormal = currentMessageRef->readPoint();

			std::cout << "Line " << currentLineID << "-> pointA: (" << currentLinePointA.x << ", " << currentLinePointA.y << ", " << currentLinePointA.z
				<< ") | pointB: (" << currentLinePointB.x << ", " << currentLinePointB.y << ", " << currentLinePointB.z
				<< ") | emptyNormal: (" << currentLineEmptyNormal.x << ", " << currentLineEmptyNormal.y << ", " << currentLineEmptyNormal.z << ") "
				<< std::endl;
																
		}

		//int faceID = currentMessageRef->readInt();
		std::cout << "(MassZone): done printing out lines for face ID: ";
		auto currentFaceEnum = MassUtils::getMassZoneOrientationEnumValue(currentMessageRef->readInt());
		MassUtils::printEnumValue(currentFaceEnum);
		std::cout << std::endl;
	}
}

void MassZone::createMassZoneBoxBoundary(MassZoneBoxType in_massZoneBoxType)
{
	// the 8 points representing the corners of a MassZoneBox.

	// remember, lower_NW has no offsets applied; upper_SE has all 3 offsets applied.
	glm::vec3 lower_NW;
	glm::vec3 lower_SW;
	glm::vec3 lower_SE;
	glm::vec3 lower_NE;
	glm::vec3 upper_NW;
	glm::vec3 upper_SW;
	glm::vec3 upper_SE;
	glm::vec3 upper_NE;

	// determine the distance between points
	float distanceBetweenPoints = 0.0f;
	if (in_massZoneBoxType == MassZoneBoxType::BLOCK)
	{
		distanceBetweenPoints = 1.0f;
	}
	else if (in_massZoneBoxType == MassZoneBoxType::ENCLAVE)
	{
		distanceBetweenPoints = 4.0f;
	}
	else if (in_massZoneBoxType == MassZoneBoxType::COLLECTION)
	{
		distanceBetweenPoints = 32.0f;
	}

	// set up the values for the points.

	// lower_NW

	// lower_SW
	lower_SW.z += distanceBetweenPoints;

	// lower_SE
	lower_SE.x += distanceBetweenPoints;
	lower_SE.z += distanceBetweenPoints;

	// lower_NE
	lower_NE.x += distanceBetweenPoints;

	// upper_NW
	upper_NW.y += distanceBetweenPoints;

	// upper_SW
	upper_SW.y += distanceBetweenPoints;
	upper_SW.z += distanceBetweenPoints;

	// upper_SE
	upper_SE.x += distanceBetweenPoints;
	upper_SE.y += distanceBetweenPoints;
	upper_SE.z += distanceBetweenPoints;

	// upper_NE
	upper_NE.x += distanceBetweenPoints;
	upper_NE.y += distanceBetweenPoints;

	// create the boundaries, set their empty normal values (required later if there is a needed sub-zone field)

	// NEG_Z boundary (north) creation/insertion
	
	glm::vec3 northEmptyNormal;
	northEmptyNormal.z = -1.0f;
	glm::vec3 northBoundaryCenterPoint(0.5f, 0.5f, 0.0f);	// neg z boundary is 0
	northBoundaryCenterPoint *= distanceBetweenPoints;
	MassZoneBoxBoundary northBoundary(MassZoneBoxBoundaryOrientation::NEG_Z, lower_NW, upper_NW, upper_NE, lower_NE, northEmptyNormal, northBoundaryCenterPoint);
	zoneBox.insertNewBoundary(MassZoneBoxBoundaryOrientation::NEG_Z, northBoundary, std::move(boundaryDebugOptions[MassZoneBoxBoundaryOrientation::NEG_Z]));

	// POS_X boundary (east)
	glm::vec3 eastEmptyNormal;
	eastEmptyNormal.x = 1.0f;
	glm::vec3 eastBoundaryCenterPoint(1.0f, 0.5f, 0.5f);	// pos x boundary is 1
	eastBoundaryCenterPoint *= distanceBetweenPoints;
	MassZoneBoxBoundary eastBoundary(MassZoneBoxBoundaryOrientation::POS_X, lower_NE, upper_NE, upper_SE, lower_SE, eastEmptyNormal, eastBoundaryCenterPoint);
	zoneBox.insertNewBoundary(MassZoneBoxBoundaryOrientation::POS_X, eastBoundary, std::move(boundaryDebugOptions[MassZoneBoxBoundaryOrientation::POS_X]));

	// POS_Z boundary (south) creation/insertion
	glm::vec3 southEmptyNormal;
	southEmptyNormal.z = 1.0f;
	glm::vec3 southBoundaryCenterPoint(0.5f, 0.5f, 1.0f);	// pos z boundary is 1
	southBoundaryCenterPoint *= distanceBetweenPoints;
	MassZoneBoxBoundary southBoundary(MassZoneBoxBoundaryOrientation::POS_Z, lower_SE, upper_SE, upper_SW, lower_SW, southEmptyNormal, southBoundaryCenterPoint);
	zoneBox.insertNewBoundary(MassZoneBoxBoundaryOrientation::POS_Z, southBoundary, std::move(boundaryDebugOptions[MassZoneBoxBoundaryOrientation::POS_Z]));

	// NEG_X boundary (west) creation/insertion
	glm::vec3 westEmptyNormal;
	westEmptyNormal.x = -1.0f;
	glm::vec3 westBoundaryCenterPoint(0.0f, 0.5f, 0.5f);	// neg x boundary is 0
	westBoundaryCenterPoint *= distanceBetweenPoints;
	MassZoneBoxBoundary westBoundary(MassZoneBoxBoundaryOrientation::NEG_X, lower_SW, upper_SW, upper_NW, lower_NW, westEmptyNormal, westBoundaryCenterPoint);
	zoneBox.insertNewBoundary(MassZoneBoxBoundaryOrientation::NEG_X, westBoundary, std::move(boundaryDebugOptions[MassZoneBoxBoundaryOrientation::NEG_X]));

	// POS_Y boundary (above) creation/insertion
	glm::vec3 aboveEmptyNormal;
	aboveEmptyNormal.y = 1.0f;
	glm::vec3 aboveBoundaryCenterPoint(0.5f, 1.0f, 0.5f);	// pos y boundary is 1
	aboveBoundaryCenterPoint *= distanceBetweenPoints;
	MassZoneBoxBoundary aboveBoundary(MassZoneBoxBoundaryOrientation::POS_Y, upper_NW, upper_NE, upper_SE, upper_SW, aboveEmptyNormal, aboveBoundaryCenterPoint);
	zoneBox.insertNewBoundary(MassZoneBoxBoundaryOrientation::POS_Y, aboveBoundary, std::move(boundaryDebugOptions[MassZoneBoxBoundaryOrientation::POS_Y]));

	// NEG_Y boundary (below) creation/insertion
	glm::vec3 belowEmptyNormal;
	belowEmptyNormal.y = -1.0f;
	glm::vec3 belowBoundaryCenterPoint(0.5f, 0.0f, 0.5f);	// neg y boundary is 0
	belowBoundaryCenterPoint *= distanceBetweenPoints;
	MassZoneBoxBoundary belowBoundary(MassZoneBoxBoundaryOrientation::NEG_Y, lower_NW, lower_NE, lower_SE, lower_SW, belowEmptyNormal, belowBoundaryCenterPoint);
	zoneBox.insertNewBoundary(MassZoneBoxBoundaryOrientation::NEG_Y, belowBoundary, std::move(boundaryDebugOptions[MassZoneBoxBoundaryOrientation::NEG_Y]));

	//zoneBox.printBoundaries();

	//std::cout << "############### CHECKING #######################" << std::endl;
	//zoneBox.printBoundaryLineCounts();
	//std::cout << "############### DOUBLE CHECKING #######################" << std::endl;
	//zoneBox.printBoundaryLineCounts();
	//int checkVal = 3;
	//std::cin >> checkVal;	
}

void MassZone::insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation in_massZoneBoxBoundaryOrientation, SPolyDO in_sPolyDO)
{
	boundaryDebugOptions[in_massZoneBoxBoundaryOrientation].insert(in_sPolyDO);
}

void MassZone::runClipper()
{
	clipper.run();
}

std::set<MassZoneBoxBoundaryOrientation> MassZone::getTouchedBoxFacesList(MassZoneBoxType in_massZoneBoxType)
{
	std::set<MassZoneBoxBoundaryOrientation> returnSet = zoneBox.generateTouchedBoxFacesList(in_massZoneBoxType);
	return returnSet;
}

void MassZone::runFirstTertiaryProductionPassInZoneBox(std::set<MassZoneBoxBoundaryOrientation> in_orientationSet, 
													   std::map<MassZoneBoxBoundaryOrientation, SPolySupergroup>* in_outputSuperGroupsMapRef)
{
	zoneBox.runFirstTertiaryProductionPass(in_orientationSet, in_outputSuperGroupsMapRef);
}

void MassZone::produceExtractableMassZoneShellSPolys(std::map<MassZoneBoxBoundaryOrientation, SPolySupergroup>* in_outputSuperGroupsMapRef)
{
	PolyLogger tempExtractableLogger;
	tempExtractableLogger.setDebugLevel(extractableShellDebugLevel);
	auto zoneBoxBoundariesBegin = zoneBox.boxBoundaries.begin();
	auto zoneBoxBoundariesEnd = zoneBox.boxBoundaries.end();
	for (; zoneBoxBoundariesBegin != zoneBoxBoundariesEnd; zoneBoxBoundariesBegin++)
	{
		//OrganicGLWinUtils::printMassZoneBoxBoundaryOrientationEnum(zoneBoxBoundariesBegin->first);
		if
		(
			(zoneBoxBoundariesBegin->second.boundaryPolySet.boundarySPolySG.sPolyMap.size() != 0)
		)
		{
			//std::cout << "(MassZone) -> uncontested, but contains produced SPolys. " << std::endl;
			if (tempExtractableLogger.isLoggingSet())
			{
				OrganicGLWinUtils::printMassZoneBoxBoundaryOrientationEnum(zoneBoxBoundariesBegin->first);
				std::cout << ": ";
			}
			tempExtractableLogger.log("(MassZone) -> uncontested, but contains produced SPolys. ", "\n");
			(*in_outputSuperGroupsMapRef)[zoneBoxBoundariesBegin->first] = zoneBoxBoundariesBegin->second.boundaryPolySet.boundarySPolySG;
		}

		else if
		(
			(zoneBoxBoundariesBegin->second.boundaryPolySet.didCategorizedLineWinContest == true)
		)
		{
			// produce the SPoly that would be produced by the line winning the contest, put it into the ref'd map.
			SPolySupergroup tempGroup;
			tempGroup.insertSPoly(*zoneBoxBoundariesBegin->second.boundaryPolySet.boundarySPolyRef);
			(*in_outputSuperGroupsMapRef)[zoneBoxBoundariesBegin->first] = tempGroup;
			//std::cout << "(MassZone) -> categorized line won contest " << std::endl;
			if (tempExtractableLogger.isLoggingSet())
			{
				OrganicGLWinUtils::printMassZoneBoxBoundaryOrientationEnum(zoneBoxBoundariesBegin->first);
				std::cout << ": ";
			}
			tempExtractableLogger.log("(MassZone) -> categorized line won contest ", "\n");
		}
		else
		{
			if (tempExtractableLogger.isLoggingSet())
			{
				OrganicGLWinUtils::printMassZoneBoxBoundaryOrientationEnum(zoneBoxBoundariesBegin->first);
				std::cout << ": ";
			}
			//std::cout << "(MassZone) -> N/A. " << std::endl;
			tempExtractableLogger.log("(MassZone) -> N/A. ", "\n");
		}
	}
}

MessageContainer MassZone::createMassZoneShell(MassZoneType in_massZoneType)
{
	// need to apply tertiary extraction options, to all boundaries that need them set.
	//bool wasShellCreationSuccessful = true;
	MessageContainer shellCreationErrors;

	// build the log output prefix 
	std::string prefixString;
	if (in_massZoneType == MassZoneType::OLD_ZONE)
	{
		prefixString = "(MassZone) (OLD_ZONE) ";
	}
	else if (in_massZoneType == MassZoneType::NEW_ZONE)
	{
		prefixString = "(MassZone) (NEW_ZONE) ";
	}
	else if (in_massZoneType == MassZoneType::COHERENT_ZONE)
	{
		prefixString = "(MassZone) (COHERENT_ZONE) ";
	}

	// Step 1: compare all subZones that are from an SPoly, to all the MassZoneBoxBoundaries, to determine the CategorizedLines that will
	//         be generated in each MassZoneBoxBoundary's SPoly. We will also load the SPoly from each zone into the clipper here.

	/*
	auto subZoneMapBeginPre = subZoneMap.begin();
	auto subZoneMapEndPre = subZoneMap.end();
	for (; subZoneMapBeginPre != subZoneMapEndPre; subZoneMapBeginPre++)
	{
		//std::cout << "!! Number of triangles in this subzone: " << subZoneMapBeginPre->second.sPolyCopy.triangles.size() << std::endl;
		//std::cout << "!! Number of border lines in this subzone: " << subZoneMapBeginPre->second.sPolyCopy.borderLines.size() << std::endl;
	}
	*/

	
	auto subZoneMapBegin = subZoneMap.begin();
	auto subZoneMapEnd = subZoneMap.end();
	for (; subZoneMapBegin != subZoneMapEnd; subZoneMapBegin++)
	{
		// each SPoly-based subZone must be run against all 6 boundaries in the zoneBox.
		//std::cout << ">> Comparing SPoly based subZone,  with the SPoly having ID: " << subZoneToSPolyMap[subZoneMapBegin->first] << std::endl;
		zoneBox.runSPolyBasedSubZoneAgainstBoundaries(&subZoneMapBegin->second);
		clipper.insertSPolyRefIntoClippingShell(&subZoneMapBegin->second.sPolyCopy);

		//std::cout << "++++++++++++++++ Printing out SPoly lines, of the SPoly we are about to insert: " << std::endl;
		//subZoneMapBegin->second.sPolyCopy.printBorderLines();
		//std::cout << "!!! Inserted SPoly ref into clipper. " << std::endl;
	}

	// Step 2: print the lines in each boundary (for testing only)
	PolyLogger tempCategorizedLineLogger;
	tempCategorizedLineLogger.setDebugLevel(printBoundaryLinesLogLevel);
	if (tempCategorizedLineLogger.isLoggingSet() == true)
	{
		tempCategorizedLineLogger.log(prefixString, ">>>> starting printing of categorized lines in zoneBox boundaries...", "\n");
		zoneBox.printCategorizedLinesInBoundaries();
		tempCategorizedLineLogger.log(prefixString, ">>>> finished printing of categorized lines in zoneBox boundaries...", "\n");
		tempCategorizedLineLogger.log(prefixString, "Enter number to continue...", "\n");
		//std::cout << "############### Done printing categorized lines in boundaries...." << std::endl;
		//int someVal = 3;
		//std::cin >> someVal;
		tempCategorizedLineLogger.waitForDebugInput();
	}

	// zoneBox.printCategorizedLinesInBoundaries();

	// Step 3: When all comparisons have been made, go through each MassZoneBoxBoundary's MassZoneBosBoundarySPolySet, run the 
	// SPolyFracturer for each MassZoneBoxBoundarySPolySet, to generate any SPolys.
	
	PolyLogger tempSPolyBoundaryProductionLogger;
	tempSPolyBoundaryProductionLogger.setDebugLevel(boundarySPolyConstructionLogLevel);
	auto boxBoundariesBegin = zoneBox.boxBoundaries.begin();
	auto boxBoundariesEnd = zoneBox.boxBoundaries.end();
	for (; boxBoundariesBegin != boxBoundariesEnd; boxBoundariesBegin++)
	{
		if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Z)
		{
			//std::cout << "|||||||||||||||||||||||||||||||||||||||||||| Attempting boundary artificial SPoly construction for NEG_Z..." << std::endl;
			tempSPolyBoundaryProductionLogger.log(prefixString, "(MassZone): |||| Attempting boundary artificial SPoly construction for NEG_Z...", "\n");
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_X)
		{
			//std::cout << "|||||||||||||||||||||||||||||||||||||||||||| Attempting boundary artificial SPoly construction for POS_X..." << std::endl;
			tempSPolyBoundaryProductionLogger.log(prefixString, "(MassZone): |||| Attempting boundary artificial SPoly construction for POS_X...", "\n");
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Z)
		{
			//std::cout << "|||||||||||||||||||||||||||||||||||||||||||| Attempting boundary artificial SPoly construction for POS_Z..." << std::endl;
			tempSPolyBoundaryProductionLogger.log(prefixString, "(MassZone): |||| Attempting boundary artificial SPoly construction for POS_Z...", "\n");
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_X)
		{
			//std::cout << "|||||||||||||||||||||||||||||||||||||||||||| Attempting boundary artificial SPoly construction for NEG_X..." << std::endl;
			tempSPolyBoundaryProductionLogger.log(prefixString, "(MassZone): |||| Attempting boundary artificial SPoly construction for NEG_X...", "\n");
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::POS_Y)
		{
			//std::cout << "|||||||||||||||||||||||||||||||||||||||||||| Attempting boundary artificial SPoly construction for POS_Y..." << std::endl;
			tempSPolyBoundaryProductionLogger.log(prefixString, "(MassZone): |||| Attempting boundary artificial SPoly construction for POS_Y...", "\n");
		}
		else if (boxBoundariesBegin->first == MassZoneBoxBoundaryOrientation::NEG_Y)
		{
			//std::cout << "|||||||||||||||||||||||||||||||||||||||||||| Attempting boundary artificial SPoly construction for NEG_Y..." << std::endl;
			tempSPolyBoundaryProductionLogger.log(prefixString, "(MassZone): |||| Attempting boundary artificial SPoly construction for NEG_Y...", "\n");
		}

		MessageContainer currentBoundaryProducedMessages = boxBoundariesBegin->second.generateSPolysFromPolySet();
		currentBoundaryProducedMessages.appendIntToAll(MassUtils::getMassZoneOrientationIntValue(boxBoundariesBegin->first));
		shellCreationErrors += currentBoundaryProducedMessages;
		

		//std::cout << "Finished producing boundary SPoly...continue? " << std::endl;
		//int prodSucceeded = 3;
		//std::cin >> prodSucceeded;
		tempSPolyBoundaryProductionLogger.log(prefixString, "Finished producing boundary SPoly...enter number to continue. ", "\n");
		tempSPolyBoundaryProductionLogger.waitForDebugInput();
	}

	if (shellCreationErrors.empty() == false)
	{
		std::cout << "(MassZone): found shell creation as unsuccessful, preparing to handle..." << std::endl;

		printBoundaryErrors(&shellCreationErrors);

		std::cout << "(MassZone): printing out subzone SPoly IDs and points: " << std::endl;
		printSubZoneSPolys();
		int handle = 3;
		std::cin >> handle;
	}

	// Step 4: put all super groups which actually contain data, into their own sub zone.
	PolyLogger tempBoundarySuperGroupLogger;
	tempBoundarySuperGroupLogger.setDebugLevel(pointClippingLogLevel);
	auto boundarySPolyGroupsBegin = zoneBox.boxBoundaries.begin();
	auto boundarySPolyGroupsEnd = zoneBox.boxBoundaries.end();
	for (; boundarySPolyGroupsBegin != boundarySPolyGroupsEnd; boundarySPolyGroupsBegin++)
	{
		if (boundarySPolyGroupsBegin->second.boundaryPolySet.boundarySPolySG.isSupergroupEmpty() == false)
		{
			if (boundarySPolyGroupsBegin->first == MassZoneBoxBoundaryOrientation::NEG_Z)
			{
				//std::cout << "Attempting boundary artificial SPoly construction for NEG_Z..." << std::endl;
				tempBoundarySuperGroupLogger.log(prefixString, "SPolySG in NEG_Z contains SPolys, processing...", "\n");
			}
			else if (boundarySPolyGroupsBegin->first == MassZoneBoxBoundaryOrientation::POS_X)
			{
				//std::cout << "Attempting boundary artificial SPoly construction for POS_X..." << std::endl;
				tempBoundarySuperGroupLogger.log(prefixString, "SPolySG in POS_X contains SPolys, processing......", "\n");
			}
			else if (boundarySPolyGroupsBegin->first == MassZoneBoxBoundaryOrientation::POS_Z)
			{
				//std::cout << "Attempting boundary artificial SPoly construction for POS_Z..." << std::endl;
				tempBoundarySuperGroupLogger.log(prefixString, "SPolySG in POS_Z contains SPolys, processing......", "\n");
			}
			else if (boundarySPolyGroupsBegin->first == MassZoneBoxBoundaryOrientation::NEG_X)
			{
				//std::cout << "Attempting boundary artificial SPoly construction for NEG_X..." << std::endl;
				tempBoundarySuperGroupLogger.log(prefixString, "SPolySG in NEG_X contains SPolys, processing......", "\n");
			}
			else if (boundarySPolyGroupsBegin->first == MassZoneBoxBoundaryOrientation::POS_Y)
			{
				//std::cout << "Attempting boundary artificial SPoly construction for POS_Y..." << std::endl;
				tempBoundarySuperGroupLogger.log(prefixString, "SPolySG in POS_Y contains SPolys, processing......", "\n");
			}
			else if (boundarySPolyGroupsBegin->first == MassZoneBoxBoundaryOrientation::NEG_Y)
			{
				//std::cout << "Attempting boundary artificial SPoly construction for NEG_Y..." << std::endl;
				tempBoundarySuperGroupLogger.log(prefixString, "SPolySG in NEG_Y contains SPolys, processing......", "\n");
			}

			// optional logging output to print SPoly data
			if (tempBoundarySuperGroupLogger.isLoggingSet() == true)
			{
				boundarySPolyGroupsBegin->second.boundaryPolySet.boundarySPolySG.printSPolys();
			}

			// load the clipper with the selected super group's SPolys.
			clipper.insertSPolySuperGroupRefsIntoClippingShell(&boundarySPolyGroupsBegin->second.boundaryPolySet.boundarySPolySG);
			tempBoundarySuperGroupLogger.log(prefixString, "Size of clipper SPoly map:", clipper.clippingShellMap.size(), "\n");
		}
	}

	return shellCreationErrors;
}

void MassZone::enableContestedCategorizedLineAnalysis()
{
	zoneBox.setContestedCategorizedLineAnalysisFlagInBoundaries();
}