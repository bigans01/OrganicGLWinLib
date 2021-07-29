#pragma once

#ifndef RADDITIVESLICEBASE_H
#define RADDITIVESLICEBASE_H

#include "RAdditiveSliceType.h"
#include <mutex>
#include "RCollisionPoint.h"
#include "MassGridArray.h"
#include "RCollisionPointArray.h"
#include "IndependentUtils.h"

class RAdditiveSliceBase
{
	public:
		// virtual functions
		virtual void buildPointSets() = 0;

		// non virtual functions
		void initialize(RAdditiveSliceType in_sliceType, 
						int in_numberofTypicalSetPoints, 
						MassGridArray* in_gridArrayRef, 
						RCollisionPointArray* in_pointArrayRef, 
						float in_sliceThickness);
		void copySetBRefsFromPreviousSlice(RAdditiveSliceBase* in_otherSliceRef);

	private:
		RAdditiveSliceType sliceType = RAdditiveSliceType::NOVAL;
		int numberOfPointsPerTypicalSet = 0;
		float sliceThickness; // how "thick" a slice is, measured in float.
		bool areSetsInitialized = false;
		std::unique_ptr<RCollisionPoint*[]> typicalPointSetA;
		std::unique_ptr<RCollisionPoint*[]> typicalPointSetB;
		MassGridArray* gridArrayRef = nullptr;
		RCollisionPointArray*  pointArrayRef = nullptr;
};

#endif
