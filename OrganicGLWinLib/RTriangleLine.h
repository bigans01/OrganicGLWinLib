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

		void runRasterTraceIntoGrid(MassGridArray* in_massGridArrayRef, glm::vec3 in_triangleEmptyNormal)
		{
			// run a tracer, just as in runRasterTrace(); but populate each traced block from that line in the array when we are done.
			RTriangleLineTracer lineTracer;
			lineTracer.setOptionalCubeLookupRef(&rasterizedBlocks);
			lineTracer.setData(pointACubeKey, pointBCubeKey, rLinePointA, rLinePointB, rPolyCubeDimLength, tileWeightRatio, debugFlag);
			lineTracer.runTrace();

			// use the values rasterizedBlocks set to populate the massGridArray with the appropriate flags.
			auto gridUpdateBegin = std::chrono::high_resolution_clock::now();
			int gridUpdateCount = 0;
			auto fetchedLookup = rasterizedBlocks.fetchRawXLookup();
			auto fetchedIntBegin = (*fetchedLookup).lookup.begin();
			auto fetchedIntEnd = (*fetchedLookup).lookup.end();
			for (; fetchedIntBegin != fetchedIntEnd; fetchedIntBegin++)
			{
				auto currentFetchedSetBegin = fetchedIntBegin->second.begin();
				auto currentFetchedSetEnd = fetchedIntBegin->second.end();
				for (; currentFetchedSetBegin != currentFetchedSetEnd; currentFetchedSetBegin++)
				{
					EnclaveKeyDef::EnclaveKey currentKey(fetchedIntBegin->first, currentFetchedSetBegin->a, currentFetchedSetBegin->b);
					MassGridSearchResult currentSearchResult = in_massGridArrayRef->searchForCell(currentKey.x, currentKey.y, currentKey.z);
					if (currentSearchResult.wasSearchKeyValid == true)
					{
						currentSearchResult.cellRef->setFlag(MassCellBitFlags::LINE_MASS, 1);

						gridUpdateCount++;
					}
				}
			}

			auto gridUpdateEnd = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> gridUpdateElapsed = gridUpdateEnd - gridUpdateBegin;

			std::cout << "Number of grid updates: " << gridUpdateCount << std::endl;
			std::cout << "Grid update time: " << gridUpdateElapsed.count() << std::endl;

		}

		bool debugFlag = false;
	private:
		RasterCubeLookup rasterizedBlocks;

};

#endif
