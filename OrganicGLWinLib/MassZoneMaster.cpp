#include "stdafx.h"
#include "MassZoneMaster.h"

void MassZoneMaster::registerSPolyToMassZone(int in_sPolyID, SPoly in_sPoly, MassZoneType in_massZoneType)
{
	sPolyZoneTypeMap[in_sPolyID] = in_massZoneType;
	if (in_massZoneType == MassZoneType::OLD_ZONE)
	{
		oldZone.insertSPolyMassSubZone(in_sPolyID, in_sPoly);
	}
	else if (in_massZoneType == MassZoneType::NEW_ZONE)
	{
		newZone.insertSPolyMassSubZone(in_sPolyID, in_sPoly);
	}
}

void MassZoneMaster::disqualifyMeshMatterMeta(int in_sPolyID)
{
	//std::cout << "!!! Disqualifying SPolyID: " << in_sPolyID << std::endl;
	auto typeFinder = sPolyZoneTypeMap.find(in_sPolyID);
	if (typeFinder != sPolyZoneTypeMap.end())	// it was found
	{
		if (typeFinder->second == MassZoneType::OLD_ZONE)
		{
			oldZone.removeMeshMatterMeta(in_sPolyID);
		}
		else if (typeFinder->second == MassZoneType::NEW_ZONE)
		{
			newZone.removeMeshMatterMeta(in_sPolyID);
		}
	}
}

void MassZoneMaster::setDOForClippableSPolyInClipper(int in_sPolyID, DebugOption in_debugOption)
{
	massZoneClippableOptionSets[in_sPolyID] += in_debugOption;
}

void MassZoneMaster::sendClippableDOSetsToClippers()
{
	auto typeMapBegin = sPolyZoneTypeMap.begin();
	auto typeMapEnd = sPolyZoneTypeMap.end();
	for (; typeMapBegin != typeMapEnd; typeMapBegin++)
	{
		// remember: sPolyZones of the OLD_ZONE go to the new zone, as that is where they are compared...
		if (typeMapBegin->second == MassZoneType::OLD_ZONE)
		{
			auto setFinder = massZoneClippableOptionSets.find(typeMapBegin->first);
			if (setFinder != massZoneClippableOptionSets.end())
			{
				auto currentSetBegin = massZoneClippableOptionSets[typeMapBegin->first].begin();
				auto currentSetEnd = massZoneClippableOptionSets[typeMapBegin->first].end();
				for (; currentSetBegin != currentSetEnd; currentSetBegin++)
				{
					newZone.insertDOForClippableSPoly(setFinder->first, *currentSetBegin);	// DebugOptionSets for OLD_ZONE sPolyZones are sent to the NEW_ZONE
				}
			}
		}

		// and vice versa
		else if (typeMapBegin->second == MassZoneType::NEW_ZONE)
		{
			auto setFinder = massZoneClippableOptionSets.find(typeMapBegin->first);
			if (setFinder != massZoneClippableOptionSets.end())
			{
				auto currentSetBegin = massZoneClippableOptionSets[typeMapBegin->first].begin();
				auto currentSetEnd = massZoneClippableOptionSets[typeMapBegin->first].end();
				for (; currentSetBegin != currentSetEnd; currentSetBegin++)
				{
					oldZone.insertDOForClippableSPoly(setFinder->first, *currentSetBegin);	// DebugOptionSets for NEW_ZONE sPolyZones are sent to the OLD_ZONE
				}
			}
		}
	}
}

void MassZoneMaster::printQualifiedMeshMatterMetas()
{
	std::cout << "Printing OLD zone qualifieds: " << std::endl;
	std::cout << "Old zone size: " << oldZone.subZoneMap.size() << std::endl;

	oldZone.printMeshMatterMeta();

	std::cout << "Printing NEW zone qualifieds: " << std::endl;
	std::cout << "New zone size: " << newZone.subZoneMap.size() << std::endl;
	newZone.printMeshMatterMeta();
}

void MassZoneMaster::createMassZoneBoxBoundaries(MassZoneBoxType in_massZoneBoxType)
{
	oldZone.createMassZoneBoxBoundary(in_massZoneBoxType);
	newZone.createMassZoneBoxBoundary(in_massZoneBoxType);
}

void MassZoneMaster::setZoneClipperReferences()
{
	oldZone.clipper.setOtherZoneMeshMatterMetaMapRef(&newZone.meshMatterMetaMap);	// old zone gets new zone's mesh matter meta
	newZone.clipper.setOtherZoneMeshMatterMetaMapRef(&oldZone.meshMatterMetaMap);
}

void MassZoneMaster::runPointClippers()
{
	sendClippableDOSetsToClippers();
	oldZone.runClipper();
	newZone.runClipper();
}

