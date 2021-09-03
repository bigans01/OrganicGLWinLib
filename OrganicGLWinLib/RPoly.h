#pragma once

#ifndef RPOLY_H
#define RPOLY_H

#include "SPoly.h"
#include "RUtils.h"
#include "RTriangle.h"
#include "RTriangleLine.h"
#include "EnclaveKeyDef.h"
#include <map>
#include "DebugOption.h"
#include "PolyLogger.h"

class RPoly
{
	public:
		RPoly() {};
		RPoly(SPoly in_sPolyToBuildFrom, 
			  int in_rPolyDimTiles, 
			  float in_rPolyDimLimit, 
			  float in_rPolyRCubeDimLength,
			  float in_rPolyTileWeightToHundredthFloatRatio,
			  PolyDebugLevel in_rTriangleDebugLevel) 
		{
			rPolyTilesPerDim = in_rPolyDimTiles;
			rPolyDimLimit = in_rPolyDimLimit;
			rPolyRCubeDimLength = in_rPolyRCubeDimLength;
			rPolyTileWeightToHundredthFloatRatio = in_rPolyTileWeightToHundredthFloatRatio;
			rPolyEmptyNormal = in_sPolyToBuildFrom.polyEmptyNormal;
			rPolyMaterialID = in_sPolyToBuildFrom.sPolyMaterialID;

			// read each STriangle to create an RTriangle
			auto sTrianglesBegin = in_sPolyToBuildFrom.triangles.begin();
			auto sTrianglesEnd = in_sPolyToBuildFrom.triangles.end();
			for (; sTrianglesBegin != sTrianglesEnd; sTrianglesBegin++)
			{
				// build each RTriangleLine from each STriangleLine, then build the RTriangle from those.
				RTriangleLine builtLineArray[3];
				PolyLogger rTriangleLogger;
				rTriangleLogger.setDebugLevel(in_rTriangleDebugLevel);
				for (int x = 0; x < 3; x++)
				{
					STriangleLine* currentLinePtr = &sTrianglesBegin->second.triangleLines[x];

					// get raster cube points
					glm::vec3 currentLinePointA = RUtils::convertToRasterGridPoint(currentLinePtr->pointA, rPolyRCubeDimLength, rPolyTilesPerDim, rPolyTileWeightToHundredthFloatRatio);
					glm::vec3 currentLinePointB = RUtils::convertToRasterGridPoint(currentLinePtr->pointB, rPolyRCubeDimLength, rPolyTilesPerDim, rPolyTileWeightToHundredthFloatRatio);

					// get raster cube cooridnates
					EnclaveKeyDef::EnclaveKey pointARasterCubeCoord = RUtils::convertToRasterGridCell(currentLinePtr->pointA, rPolyRCubeDimLength, rPolyTilesPerDim);
					EnclaveKeyDef::EnclaveKey pointBRasterCubeCoord = RUtils::convertToRasterGridCell(currentLinePtr->pointB, rPolyRCubeDimLength, rPolyTilesPerDim);

					rTriangleLogger.log("(RPoly): rPolyTileWeightToHundredthFloatRatio value: ", rPolyTileWeightToHundredthFloatRatio, "\n");
					rTriangleLogger.log("(RPoly): current line, point A: ", currentLinePtr->pointA.x, ", ", currentLinePtr->pointA.y, ", ", currentLinePtr->pointA.z, "\n");
					rTriangleLogger.log("(RPoly): current line, point B: ", currentLinePtr->pointB.x, ", ", currentLinePtr->pointB.y, ", ", currentLinePtr->pointB.z, "\n");
					rTriangleLogger.log("(RPoly): current line, point A 3d raster grid point: ", currentLinePointA.x, ", ", currentLinePointA.y, ", ", currentLinePointA.z, "\n");
					rTriangleLogger.log("(RPoly): current line, point B 3d raster grid point: ", currentLinePointB.x, ", ", currentLinePointB.y, ", ", currentLinePointB.z, "\n");
					rTriangleLogger.log("(RPoly): current line, point A cube coordinate: ", pointARasterCubeCoord.x, ", ", pointARasterCubeCoord.y, ", ", pointARasterCubeCoord.z, "\n");
					rTriangleLogger.log("(RPoly): current line, point B cube coordinate: ", pointBRasterCubeCoord.x, ", ", pointBRasterCubeCoord.y, ", ", pointBRasterCubeCoord.z, "\n");


					RTriangleLine builtLine(currentLinePointA, 
											currentLinePointB, 
											pointARasterCubeCoord, 
											pointBRasterCubeCoord, 
											rPolyRCubeDimLength, 
											*currentLinePtr, 
											rPolyTileWeightToHundredthFloatRatio,
											rPolyMaterialID);
					builtLineArray[x] = builtLine;
				}
				RTriangle builtRTriangle(builtLineArray[0], builtLineArray[1], builtLineArray[2], in_rTriangleDebugLevel, rPolyMaterialID);

				builtRTriangle.traceRasterLines();
				addRTriangle(builtRTriangle);
			}

			// read each existing SPolyBorderLines, copy their contents, but make their points rasterized.
			auto sPolyBorderLinesBegin = in_sPolyToBuildFrom.borderLines.begin();
			auto sPolyBorderLinesEnd = in_sPolyToBuildFrom.borderLines.end();
			for (; sPolyBorderLinesBegin != sPolyBorderLinesEnd; sPolyBorderLinesBegin++)
			{
				SPolyBorderLines currentRasterizedBorderLine = sPolyBorderLinesBegin->second;
				currentRasterizedBorderLine.pointA = RUtils::convertToRasterGridPoint(currentRasterizedBorderLine.pointA, rPolyRCubeDimLength, rPolyTilesPerDim, rPolyTileWeightToHundredthFloatRatio);
				currentRasterizedBorderLine.pointB = RUtils::convertToRasterGridPoint(currentRasterizedBorderLine.pointB, rPolyRCubeDimLength, rPolyTilesPerDim, rPolyTileWeightToHundredthFloatRatio);
				rBorderLines[sPolyBorderLinesBegin->first] = currentRasterizedBorderLine;
				numberOfRBorderLines++;
			}

			//std::cout << "|||||||| RPoly constructor finished, size of RTriangles: " << rTriangleMap.size() << " | number of SPolyBorderLines: " << rBorderLines.size() << std::endl;
			//std::cout << "|||||||| RPoly RCube dimensional length: " << rPolyRCubeDimLength << std::endl;
		};

		std::map<int, RTriangle> rTriangleMap;
		std::map<int, SPolyBorderLines> rBorderLines;
		void addRTriangle(RTriangle in_rTriangleToAdd);
		void traceTriangleAreaIntoGrid(MassGridArray* in_massGridArrayRef, bool in_debugFlag);
		void printRPoints();

	private:
		int rPolyTilesPerDim = 0;		// number of tiles per dimension in x/y/z
		float rPolyDimLimit = 0.0f;
		float rPolyTileWidth = 0.0f;
		float rPolyRCubeDimLength = 0.0f;
		float rPolyTileWeightToHundredthFloatRatio = 0.0f;
		short rPolyMaterialID = 0;
		short numberOfRTriangles = 0;
		short numberOfRBorderLines = 0;
		glm::vec3 rPolyEmptyNormal;
};

#endif
