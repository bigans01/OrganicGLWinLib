#pragma once

#ifndef RASTERCUBETRACERBASE_H
#define RASTERCUBETRACERBASE_H

#include "RasterCubeLookup.h"
#include "EnclaveKeyDef.h"
#include <glm/glm.hpp>
#include "DynamicBorderLineList.h"
#include "IndependentUtils.h"
#include "ECBPolyPointTri.h"
#include "DynamicEndpointMeta.h"
#include "DynamicLinePointSynchronizer.h"

class RasterCubeTracerBase
{
	public:
		void setData(EnclaveKeyDef::EnclaveKey in_startCubeKey,
					 EnclaveKeyDef::EnclaveKey in_endCubeKey,
					 glm::vec3 in_startCubePoint,
					 glm::vec3 in_endCubePoint,
					 float in_rasterCubeDimLength,
					 bool in_debugFlag)
		{
			debugFlag = in_debugFlag;

			startCubeKey = in_startCubeKey;
			currentCubeKey = in_startCubeKey;
			endCubeKey = in_endCubeKey;

			startCubePoint = in_startCubePoint;
			endCubePoint = in_endCubePoint;
			rasterCubeDimLength = in_rasterCubeDimLength;

			// the beginning tracing point is always equal to the center of a cube, which is equal to a point having half the value 
			// for rasterCubeDimLength for x, y and z directions.
			glm::vec3 tracingPoint(rasterCubeDimLength, rasterCubeDimLength, rasterCubeDimLength);
			currentTracingPoint = tracingPoint;
			currentTracingPoint /= 2;			

			tracerDirectionVector = endCubePoint - startCubePoint;
			ECBPolyPoint convertedDirectionVector(tracerDirectionVector.x, tracerDirectionVector.y, tracerDirectionVector.z);
			normalizedSlopeDirection = IndependentUtils::findNormalizedPoint(convertedDirectionVector);

			//lineLengthDownscaleValue = in_lineLengthDownscaleValue;
			//lineLength = glm::distance(startCubePoint, endCubePoint) / lineLengthDownscaleValue;
			//lineLength = glm::distance(startCubePoint, endCubePoint);
			lineLength = glm::distance(startCubePoint, endCubePoint) / 100;
			remainingDistance = lineLength;

			dynamicBorder.constructBorders(rasterCubeDimLength);
		}
		virtual void runTrace() = 0;
		void iterateToNextBlock();

	protected:
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
		//float lineLengthDownscaleValue = 0.0f;
		float remainingDistance = 0.0f;
		bool isRunComplete = false;

		// optional values
		RasterCubeLookup* optionalCubeLookup = nullptr;
		bool optionalMatchingBool = false;

		DynamicEndpointMeta getDynamicEndpointMetaData(ECBPolyPoint in_originPoint, 
													   DynamicBorderLineList* in_blockBorderRef, 
			                                           ECBPolyPoint in_distanceValues, 
			                                           ECBPolyPoint in_slopeDirection, 
			                                           ECBPolyPointTri in_XYZinterceptCoords);
		ECBPolyPoint roundXYZInterceptDistancesToAppropriatePrecision(float in_distX, float in_distY, float in_distZ);
		ECBPolyPoint roundToNearestDynamicLineOrCorner(int in_xoryorz, ECBPolyPoint in_polyPoint, int in_lineOrCorner);

		bool debugFlag = false;
};

#endif
