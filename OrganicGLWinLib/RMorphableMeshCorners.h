#pragma once

#ifndef RMORPHABLEMESHCORNERS_H
#define RMORPHABLEMESHCORNERS_H

#include "RCollisionPoint.h"

class RMorphableMeshCorners
{
	public:
		RMorphableMeshCorners() {};
		void setCorners(RCollisionPoint* in_lowerNW,
			RCollisionPoint* in_lowerSW,
			RCollisionPoint* in_lowerSE,
			RCollisionPoint* in_lowerNE,
			RCollisionPoint* in_upperNW,
			RCollisionPoint* in_upperSW,
			RCollisionPoint* in_upperSE,
			RCollisionPoint* in_upperNE
		)
		{

		};

		RCollisionPoint* in_lowerSW = nullptr;
		RCollisionPoint* in_lowerSE = nullptr;
		RCollisionPoint* in_lowerNE = nullptr;
		RCollisionPoint* in_upperNW = nullptr;
		RCollisionPoint* in_upperSW = nullptr;
		RCollisionPoint* in_upperSE = nullptr;
		RCollisionPoint* in_upperNE = nullptr;
};

#endif
