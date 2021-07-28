#include "stdafx.h"
#include "RAdditiveSliceBase.h"

void RAdditiveSliceBase::initialize(RAdditiveSliceType in_sliceType, int in_numberofTypicalSetPoints)
{
	sliceType = in_sliceType;
	numberOfPointsPerTypicalSet = in_numberofTypicalSetPoints;
	areSetsInitialized = true;
	typicalPointSetA.reset(new RCollisionPoint*[in_numberofTypicalSetPoints]);
	typicalPointSetB.reset(new RCollisionPoint*[in_numberofTypicalSetPoints]);
}