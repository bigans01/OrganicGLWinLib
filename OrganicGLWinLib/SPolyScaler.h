#pragma once

#ifndef SPOLYSCALER_H
#define SPOLYSCALER_H

#include "SPoly.h"
#include "OrganicGLWinUtils.h"
#include "QuatRotationPoints.h"
#include "PointTranslationCheck.h"
#include "SPolyScalingOps.h"

// purpose: translates, rotates, and scales an SPoly to fit a 32 x 32 x 32 area (EnclaveCollection's total mass); scale percentage reflects how much 
// you want to fit it to (half, 3/4, etc)

class SPolyScaler
{
	public:
		SPolyScaler(SPoly in_scalableSPoly, float in_scalePercentage) :
			scalableSPoly(in_scalableSPoly),
			scalablePercentage(in_scalePercentage)
		{
			// only continue if the SPoly has STriangles to process
			if (scalableSPoly.triangles.size() != 0)
			{
				translateAndRotateToXZPlane();
			}
			else
			{
				std::cout << "(SPolyScaler): notice, SPoly had no STriangles. Cancelling scaling process." << std::endl;
			}
		};
	private:
		SPoly scalableSPoly;
		float scalablePercentage;
		SPolyScalingOps scalingOps;				// records any scaling operations, so that we may apply the inverse of them later to get the SPoly back to it's original "spot"

		void translateAndRotateToXZPlane();		// translate to a 2D XZ plane (y = 0)
		//glm::vec3 findCenterPoint();					// a point that exists in the SPoly that we will use to "center" it on
		QuatRotationPoints sPolyQuatPoints;		// stores the various values of the SPoly (border points, empty normal)
};

#endif
