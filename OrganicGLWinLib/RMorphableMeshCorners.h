#pragma once

#ifndef RMORPHABLEMESHCORNERS_H
#define RMORPHABLEMESHCORNERS_H

#include "RCollisionPoint.h"
#include <iostream>
#include "RMorphableMeshCornerArray.h"
#include "RProductFaceRootPoints.h"

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

		RMorphableMeshCornerArray getPointArray()
		{
			RMorphableMeshCornerArray returnArray(lowerNW,
												lowerSW,
												lowerSE,
												lowerNE,
												upperNW,
												upperSW,
												upperSE,
												upperNE);
			return returnArray;

		}

		bool checkIfFaceIsUsable(ECBPPOrientations in_point0Orientation,
			ECBPPOrientations in_point1Orientation,
			ECBPPOrientations in_point2Orientation,
			ECBPPOrientations in_point3Orientation)
		{
			bool isUsable = false;

			bool point0Landlocked = isPointImmutable(in_point0Orientation);
			bool point1Landlocked = isPointImmutable(in_point1Orientation);
			bool point2Landlocked = isPointImmutable(in_point2Orientation);
			bool point3Landlocked = isPointImmutable(in_point3Orientation);

			if
			(
				(point0Landlocked == false)
				&&
				(point1Landlocked == false)
				&&
				(point2Landlocked == false)
				&&
				(point3Landlocked == false)
			)
			{
				isUsable = true;
			}
			return isUsable;
		}

		bool isPointImmutable(ECBPPOrientations in_pointOrientation)
		{
			bool isImmutable = false;
			switch (in_pointOrientation)
			{
				case ECBPPOrientations::CORNER_LOWERNW: { isImmutable = lowerNW->isPointLandlocked(); break; }
				case ECBPPOrientations::CORNER_LOWERSW: { isImmutable = lowerSW->isPointLandlocked(); break; }
				case ECBPPOrientations::CORNER_LOWERSE: { isImmutable = lowerSE->isPointLandlocked(); break; }
				case ECBPPOrientations::CORNER_LOWERNE: { isImmutable = lowerNE->isPointLandlocked(); break; }
				case ECBPPOrientations::CORNER_UPPERNW: { isImmutable = upperNW->isPointLandlocked(); break; }
				case ECBPPOrientations::CORNER_UPPERSW: { isImmutable = upperSW->isPointLandlocked(); break; }
				case ECBPPOrientations::CORNER_UPPERSE: { isImmutable = upperSE->isPointLandlocked(); break; }
				case ECBPPOrientations::CORNER_UPPERNE: { isImmutable = upperNE->isPointLandlocked(); break; }
			}
			return isImmutable;
		};


		RProductFaceRootPoints fetchRootPoints(ECBPPOrientations in_point0Orientation,
			ECBPPOrientations in_point1Orientation,
			ECBPPOrientations in_point2Orientation,
			ECBPPOrientations in_point3Orientation)
		{
			RCollisionPoint* point0Ref = fetchPointRefViaOrientation(in_point0Orientation);
			RCollisionPoint* point1Ref = fetchPointRefViaOrientation(in_point1Orientation);
			RCollisionPoint* point2Ref = fetchPointRefViaOrientation(in_point2Orientation);
			RCollisionPoint* point3Ref = fetchPointRefViaOrientation(in_point3Orientation);
			RProductFaceRootPoints returnRootPoints(point0Ref, point1Ref, point2Ref, point3Ref);
			return returnRootPoints;
		}

		RCollisionPoint* lowerNW = nullptr;
		RCollisionPoint* lowerSW = nullptr;
		RCollisionPoint* lowerSE = nullptr;
		RCollisionPoint* lowerNE = nullptr;
		RCollisionPoint* upperNW = nullptr;
		RCollisionPoint* upperSW = nullptr;
		RCollisionPoint* upperSE = nullptr;
		RCollisionPoint* upperNE = nullptr;

	private:
		RCollisionPoint* fetchPointRefViaOrientation(ECBPPOrientations in_pointOrientation)
		{
			RCollisionPoint* pointRef = nullptr;
			switch (in_pointOrientation)
			{
				case ECBPPOrientations::CORNER_LOWERNW: { pointRef = lowerNW; break; }
				case ECBPPOrientations::CORNER_LOWERSW: { pointRef = lowerSW; break; }
				case ECBPPOrientations::CORNER_LOWERSE: { pointRef = lowerSE; break; }
				case ECBPPOrientations::CORNER_LOWERNE: { pointRef = lowerNE; break; }
				case ECBPPOrientations::CORNER_UPPERNW: { pointRef = upperNW; break; }
				case ECBPPOrientations::CORNER_UPPERSW: { pointRef = upperSW; break; }
				case ECBPPOrientations::CORNER_UPPERSE: { pointRef = upperSE; break; }
				case ECBPPOrientations::CORNER_UPPERNE: { pointRef = upperNE; break; }
			}
			return pointRef;
		};

};

#endif