void MassZoneMaster::checkFissionTargets()
{
	std::cout << "::::::::::::::--------------> Checking fission targets in clippers. " << std::endl;
	oldZone.clipper.fissionTargets.printSet();
	newZone.clipper.fissionTargets.printSet();
}

void MassZoneMaster::setOldZoneClipperManipulationMode(MassManipulationMode in_massManipulationMode)
{
	oldZone.clipper.clipperManipulationMode = in_massManipulationMode;
}

void MassZoneMaster::setNewZoneClipperManipulationMode(MassManipulationMode in_massManipulationMode)
{
	newZone.clipper.clipperManipulationMode = in_massManipulationMode;
}

void MassZoneMaster::setMassZoneLogLevels(PolyDebugLevel in_polyDebugLevel)
{
	oldZone.setMassZoneLogLevel(in_polyDebugLevel);
	newZone.setMassZoneLogLevel(in_polyDebugLevel);
}

void MassZoneMaster::createMassZoneShells()
{
	//std::cout << " >>>>>>>>>>>>>>> Creating old zone mass shell..." << std::endl;
	oldZone.createMassZoneShell(MassZoneType::OLD_ZONE);
	//std::cout << ">>>>> Finished work with old zone mass shell... " << std::endl;
	//int oldZoneWork = 3;
	//std::cin >> oldZoneWork;

	//std::cout << " >>>>>>>>>>>>>>> Creating new zone mass shell..." << std::endl;
	newZone.createMassZoneShell(MassZoneType::NEW_ZONE);
	//std::cout << ">>>>> Finished work with new zone mass shell... " << std::endl;
	//int newZoneWork = 3;
	//std::cin >> newZoneWork;

	//std::cout << "!!! Run of production of zone mass shells complete; continue? " << std::endl;
	//int massZoneStopVal = 3;
	//std::cin >> massZoneStopVal;
}

void MassZoneMaster::printMassZoneBorderLineCounts()
{
	oldZone.zoneBox.printBoundaryLineCounts();
	newZone.zoneBox.printBoundaryLineCounts();
}

