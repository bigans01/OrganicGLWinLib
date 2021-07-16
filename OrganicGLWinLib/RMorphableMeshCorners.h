#pragma once

#ifndef RMORPHABLEMESHCORNERS_H
#define RMORPHABLEMESHCORNERS_H

#include "RCollisionPoint.h"
#include <iostream>

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

		void updateUsageCounts()
		{
			lowerNW->incrementUsageCount();
			lowerSW->incrementUsageCount();
			lowerSE->incrementUsageCount();
			lowerNE->incrementUsageCount();
			lowerNW->incrementUsageCount();
			lowerSW->incrementUsageCount();
			lowerSE->incrementUsageCount();
			upperNE->incrementUsageCount();
		}

		void printCornerData()
		{
			std::cout << "(RMorphableMeshCorners): printing corner points and orientations: " << std::endl;
			std::cout << "Lower NW: "; lowerNW->printPointAndOrientation();
			std::cout << "Lower SW: "; lowerSW->printPointAndOrientation();
			std::cout << "Lower SE: "; lowerSE->printPointAndOrientation();
			std::cout << "Lower NE: "; lowerNE->printPointAndOrientation();
			std::cout << "Upper NW: "; upperNW->printPointAndOrientation();
			std::cout << "Upper SW: "; upperSW->printPointAndOrientation();
			std::cout << "Upper SE: "; upperSE->printPointAndOrientation();
			std::cout << "Upper NE: "; upperNE->printPointAndOrientation();
		}

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
