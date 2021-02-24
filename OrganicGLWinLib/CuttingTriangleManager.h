#pragma once

#ifndef CUTTINGTRIANGLEMANAGER_H
#define CUTTINGTRIANGLEMANAGER_H

#include <map>
#include "CuttingTriangle.h"

class CuttingTriangleManager
{
	public:
		void insertCuttingTriangle(STriangle in_sTriangle);
		std::map<int, CuttingTriangle> cuttingTriangles;
		void printCuttingTriangles();
};

#endif
