#pragma once

#ifndef SPOLYSUPERGROUPMANAGER_H
#define SPOLYSUPERGROUPMANAGER_H

#include <map>
#include "SPolySupergroup.h"

class SPolySupergroupManager
{
public:
	std::map<int, SPolySupergroup> supergroupMap;
	void insertSPolyIntoSuperGroup(int in_supergroupID, SPoly in_sPoly)
	{
		supergroupMap[in_supergroupID].insertSPoly(in_sPoly);
	}
};

#endif

