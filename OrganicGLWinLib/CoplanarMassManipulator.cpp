#include "stdafx.h"
#include "CoplanarMassManipulator.h"

void CoplanarMassManipulator::initialize(SPoly* in_trackedSPolyRef, SPolyRefMap in_sPolyRefMap, QuatRotationPoints* in_coplanarPointsRef)
{
	trackedSPolyRef = in_trackedSPolyRef;
	sPolyRefMap = in_sPolyRefMap;
	coplanarPointsRef = in_coplanarPointsRef;
}