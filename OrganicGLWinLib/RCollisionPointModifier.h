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

class RCollisionPointModifier
{
	public:
		RCollisionPointModifier() {};
		RCollisionPointModifier(RMorphableMeshCornerArray* in_meshCornerArrayRef,
								RPointToGridTranslator* in_gridTranslatorRef) :
			meshCornerArrayRef(in_meshCornerArrayRef),
			gridTranslatorRef(in_gridTranslatorRef)
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

		std::map<int, RCollisionPoint*> immutablePoints;
		int immutablePointCounter = 0;

		std::map<int, RCollisionPoint*> freePoints;
		int freePointsCounter = 0;

};

#endif
