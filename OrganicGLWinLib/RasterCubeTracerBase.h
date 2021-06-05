#pragma once

#ifndef RASTERCUBETRACERBASE_H
#define RASTERCUBETRACERBASE_H

#include "RasterCubeLookup.h"
#include "EnclaveKeyDef.h"
#include <glm/glm.hpp>
#include "DynamicBorderLineList.h"
#include "IndependentUtils.h"
#include "ECBPolyPointTri.h"

class RasterCubeTracerBase
{
	public:
		void setData(EnclaveKeyDef::EnclaveKey in_startCubeKey,
					 EnclaveKeyDef::EnclaveKey in_endCubeKey,
					 glm::vec3 in_startCubePoint,
					 glm::vec3 in_endCubePoint,
					 float in_rasterCubeDimLength)
		{
			startCubeKey = in_startCubeKey;
			currentCubeKey = in_startCubeKey;
			endCubeKey = in_endCubeKey;

			startCubePoint = in_startCubePoint;
			currentTracingPoint = in_startCubePoint;
			endCubePoint = in_endCubePoint;
			rasterCubeDimLength = in_rasterCubeDimLength;

			tracerDirectionVector = endCubePoint - startCubePoint;
			ECBPolyPoint convertedDirectionVector(tracerDirectionVector.x, tracerDirectionVector.y, tracerDirectionVector.z);
			normalizedSlopeDirection = IndependentUtils::findNormalizedPoint(convertedDirectionVector);

			lineLength = glm::distance(startCubePoint, endCubePoint);

			dynamicBorder.constructBorders(rasterCubeDimLength);
		}
		void iterateToNextBlock();

	private:
		DynamicBorderLineList dynamicBorder;

		EnclaveKeyDef::EnclaveKey startCubeKey;
		EnclaveKeyDef::EnclaveKey currentCubeKey;
		EnclaveKeyDef::EnclaveKey endCubeKey;

		glm::vec3 startCubePoint;
		glm::vec3 currentTracingPoint;
		glm::vec3 endCubePoint;
		glm::vec3 tracerDirectionVector;

		ECBPolyPoint normalizedSlopeDirection;

		float rasterCubeDimLength = 0.0f;
		float lineLength = 0.0f;
		bool isRunComplete = false;

		// optional values
		RasterCubeLookup* optionalCubeLookup = nullptr;
		bool optionalMatchingBool = false;
};

#endif
