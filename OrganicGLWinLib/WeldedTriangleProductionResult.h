#pragma once

#ifndef WELDEDTRIANGLEPRODUCTIONRESULT_H
#define WELDEDTRIANGLEPRODUCTIONRESULT_H

#include "WeldedTriangle.h"

class WeldedTriangleProductionResult
{
	public:
		WeldedTriangleProductionResult() {};
		WeldedTriangleProductionResult(bool in_wasProductionValid, WeldedTriangle in_producedTriangle) :
			wasProductionValid(in_wasProductionValid),
			producedTriangle(in_producedTriangle)
		{

		};

		bool wasProductionValid;
		WeldedTriangle producedTriangle;
};

#endif
