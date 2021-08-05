#pragma once

#ifndef RCOLLISIONPOINTMODIFIER_H
#define RCOLLISIONPOINTMODIFIER_H

#include <glm/glm.hpp>
#include "ECBPPOrientations.h"
#include <iostream>
#include "IndependentUtils.h"
#include "RCollisionPointState.h"
#include "RMorphableMeshCornerArray.h"
#include "RCollisionPoint.h"
#include "RPointToGridTranslator.h"
#include <map>
#include "RUtils.h"
#include "EnclaveKeyDef.h"
#include <algorithm>

class RCollisionPointModifier
{
	public:
		RCollisionPointModifier() {};
		RCollisionPointModifier(RMorphableMeshCornerArray* in_meshCornerArrayRef,
								RPointToGridTranslator* in_gridTranslatorRef, 
								float in_calculatedCubeDimLength,
								float in_calculatedTileDimWeightRatio, 
								int in_tilesPerDim) :
			meshCornerArrayRef(in_meshCornerArrayRef),
			gridTranslatorRef(in_gridTranslatorRef),
			calculatedCubeDimLength(in_calculatedCubeDimLength),
			calculatedTileDimWeightRatio(in_calculatedTileDimWeightRatio),
			tilesPerDim(in_tilesPerDim)
		{
			runPointScan();	// use the meshCornerArrayRef to determine the point types and put each one into its appropriate map (immutablePoints, freePoints, etc)
		};
	private:
		RMorphableMeshCornerArray* meshCornerArrayRef = nullptr;
		RPointToGridTranslator* gridTranslatorRef = nullptr;

		// NOTES:
		// --immutable points are "landlocked"; they are also "anchor" points that free points can move to.
		// --once a free point has been modified to the point that it hits mass, it must be flagged as "moved"
		// --moved points discovered during the initial scanning pass are ignored
		void runPointScan();

		float calculatedCubeDimLength = 0.0f;			// needed by class derived from RasterCubeTracerBase (no class for that yet)
		float calculatedTileDimWeightRatio = 0.0f;		// ""
		int tilesPerDim = 0;

		std::map<int, RCollisionPoint*> immutablePoints;
		int immutablePointCounter = 0;

		std::map<int, RCollisionPoint*> freePoints;
		int freePointsCounter = 0;

		// used only by this class, to keep track of what points need to be moved.
		struct CollisionPointItinerary
		{
			CollisionPointItinerary() {};
			CollisionPointItinerary(RCollisionPoint* in_pointToMove, 
									RCollisionPoint* in_destinationPoint, 
									float in_itineraryCubeDimLength, 
									float in_itineraryTileDimWeightRatio,
									int in_itineraryTilesPerDim
									) :
				pointToMove(in_pointToMove),
				destinationPoint(in_destinationPoint),
				itineraryCubeDimLength(in_itineraryCubeDimLength),
				itineraryTileDimWeightRatio(in_itineraryTileDimWeightRatio),
				itineraryTilesPerDim(in_itineraryTilesPerDim)
			{};

			void runJourney()
			{
				glm::vec3 rasterCubePointA = RUtils::convertToRasterGridPoint(pointToMove->currentValue, itineraryCubeDimLength, itineraryTilesPerDim, itineraryTileDimWeightRatio);
				glm::vec3 rasterCubePointB = RUtils::convertToRasterGridPoint(destinationPoint->currentValue, itineraryCubeDimLength, itineraryTilesPerDim, itineraryTileDimWeightRatio);

				EnclaveKeyDef::EnclaveKey pointARasterCubeKey = RUtils::convertToRasterGridCell(pointToMove->currentValue, itineraryCubeDimLength, itineraryTilesPerDim);
				EnclaveKeyDef::EnclaveKey pointBRasterCubeKey = RUtils::convertToRasterGridCell(destinationPoint->currentValue, itineraryCubeDimLength, itineraryTilesPerDim);

				std::cout << ">> Journey Point A, rasterCubePointA: " << rasterCubePointA.x << ", " << rasterCubePointA.y << ", " << rasterCubePointA.z
										<< " | pointARasterCubeKey: " << pointARasterCubeKey.x << ", " << pointARasterCubeKey.y << ", " << pointARasterCubeKey.z << std::endl;

				std::cout << ">> Journey Point B, rasterCubePointB: " << rasterCubePointB.x << ", " << rasterCubePointB.y << ", " << rasterCubePointB.z
										<< " | pointARasterCubeKey: " << pointBRasterCubeKey.x << ", " << pointBRasterCubeKey.y << ", " << pointBRasterCubeKey.z << std::endl;
			};

			RCollisionPoint* pointToMove = nullptr;
			RCollisionPoint* destinationPoint = nullptr;
			float itineraryCubeDimLength = 0.0f;
			float itineraryTileDimWeightRatio = 0.0f;
			int itineraryTilesPerDim = 0;

		};

		std::map<int, CollisionPointItinerary> collisionPointItineraryMap;
		int collisionPointItineraryMapIndex = 0;

};

#endif
