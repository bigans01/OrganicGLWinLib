#pragma once

#ifndef SPOLYMORPHTRACKER_H
#define SPOLYMORPHTRACKER_H

#include <map>
#include "SPoly.h"



class SPolyMorphTracker
{
public:
	std::map<int, SPoly> morphablePolys;
};

#endif