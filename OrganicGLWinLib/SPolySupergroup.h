#pragma once

#ifndef SPOLYSUPERGROUP_H
#define SPOLYSUPERGROUP_H

#include <map>
#include "SPoly.h"

class SPolySupergroup
{
	public:
		std::map<int, SPoly> sPolyMap;
		void insertSPoly(SPoly in_sPoly)
		{
			sPolyMap[sPolyMap.size()] = in_sPoly;
		}

		void printSPolys()
		{
			auto sPolysBegin = sPolyMap.begin();
			auto sPolysEnd = sPolyMap.end();
			for (; sPolysBegin != sPolysEnd; sPolysBegin++)
			{
				std::cout << ">>> SPoly: " << sPolysBegin->first << std::endl;
				auto sTrianglesBegin = sPolysBegin->second.triangles.begin();
				auto sTrianglesEnd = sPolysBegin->second.triangles.end();
				for (; sTrianglesBegin != sTrianglesEnd; sTrianglesBegin++)
				{
					std::cout << "STriangle: " << sTrianglesBegin->first << std::endl;
					for (int x = 0; x < 3; x++)
					{
						std::cout << "Point " << x << ": " 
							<< sTrianglesBegin->second.triangleLines[x].pointA.x << ", "
							<< sTrianglesBegin->second.triangleLines[x].pointA.y << ", "
							<< sTrianglesBegin->second.triangleLines[x].pointA.z << std::endl;
					}
				}
			}
		}
};

#endif