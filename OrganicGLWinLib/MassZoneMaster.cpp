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
	std::cout << " >>>>>>>>>>>>>>> Creating new zone mass shell..." << std::endl;
	newZone.createMassZoneShell();

	std::cout << "!!! Run of production of zone mass shells complete; continue? " << std::endl;
	int massZoneStopVal = 3;
	std::cin >> massZoneStopVal;
}

void MassZoneMaster::printMassZoneBorderLineCounts()
{
	oldZone.zoneBox.printBoundaryLineCounts();
	newZone.zoneBox.printBoundaryLineCounts();
}