void MassZoneMaster::handleDebugOption(DebugOption in_debugOption)
{
	switch (in_debugOption)
	{
		////////////////// For oldZone

		// Generic options
		case (DebugOption::MASSZONEMASTER_OLDZONE_PRINT_BOUNDARY_CATEGORIZED_LINES): { oldZone.printBoundaryLinesLogLevel = PolyDebugLevel::DEBUG; break; };
		case (DebugOption::MASSZONEMASTER_OLDZONE_HALT_BETWEEN_BOUNDARY_SPOLY_CONSTRUCTION): {oldZone.boundarySPolyConstructionLogLevel = PolyDebugLevel::DEBUG; break; };
		case (DebugOption::MASSZONEMASTER_OLDZONE_POINT_CLIPPING): {oldZone.clipper.setClipperDebugLevel(PolyDebugLevel::DEBUG, "(OLD_ZONE) "); break; };

		// NEG_Z options (North)
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_SPOLY_MAIN): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Z, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_SPOLY_FACTORY): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Z, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_SPOLY_FACTORY_MERGER): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Z, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_FRACTURER): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Z, SPolyDO::FRACTURER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_CATEGORIZED_LINES) : { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Z, SPolyDO::CATEGORIZED_LINES); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS): {oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Z, SPolyDO::CATEGORIZED_LINE_COPLANAR_TESTS); break; }

        // POS_X options (East)
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_SPOLY_MAIN): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_X, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_SPOLY_FACTORY): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_X, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_SPOLY_FACTORY_MERGER): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_X, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_FRACTURER): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_X, SPolyDO::FRACTURER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_CATEGORIZED_LINES): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_X, SPolyDO::CATEGORIZED_LINES); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS): {oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_X, SPolyDO::CATEGORIZED_LINE_COPLANAR_TESTS); break; }

		// POS_Z options (South)
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_SPOLY_MAIN): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_Z, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_SPOLY_FACTORY): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_Z, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_SPOLY_FACTORY_MERGER): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_Z, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_FRACTURER): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_Z, SPolyDO::FRACTURER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_CATEGORIZED_LINES): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_Z, SPolyDO::CATEGORIZED_LINES); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS): {oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_Z, SPolyDO::CATEGORIZED_LINE_COPLANAR_TESTS); break; }

        // NEG_X options (West)
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_SPOLY_MAIN): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_X, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_SPOLY_FACTORY): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_X, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_SPOLY_FACTORY_MERGER): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_X, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_FRACTURER): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_X, SPolyDO::FRACTURER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_CATEGORIZED_LINES): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_X, SPolyDO::CATEGORIZED_LINES); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS): {oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_X, SPolyDO::CATEGORIZED_LINE_COPLANAR_TESTS); break; }

		// POS_Y options (Above)
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_SPOLY_MAIN): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_Y, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_SPOLY_FACTORY): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_Y, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_SPOLY_FACTORY_MERGER): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_Y, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_FRACTURER): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_Y, SPolyDO::FRACTURER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_CATEGORIZED_LINES): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_Y, SPolyDO::CATEGORIZED_LINES); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS): {oldZone.insertBoundaryDebugOption(BoundaryOrientation::POS_Y, SPolyDO::CATEGORIZED_LINE_COPLANAR_TESTS); break; }

		// NEG_Y options (Below)
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_SPOLY_MAIN): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Y, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_SPOLY_FACTORY): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Y, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_SPOLY_FACTORY_MERGER): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Y, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_FRACTURER): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Y, SPolyDO::FRACTURER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_CATEGORIZED_LINES): { oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Y, SPolyDO::CATEGORIZED_LINES); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS): {oldZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Y, SPolyDO::CATEGORIZED_LINE_COPLANAR_TESTS); break; }

		////////////////// For newZone																	 
		
		// Generic options
		case (DebugOption::MASSZONEMASTER_NEWZONE_PRINT_BOUNDARY_CATEGORIZED_LINES): { newZone.printBoundaryLinesLogLevel = PolyDebugLevel::DEBUG; break; };
		case (DebugOption::MASSZONEMASTER_NEWZONE_HALT_BETWEEN_BOUNDARY_SPOLY_CONSTRUCTION): {newZone.boundarySPolyConstructionLogLevel = PolyDebugLevel::DEBUG; break; };
		case (DebugOption::MASSZONEMASTER_NEWZONE_POINT_CLIPPING): {newZone.clipper.setClipperDebugLevel(PolyDebugLevel::DEBUG, "(NEW_ZONE) "); break; };

		// NEG_Z options (North)
		case (DebugOption::MASSZONEMASTER_NEWZONE_NEG_Z_BOUNDARY_SPOLY_MAIN): { newZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Z, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_NEG_Z_BOUNDARY_SPOLY_FACTORY): { newZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Z, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_NEG_Z_BOUNDARY_SPOLY_FACTORY_MERGER): { newZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Z, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_NEG_Z_BOUNDARY_FRACTURER): { newZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Z, SPolyDO::FRACTURER); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_NEG_Z_BOUNDARY_CATEGORIZED_LINES): { newZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Z, SPolyDO::CATEGORIZED_LINES); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_NEG_Z_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS): {newZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_Z, SPolyDO::CATEGORIZED_LINE_COPLANAR_TESTS); break; }

		// POS_X options (East)
		case (DebugOption::MASSZONEMASTER_NEWZONE_POS_X_BOUNDARY_SPOLY_MAIN): { newZone.insertBoundaryDebugOption(BoundaryOrientation::POS_X, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_POS_X_BOUNDARY_SPOLY_FACTORY): { newZone.insertBoundaryDebugOption(BoundaryOrientation::POS_X, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_POS_X_BOUNDARY_SPOLY_FACTORY_MERGER): { newZone.insertBoundaryDebugOption(BoundaryOrientation::POS_X, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_POS_X_BOUNDARY_FRACTURER): { newZone.insertBoundaryDebugOption(BoundaryOrientation::POS_X, SPolyDO::FRACTURER); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_POS_X_BOUNDARY_CATEGORIZED_LINES): { newZone.insertBoundaryDebugOption(BoundaryOrientation::POS_X, SPolyDO::CATEGORIZED_LINES); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_POS_X_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS): {newZone.insertBoundaryDebugOption(BoundaryOrientation::POS_X, SPolyDO::CATEGORIZED_LINE_COPLANAR_TESTS); break; }

		// NEG_X options (West)
		case (DebugOption::MASSZONEMASTER_NEWZONE_NEG_X_BOUNDARY_SPOLY_MAIN): { newZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_X, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_NEG_X_BOUNDARY_SPOLY_FACTORY): { newZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_X, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_NEG_X_BOUNDARY_SPOLY_FACTORY_MERGER): { newZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_X, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_NEG_X_BOUNDARY_FRACTURER): { newZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_X, SPolyDO::FRACTURER); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_NEG_X_BOUNDARY_CATEGORIZED_LINES): { newZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_X, SPolyDO::CATEGORIZED_LINES); break; }
		case (DebugOption::MASSZONEMASTER_NEWZONE_NEG_X_BOUNDARY_CATEGORIZED_LINE_COPLANAR_TESTS): {newZone.insertBoundaryDebugOption(BoundaryOrientation::NEG_X, SPolyDO::CATEGORIZED_LINE_COPLANAR_TESTS); break; }

		
	}
}