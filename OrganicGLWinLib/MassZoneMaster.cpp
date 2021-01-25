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
	std::cout << "!!! Disqualifying SPolyID: " << in_sPolyID << std::endl;
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

void MassZoneMaster::setMassZoneLogLevels(PolyDebugLevel in_polyDebugLevel)
{
	oldZone.setMassZoneLogLevel(in_polyDebugLevel);
	newZone.setMassZoneLogLevel(in_polyDebugLevel);
}

void MassZoneMaster::createMassZoneShells()
{
	std::cout << " >>>>>>>>>>>>>>> Creating old zone mass shell..." << std::endl;
	oldZone.createMassZoneShell();
	std::cout << ">>>>> Finished work with old zone mass shell... " << std::endl;
	int oldZoneWork = 3;
	std::cin >> oldZoneWork;

	std::cout << " >>>>>>>>>>>>>>> Creating new zone mass shell..." << std::endl;
	newZone.createMassZoneShell();
	std::cout << ">>>>> Finished work with new zone mass shell... " << std::endl;
	int newZoneWork = 3;
	std::cin >> newZoneWork;

	std::cout << "!!! Run of production of zone mass shells complete; continue? " << std::endl;
	int massZoneStopVal = 3;
	std::cin >> massZoneStopVal;
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
		// NEG_Z options (North)
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_SPOLY_MAIN): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::NEG_Z, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_SPOLY_FACTORY): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::NEG_Z, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_SPOLY_FACTORY_MERGER): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::NEG_Z, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Z_BOUNDARY_FRACTURER): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::NEG_Z, SPolyDO::FRACTURER); break; }

        // POS_X options (East)
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_SPOLY_MAIN): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::POS_X, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_SPOLY_FACTORY): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::POS_X, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_SPOLY_FACTORY_MERGER): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::POS_X, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_X_BOUNDARY_FRACTURER): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::POS_X, SPolyDO::FRACTURER); break; }

		// POS_Z options (South)
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_SPOLY_MAIN): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::POS_Z, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_SPOLY_FACTORY): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::POS_Z, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_SPOLY_FACTORY_MERGER): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::POS_Z, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Z_BOUNDARY_FRACTURER): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::POS_Z, SPolyDO::FRACTURER); break; }

        // NEG_X options (West)
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_SPOLY_MAIN): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::NEG_X, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_SPOLY_FACTORY): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::NEG_X, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_SPOLY_FACTORY_MERGER): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::NEG_X, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_X_BOUNDARY_FRACTURER): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::NEG_X, SPolyDO::FRACTURER); break; }

		// POS_Y options (Above)
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_SPOLY_MAIN): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::POS_Y, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_SPOLY_FACTORY): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::POS_Y, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_SPOLY_FACTORY_MERGER): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::POS_Y, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_POS_Y_BOUNDARY_FRACTURER): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::POS_Y, SPolyDO::FRACTURER); break; }

		// NEG_Y options (Below)
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_SPOLY_MAIN): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::NEG_Y, SPolyDO::MAIN); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_SPOLY_FACTORY): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::NEG_Y, SPolyDO::FACTORY); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_SPOLY_FACTORY_MERGER): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::NEG_Y, SPolyDO::FACTORY_MERGER); break; }
		case (DebugOption::MASSZONEMASTER_OLDZONE_NEG_Y_BOUNDARY_FRACTURER): { oldZone.insertBoundaryDebugOption(MassZoneBoxBoundaryOrientation::NEG_Y, SPolyDO::FRACTURER); break; }

	}
}