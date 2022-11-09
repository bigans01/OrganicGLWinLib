#pragma once

#ifndef INTERCEPTREGISTER_H
#define INTERCEPTREGISTER_H

#include "IRGroup.h"
#include "IRPointType.h"

class InterceptRegister
{
public:
	std::map<int, IRGroup> irGroups;
	void insertCatLine(int in_borderLineID, int in_catLineGroupID, int in_catLineID, IRPointType in_pointType, glm::vec3 in_point);
};

#endif
