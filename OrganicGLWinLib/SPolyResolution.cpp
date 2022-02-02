#include "stdafx.h"
#include "SPolyResolution.h"

void SPolyResolution::calculateResolution()
{
	// first, determine the resolver.
	determineResolver();
}

void SPolyResolution::determineResolver()
{
	switch (resolutionOrientation)
	{
		case MassZoneBoxBoundaryOrientation::POS_X:
		{
			selectedResolverPtr.reset(new PosXFaceResolver());
			selectedResolverPtr->initialize(resolutionSPolyRef, sequencesToResolve, resolutionBoxType, resolutionOrientation);
			break;
		}
	}
}


