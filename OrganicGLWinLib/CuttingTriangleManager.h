#pragma once

#ifndef CUTTINGTRIANGLEMANAGER_H
#define CUTTINGTRIANGLEMANAGER_H

#include <map>
#include "CuttingTriangle.h"

class CuttingTriangleManager
{
	public:
		std::map<int, CuttingTriangle> cuttingTriangles;
};

#endif
