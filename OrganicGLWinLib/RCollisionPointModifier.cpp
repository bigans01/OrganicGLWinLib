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
				break;
			};
			case RCollisionPointState::IMMUTABLE: 
			{
				immutablePoints[immutablePointCounter++] = currentPointRef;
				break;
			};
		}
	}

	// test: print point counts
	std::cout << "Number of free points: " << freePointsCounter << std::endl;
	std::cout << "Number of immutable points: " << immutablePointCounter << std::endl;
}