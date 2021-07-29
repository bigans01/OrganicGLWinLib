#include "stdafx.h"
#include "RAdditiveSliceBase.h"

void RAdditiveSliceBase::initialize(RAdditiveSliceType in_sliceType, 
									int in_numberofTypicalSetPoints, 
									MassGridArray* in_gridArrayRef, 
									RCollisionPointArray* in_pointArrayRef,
									float in_sliceThickness)
{
	sliceType = in_sliceType;
	numberOfPointsPerTypicalSet = in_numberofTypicalSetPoints;
	areSetsInitialized = true;
	typicalPointSetA.reset(new RCollisionPoint*[in_numberofTypicalSetPoints]);
	typicalPointSetB.reset(new RCollisionPoint*[in_numberofTypicalSetPoints]);
	gridArrayRef = in_gridArrayRef;
	pointArrayRef = in_pointArrayRef;
	sliceThickness = in_sliceThickness;
}

void RAdditiveSliceBase::copySetBRefsFromPreviousSlice(RAdditiveSliceBase* in_otherSliceRef)
{
	// copy, only if this instance and the other instance have their sets initialized
	if
	(
		(areSetsInitialized == true)
		&&
		(in_otherSliceRef->areSetsInitialized == true)
	)
	{
		for (int x = 0; x < numberOfPointsPerTypicalSet; x++)
		{
			// copy B into A
			typicalPointSetA[x] = in_otherSliceRef->typicalPointSetB[x];
		}
	}
}