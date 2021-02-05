#pragma once

#ifndef CUTTABLETRIANGLE_H
#define CUTTABLETRIANGLE_H

#include "STriangle.h"
#include "CuttingTriangle.h"
#include <map>

class CuttableTriangle
{
	public:
		void buildCuttableTriangle(STriangle in_cuttableTriangle);
		void compareAgainstCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef);
	private:
		friend class STriangleOutputContainer;
		friend class STriangleCutter;
		std::map<int, STriangle> outputTriangles;
};

#endif
