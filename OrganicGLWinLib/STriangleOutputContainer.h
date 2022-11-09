#pragma once

#ifndef STRIANGLEOUTPUTCONTAINER_H
#define STRIANGLEOUTPUTCONTAINER_H

#include "STriangle.h"

class STriangleOutputContainer
{
	public:
		void insertOutputSTriangles(std::map<int, STriangle>* in_sTriangleMapRef)
		{
			auto mapRefBegin = in_sTriangleMapRef->begin();
			auto mapRefEnd = in_sTriangleMapRef->end();
			for (; mapRefBegin != mapRefEnd; mapRefBegin++)
			{
				sTriangleOutputVector.push_back(mapRefBegin->second);
			}
		}
		std::vector<STriangle> sTriangleOutputVector;

};

#endif
