#pragma once

#ifndef RTRIANGLELINE_H
#define RTRIANGLELINE_H

#include <unordered_set>
#include "EnclaveKeyDef.h"
#include <glm/glm.hpp>
#include "STriangleLine.h"
#include "RasterCubeLookup.h"
#include "RTriangleLineTracer.h"
#include "MassGridArray.h"

class RTriangleLine
{
	public:
		RTriangleLine() {};
		RTriangleLine(glm::vec3 in_rasterGridPointA, 
					  glm::vec3 in_rasterGridPointB, 
					  EnclaveKeyDef::EnclaveKey in_pointACubeKey,
					  EnclaveKeyDef::EnclaveKey in_pointBCubeKey,
					  float in_rPolyCubeDimLength,
					  STriangleLine in_sTriangleLineToBuildFrom,
			          float in_tileWeightRatio) :
			rLinePointA(in_rasterGridPointA),
			rLinePointB(in_rasterGridPointB),
			rPolyCubeDimLength(in_rPolyCubeDimLength),
			pointACubeKey(in_pointACubeKey),
			pointBCubeKey(in_pointBCubeKey),
			tileWeightRatio(in_tileWeightRatio)
		{
			rLineIsBorderLine = in_sTriangleLineToBuildFrom.isBorderLine;
			rLineBorderLineID = in_sTriangleLineToBuildFrom.borderLineID;
			isPointAOnBorderLine = in_sTriangleLineToBuildFrom.pointAOnBorderLine;
			isPointBOnBorderLine = in_sTriangleLineToBuildFrom.pointBOnBorderLine;
		};

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> findBlocksAtX(int in_x)
		{
			return rasterizedBlocks.fetchXSlice(in_x);
		}

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> findBlocksAtY(int in_y)
		{
			return rasterizedBlocks.fetchYSlice(in_y);
		}

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> findBlocksAtZ(int in_z)
		{
			return rasterizedBlocks.fetchZSlice(in_z);
		}

		glm::vec3 rLinePointA;	// the beginning of the line
		glm::vec3 rLinePointB;	// the end of the line
		EnclaveKeyDef::EnclaveKey pointACubeKey;	// the cubes that the points reside in
		EnclaveKeyDef::EnclaveKey pointBCubeKey;	// ""
		float rPolyCubeDimLength = 0.0f;			// the dimension length for xyz, for a rasterization cube.
		float tileWeightRatio = 0.0f;
		unsigned char rLineIsBorderLine = 0;		// indicates whether or not it is a border line of the STriangleSet this triangle belongs in
		unsigned char rLineBorderLineID = 0;		// indicates the borderLineID, if it is indeed a border line
		unsigned char isPointAOnBorderLine = 0;	// is point A on border line? (checked when borderLineID is 1)
		unsigned char isPointBOnBorderLine = 0;	// is point B on border line? " " " 

		void runRasterTrace()
		{
			RTriangleLineTracer lineTracer;
			if (debugFlag == true)
			{
				std::cout << "::::::::::: HALTING; for debug..." << std::endl;
				int halt = 3;
				std::cin >> halt;
			}

			lineTracer.setOptionalCubeLookupRef(&rasterizedBlocks);
			lineTracer.setData(pointACubeKey, pointBCubeKey, rLinePointA, rLinePointB, rPolyCubeDimLength, tileWeightRatio, debugFlag);
			lineTracer.runTrace();
		};

		void runRasterTraceIntoGrid(MassGridArray* in_massGridArrayRef)
		{

		}

		bool debugFlag = false;
	private:
		RasterCubeLookup rasterizedBlocks;

};

#endif
