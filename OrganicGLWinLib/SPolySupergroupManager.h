#pragma once

#ifndef SPOLYSUPERGROUPMANAGER_H
#define SPOLYSUPERGROUPMANAGER_H

#include "SPolySupergroup.h"

class SPolySupergroupManager
{
public:
	std::map<int, SPolySupergroup> supergroupMap;
	void insertSPolyIntoSuperGroup(int in_supergroupID, SPoly in_sPoly)
	{
		supergroupMap[in_supergroupID].insertSPoly(in_sPoly);
	}
	void printSupergroups()
	{
		auto supergroupMapBegin = supergroupMap.begin();
		auto supergroupMapEnd = supergroupMap.end();
		for (; supergroupMapBegin != supergroupMapEnd; supergroupMapBegin++)
		{
			std::cout << ">>>>> Parent poly ID: " << supergroupMapBegin->first << std::endl;
			supergroupMapBegin->second.printSPolys();
		}
	}
};

#endif

