#pragma once

#ifndef IRLINE_H
#define IRLINE_H

#include <map>
#include <glm/glm.hpp>
#include "IRPointType.h"

class IRLine
{
public:
	std::map<IRPointType, glm::vec3> irPoints;
};

#endif