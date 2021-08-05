#include "stdafx.h"
#include "RCollisionPointModifier.h"

void RCollisionPointModifier::runPointScan()
{
	// cycle through each element in the point ref array.
	for (int x = 0; x < 8; x++)
	{
		RCollisionPoint* currentPointRef = meshCornerArrayRef->pointRefArray[x];
		RCollisionPointState currentPointState = currentPointRef->getCollisionPointState();
		switch (currentPointState)
		{
			// only need to handle free or immutable points;
			case RCollisionPointState::FREE: 
			{
				freePoints[freePointsCounter++] = currentPointRef;
				break;	// don't break my code, my achey breaky code, I just can't take it anymore (no really, you need to break, Mike)
			};
			case RCollisionPointState::IMMUTABLE: 
			{
				immutablePoints[immutablePointCounter++] = currentPointRef;
				break;	// don't break my code, my achey breaky code, I just can't take it anymore (no really, you need to break, Mike)
			};
		}
	}

	// test: print point counts
	std::cout << "Number of free points: " << freePointsCounter << std::endl;
	std::cout << "Number of immutable points: " << immutablePointCounter << std::endl;

	// for each free point, find it's nearest immutable point.
	auto freePointItineraryGeneratorBegin = freePoints.begin();
	auto freePointItineraryGeneratorEnd = freePoints.end();
	for (; freePointItineraryGeneratorBegin != freePointItineraryGeneratorEnd; freePointItineraryGeneratorBegin++)
	{
		float currentMin = 1000.0f;
		int currentMinLinkedIndex = 0;
		bool wasMinFound = false;
		auto immutableLinkCandidatesBegin = immutablePoints.begin();
		auto immutableLinkCandidatesEnd = immutablePoints.end();
		for (; immutableLinkCandidatesBegin != immutableLinkCandidatesEnd; immutableLinkCandidatesBegin++)
		{
			//std::cout << "Free point: " << freePointItineraryGeneratorBegin->second->originalValue.x << ", "
			//							<< freePointItineraryGeneratorBegin->second->originalValue.y << ", "
			//							<< freePointItineraryGeneratorBegin->second->originalValue.z << std::endl;
			//
			//std::cout << "Immutable point: " << immutableLinkCandidatesBegin->second->originalValue.x << ", "
			//								<< immutableLinkCandidatesBegin->second->originalValue.y << ", "
			//								<< immutableLinkCandidatesBegin->second->originalValue.z << std::endl;

			float currentDist = glm::distance(freePointItineraryGeneratorBegin->second->currentValue, immutableLinkCandidatesBegin->second->currentValue);
			if (currentDist < currentMin)
			{
				currentMin = currentDist;
				currentMinLinkedIndex = immutableLinkCandidatesBegin->first;
				wasMinFound = true;
			}
		}

		// store the closest link found, if any were found
		if (wasMinFound == true)
		{
			
			CollisionPointItinerary newItinerary(freePointItineraryGeneratorBegin->second, 
												immutablePoints[currentMinLinkedIndex], 
												calculatedCubeDimLength, 
												calculatedTileDimWeightRatio, 
												tilesPerDim);
			collisionPointItineraryMap[collisionPointItineraryMapIndex] = newItinerary;
			collisionPointItineraryMap[collisionPointItineraryMapIndex].runJourney();
			collisionPointItineraryMapIndex++;
			
		}
	}
}