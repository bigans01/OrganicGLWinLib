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
					rasterizationPlaneCenterPoint = glm::vec3(0.5, 0.5, 0); 
					currentMaxNegPosBorder = maxNegPosBorderBase;
					downsizeFactor = 0.5f / currentMaxNegPosBorder;
					break; 
				}
				case MassZoneBoxType::ENCLAVE: 
				{ 
					rasterizationPlaneCenterPoint = glm::vec3(2, 2, 0); 
					currentMaxNegPosBorder = maxNegPosBorderBase * 4;
					downsizeFactor = 2 / currentMaxNegPosBorder;
					break;
				}
				
				case MassZoneBoxType::COLLECTION: 
				{ 
					rasterizationPlaneCenterPoint = glm::vec3(16,16,0); 
					currentMaxNegPosBorder = maxNegPosBorderBase * 32;
					downsizeFactor = 16 / currentMaxNegPosBorder;
					break; 
				}
			};

			runFitScans();
			applyDownsizeFactor();
			translatePointsBackToRasterizationBox();

			//std::cout << "++++++++ adjustment complete; points are: " << std::endl;
			//pointsRef->printPoints();
		};
	private:
		QuatRotationPoints* pointsRef = nullptr;
		MassZoneBoxType rasterizationPlaneSize = MassZoneBoxType::NOVAL;
		glm::vec3 rasterizationPlaneCenterPoint;
		const float maxNegPosBorderBase = 1.732052f / 2.0f;
		float currentMaxNegPosBorder = 0.0f;
		float downsizeFactor = 0.0f;			
		bool isScalingNeeded = false;

		void runFitScans();
		void runFitScanX();
		void runFitScanY();
		void applyDownsizeFactor();
		void translatePointsBackToRasterizationBox();
};

#endif
