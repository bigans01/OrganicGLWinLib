#include "stdafx.h"
#include "InterceptRegister.h"

void InterceptRegister::insertCatLine(int in_borderLineID, int in_catLineGroupID, int in_catLineID, IRPointType in_pointType, glm::vec3 in_point)
{
	//insertIntersectingCatLine(int in_borderLineID, int in_catLineGroupID, int in_catLineID, glm::vec3 in_point)
	if (irGroups.find(in_borderLineID) == irGroups.end())	// if the group doesn't exist, create it
	{
		irGroups.insert(std::pair<int, IRGroup>(in_borderLineID, IRGroup()));	// insert a new IRGroup into the map
	}

	// grab the reference to the IRGroup (the one we just created in the previous line, if it didn't exist)	-- this gets us the appropriate BORDER line map
	IRGroup* irGroupRef = &irGroups[in_borderLineID];
	if (irGroupRef->irLines.find(in_catLineGroupID) == irGroupRef->irLines.end())
	{
		irGroupRef->irLines.insert(std::pair<int, IRLine>(in_catLineGroupID, IRLine()));	// insert a new IRLine into the map
	}

	IRLine* irLineRef = &irGroupRef->irLines[in_catLineGroupID];			// get the appropriate CATEGORIZED LINE in the BORDER LINE
	if (irLineRef->irPoints.find(in_pointType) == irLineRef->irPoints.end())
	{
		irLineRef->irPoints.insert(std::pair<IRPointType, glm::vec3>(in_pointType, in_point));		// insert the point type, and associated point
	}

	std::cout << "Inserted new intercept: " << std::endl;
	std::cout << "Border Line: " << in_borderLineID << std::endl;
	std::cout << "Group ID: " << in_catLineGroupID << std::endl;
	std::cout << "Line ID: " << in_catLineID << std::endl;

	int aorb = 0;
	if (in_pointType == IRPointType::POINT_A)
	{
		aorb = 0;
	}
	else if (in_pointType == IRPointType::POINT_B)
	{
		aorb = 1;
	}

	/*
	std::cout << "A or B: " << aorb << std::endl;
	std::cout << "Point: " << in_point.x << ", " << in_point.y << ", " << in_point.z << std::endl;
	*/
}