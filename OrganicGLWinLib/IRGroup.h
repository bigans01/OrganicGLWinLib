#pragma once

#ifndef IRGROUP_H
#define IRGROUP_H

#include <map>
#include "IRLine.h"

class IRGroup
{
public:
	std::map<int, IRLine> irLines;
};

#endif