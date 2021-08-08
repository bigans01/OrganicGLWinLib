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
#include "RCollisionPointCaster.h"
#include "MassGridArray.h"

class RCollisionPointModifier
{
	public:
		RCollisionPointModifier() {};
		RCollisionPointModifier(RMorphableMeshCornerArray* in_meshCornerArrayRef,
								RPointToGridTranslator* in_gridTranslatorRef, 
								float in_calculatedCubeDimLength,
								float in_calculatedTileDimWeightRatio, 
								int in_tilesPerDim,
								MassGridArray* in_modifierMassGridArrayRef) :
			meshCornerArrayRef(in_meshCornerArrayRef),
			gridTranslatorRef(in_gridTranslatorRef),
			calculatedCubeDimLength(in_calculatedCubeDimLength),
			calculatedTileDimWeightRatio(in_calculatedTileDimWeightRatio),
			tilesPerDim(in_tilesPerDim),
			modifierMassGridArrayRef(in_modifierMassGridArrayRef)
		{
			runPointScan();	// use the meshCornerArrayRef to determine the point types and put each one into its appropriate map (immutablePoints, freePoints, etc)

			// optional, can comment out when done
			auto movedPointsBegin = movedPointMap.begin();
			auto movedPointsEnd = movedPointMap.end();
			for (; movedPointsBegin != movedPointsEnd; movedPointsBegin++)
			{
				std::cout << "!!! Moved point, original value: " << movedPointsBegin->second->originalValue.x << ", "
					<< movedPointsBegin->second->originalValue.y << ", "
					<< movedPointsBegin->second->originalValue.z << " | current value: "
					<< movedPointsBegin->second->currentValue.x << " , "
					<< movedPointsBegin->second->currentValue.y << " , "
					<< movedPointsBegin->second->currentValue.z << std::endl;
			}
		};
	private:
		RMorphableMeshCornerArray* meshCornerArrayRef = nullptr;
		RPointToGridTranslator* gridTranslatorRef = nullptr;
		MassGridArray* modifierMassGridArrayRef = nullptr;
		std::map<int, RCollisionPoint*> movedPointMap;		// will store any moved points that this modifier produced; it is optional, and being used for temporary
															// debug/observation purposes
		int movedPointMapCurrentIndex = 0;

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
									int in_itineraryTilesPerDim,
									MassGridArray* in_itineraryMassGridArrayRef
									) :
				pointToMove(in_pointToMove),
				destinationPoint(in_destinationPoint),
				itineraryCubeDimLength(in_itineraryCubeDimLength),
				itineraryTileDimWeightRatio(in_itineraryTileDimWeightRatio),
				itineraryTilesPerDim(in_itineraryTilesPerDim),
				itineraryMassGridArrayRef(in_itineraryMassGridArrayRef)
			{};

			void runJourney(std::map<int, RCollisionPoint*>* in_pointMapRef, int* in_pointMapCurrentIndex)
			{
				glm::vec3 rasterCubePointA = RUtils::convertToRasterGridPoint(pointToMove->currentValue, itineraryCubeDimLength, itineraryTilesPerDim, itineraryTileDimWeightRatio);
				glm::vec3 rasterCubePointB = RUtils::convertToRasterGridPoint(destinationPoint->currentValue, itineraryCubeDimLength, itineraryTilesPerDim, itineraryTileDimWeightRatio);

				EnclaveKeyDef::EnclaveKey pointARasterCubeKey = RUtils::convertToRasterGridCell(pointToMove->currentValue, itineraryCubeDimLength, itineraryTilesPerDim);
				EnclaveKeyDef::EnclaveKey pointBRasterCubeKey = RUtils::convertToRasterGridCell(destinationPoint->currentValue, itineraryCubeDimLength, itineraryTilesPerDim);

				std::cout << ">> Journey Point A, rasterCubePointA: " << rasterCubePointA.x << ", " << rasterCubePointA.y << ", " << rasterCubePointA.z
										<< " | pointARasterCubeKey: " << pointARasterCubeKey.x << ", " << pointARasterCubeKey.y << ", " << pointARasterCubeKey.z << std::endl;

				std::cout << ">> Journey Point B, rasterCubePointB: " << rasterCubePointB.x << ", " << rasterCubePointB.y << ", " << rasterCubePointB.z
										<< " | pointARasterCubeKey: " << pointBRasterCubeKey.x << ", " << pointBRasterCubeKey.y << ", " << pointBRasterCubeKey.z << std::endl;

				std::cout << "Cube dim length: " << itineraryCubeDimLength << std::endl;

				RCollisionPointCaster journeyPointCaster;
				journeyPointCaster.setData(pointARasterCubeKey, pointBRasterCubeKey, rasterCubePointA, rasterCubePointB, itineraryCubeDimLength, itineraryTileDimWeightRatio, false);
				journeyPointCaster.setGridArrayRef(itineraryMassGridArrayRef);
				journeyPointCaster.setPointToMoveRef(pointToMove);
				journeyPointCaster.runTrace();
				glm::vec3 convertedPoint = journeyPointCaster.getConvertedTraceEndPoint();
				std::cout << "Converted point is: " << convertedPoint.x << ", " << convertedPoint.y << ", " << convertedPoint.z << std::endl;

				(*in_pointMapRef)[(*in_pointMapCurrentIndex)++] = pointToMove;	// testing only
			};

			RCollisionPoint* pointToMove = nullptr;
			RCollisionPoint* destinationPoint = nullptr;
			MassGridArray* itineraryMassGridArrayRef = nullptr;
			float itineraryCubeDimLength = 0.0f;
			float itineraryTileDimWeightRatio = 0.0f;
			int itineraryTilesPerDim = 0;

		};

		std::map<int, CollisionPointItinerary> collisionPointItineraryMap;
		int collisionPointItineraryMapIndex = 0;

};

#endif
