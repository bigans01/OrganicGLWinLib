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
			lowerNW = in_lowerNW;
			lowerSW = in_lowerSW;
			lowerSE = in_lowerSE;
			lowerNE = in_lowerNE;
			upperNW = in_upperNW;
			upperSW = in_upperSW;
			upperSE = in_upperSE;
			upperNE = in_upperNE;
		};

		RCollisionPoint* lowerNW = nullptr;
		RCollisionPoint* lowerSW = nullptr;
		RCollisionPoint* lowerSE = nullptr;
		RCollisionPoint* lowerNE = nullptr;
		RCollisionPoint* upperNW = nullptr;
		RCollisionPoint* upperSW = nullptr;
		RCollisionPoint* upperSE = nullptr;
		RCollisionPoint* upperNE = nullptr;
};

#endif
