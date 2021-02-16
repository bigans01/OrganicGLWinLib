#pragma once

#ifndef CUTTRIANGLEPRODUCTIONRESULT_H
#define CUTTRIANGLEPRODUCTIONRESULT_H

#include "CutTriangle.h"

class CutTriangleProductionResult
{
	public:
		CutTriangleProductionResult() {};
		CutTriangleProductionResult(bool in_wasProductionValid, CutTriangle in_producedTriangle) :
			wasProductionValid(in_wasProductionValid),
			producedCutTriangle(in_producedTriangle) {};
		bool wasProductionValid = false;
		CutTriangle producedCutTriangle;
};

#endif
