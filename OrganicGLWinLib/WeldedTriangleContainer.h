#pragma once

#ifndef WELDEDTRIANGLECONTAINER_H
#define WELDEDTRIANGLECONTAINER_H

#include "WeldedTriangle.h"

class WeldedTriangleContainer
{
	public:
		std::map<int, WeldedTriangle> triangleMap;
		void insertWeldedTriangleViaIndex(int in_weldedTriangleID, WeldedTriangle in_weldedTriangle)
		{
			triangleMap[in_weldedTriangleID] = in_weldedTriangle;
		};
		void insertWeldedTriangle(WeldedTriangle in_weldedTriangle)
		{
			triangleMap[int(triangleMap.size())] = in_weldedTriangle;
		};
};

#endif
