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
#include "PolyLogger.h"

class RasterCubeTracerBase
{
	public:
		void setData(EnclaveKeyDef::EnclaveKey in_startCubeKey,
			EnclaveKeyDef::EnclaveKey in_endCubeKey,
			glm::vec3 in_startCubePoint,
			glm::vec3 in_endCubePoint,
			float in_rasterCubeDimLength,
			float in_tileWeightRatio,
			bool in_debugFlag);

		void setUpNextRun(EnclaveKeyDef::EnclaveKey in_startCubeKey,
			EnclaveKeyDef::EnclaveKey in_endCubeKey,
			glm::vec3 in_startCubePoint,
			glm::vec3 in_endCubePoint);

		void setOptionalCubeLookupRef(RasterCubeLookup* in_optionalCubeLookupRef);
		void setOptionalPolyLoggerRef(PolyLogger* in_optionalLoggerRef);

		virtual void runTrace() = 0;
		void iterateToNextBlock();
		bool isRunComplete = false;
		EnclaveKeyDef::EnclaveKey startCubeKey;
		EnclaveKeyDef::EnclaveKey currentCubeKey;
		EnclaveKeyDef::EnclaveKey endCubeKey;

		glm::vec3 startCubePoint;
		glm::vec3 currentTracingPoint;
		glm::vec3 endCubePoint;
		glm::vec3 tracerDirectionVector;

	protected:
		DynamicEndpointMeta getDynamicEndpointMetaData(ECBPolyPoint in_originPoint, 
													   DynamicBorderLineList* in_blockBorderRef, 
			                                           ECBPolyPoint in_distanceValues, 
			                                           ECBPolyPoint in_slopeDirection, 
			                                           ECBPolyPointTri in_XYZinterceptCoords);
		ECBPolyPoint roundXYZInterceptDistancesToAppropriatePrecision(float in_distX, float in_distY, float in_distZ);
		ECBPolyPoint bindToNearestCorner(int in_xoryorz, ECBPolyPoint in_polyPoint);
		ECBPolyPoint bindToNearestLine(int in_xoryorz, ECBPolyPoint in_polyPoint);
		ECBPolyPoint getTrimmedPoint(ECBPolyPoint in_polyPoint);

		DynamicBorderLineList dynamicBorder;


		ECBPolyPoint normalizedSlopeDirection;

		float rasterCubeDimLength = 0.0f;
		float lineLength = 0.0f;
		float remainingDistance = 0.0f;
		float tileWeightRatio = 0.0f;
		PolyLogger* optionalLoggerRef = nullptr;

		// optional values
		RasterCubeLookup* optionalCubeLookup = nullptr;
		bool optionalMatchingBool = false;


		bool debugFlag = false;
};

#endif
