#pragma once

#ifndef RADDITIVESLICEBASE_H
#define RADDITIVESLICEBASE_H

#include "RAdditiveSliceType.h"
#include <mutex>
#include "RCollisionPoint.h"

class RAdditiveSliceBase
{
	public:
		// virtual functions
		virtual void buildPointSets() = 0;

		// non virtual functions
		void initialize(RAdditiveSliceType in_sliceType, int in_numberofTypicalSetPoints);

	private:
		RAdditiveSliceType sliceType = RAdditiveSliceType::NOVAL;
		int numberOfPointsPerTypicalSet = 0;
		bool areSetsInitialized = false;
		std::unique_ptr<RCollisionPoint*> typicalPointSetA;
		std::unique_ptr<RCollisionPoint*> typicalPointSetB;
};

#endif
