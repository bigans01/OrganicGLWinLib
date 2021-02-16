#pragma once

#ifndef CUTTRIANGLECONTAINER_H
#define CUTTRIANGLECONTAINER_H

#include <map>
#include "CutTriangle.h"

class CutTriangleContainer
{
	public:
		std::map<int, CutTriangle> cutTrianglesMap;
		void insertCutTriangleViaIndex(int in_cutTriangleID, CutTriangle in_cutTriangle)
		{
			cutTrianglesMap[in_cutTriangleID] = in_cutTriangle;
		}
		void insertWeldedTriangle(CutTriangle in_cutTriangle)
		{
			cutTrianglesMap[int(cutTrianglesMap.size())] = in_cutTriangle;
		}
};

#endif
