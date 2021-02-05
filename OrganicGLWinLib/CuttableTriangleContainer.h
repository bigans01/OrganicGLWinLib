#pragma once

#ifndef CUTTABLETRIANGLECONTAINER_H
#define CUTTABLETRIANGLECONTAINER_H

#include <map>
#include "STriangle.h"
#include "CuttableTriangle.h"
#include "STriangleOutputContainer.h"

class CuttableTriangleContainer
{
	private:
		friend class STriangleCutter;
		void buildFirstCuttableTriangle(STriangle in_firstTriangle);
		void insertNewCuttableTriangle(STriangle in_sTriangle);
		void rebuildCuttableTriangleMapFromContainer(STriangleOutputContainer* in_sTriangleOutputContainerRef);
		std::map<int, CuttableTriangle> cuttableTriangleMap;
};

#endif
