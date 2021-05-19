#pragma once

#ifndef RASTERIZATIONPOINTADJUSTER_H
#define RASTERIZATIONPOINTADJUSTER_H

#include "QuatRotationPoints.h"
#include "MassZoneBoxType.h"
#include <glm/glm.hpp>
#include <algorithm>

// this class assumes the following conditions, prior to its non-default constructor call:
// -1.) the reference to QuatRotationPoints is an instance of that class, such that all points have been aligned to the same Z-plane.
// -2.) assuming 1. is true, all points have been translated appropriately by their MassZoneBoxType value, PRIOR to this constructor call

class RasterizationPointAdjuster
{
	public:
		RasterizationPointAdjuster() {};
		RasterizationPointAdjuster(QuatRotationPoints* in_pointsRef, MassZoneBoxType in_massZoneBoxType) :
			pointsRef(in_pointsRef),
			rasterizationPlaneSize(in_massZoneBoxType)
		{
			switch (rasterizationPlaneSize)
			{
				case MassZoneBoxType::BLOCK: 
				{ 
					rasterizationPlaneCenterPoint = glm::vec3(0.5, 0.5, 0.5); 
					currentMaxNegPosBorder = maxNegPosBorderBase;
					break; 
				}
				case MassZoneBoxType::ENCLAVE: 
				{ 
					rasterizationPlaneCenterPoint = glm::vec3(2, 2, 2); 
					currentMaxNegPosBorder = maxNegPosBorderBase * 4;
					break;
				}
				
				case MassZoneBoxType::COLLECTION: 
				{ 
					rasterizationPlaneCenterPoint = glm::vec3(16,16,16); 
					currentMaxNegPosBorder = maxNegPosBorderBase * 32;
					break; 
				}
			};

			runFitScans();
		};
	private:
		QuatRotationPoints* pointsRef = nullptr;
		MassZoneBoxType rasterizationPlaneSize = MassZoneBoxType::NOVAL;
		glm::vec3 rasterizationPlaneCenterPoint;
		const float maxNegPosBorderBase = 1.732052 / 2;
		float currentMaxNegPosBorder = 0.0f;	

		void runFitScans();
		void runFitScanX();
		void runFitScanY();
};

#endif
