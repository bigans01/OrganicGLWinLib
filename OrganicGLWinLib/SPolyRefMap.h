#pragma once

#ifndef SPOLYREFMAP_H
#define SPOLYREFMAP_H

#include <map>
#include "SPoly.h"

class SPolyRefMap
{
	public:
		/*
		std::map<int, SPoly*> refMap;
		void insertSPolyRef(int in_sPolyIndex, SPoly* in_sPolyRef)
		{
			refMap[in_sPolyIndex] = in_sPolyRef;
		}
		*/

		std::map<int, SPoly> refMap;
		void insertSPolyRef(int in_sPolyIndex, SPoly in_sPolyRef)
		{
			refMap[in_sPolyIndex] = in_sPolyRef;
		}
};

#endif
