#include "stdafx.h"
#include "SPolyResolution.h"

void SPolyResolution::calculateResolution()
{
	// first, determine the resolver.
	determineResolver();
}

void SPolyResolution::determineResolver()
{
	std::cout << "(SPolyResolution): determining resolver..." << std::endl;
	switch (resolutionOrientation)
	{
		case MassZoneBoxBoundaryOrientation::POS_X:
		{
			selectedResolverPtr.reset(new PosXFaceResolver());
			break;
		}

		case MassZoneBoxBoundaryOrientation::NEG_X:
		{
			selectedResolverPtr.reset(new NegXFaceResolver());
			break;
		}

		case MassZoneBoxBoundaryOrientation::POS_Y:
		{
			selectedResolverPtr.reset(new PosYFaceResolver());
			break;
		}

		case MassZoneBoxBoundaryOrientation::NEG_Y:
		{
			selectedResolverPtr.reset(new NegYFaceResolver());
			break;
		}

		case MassZoneBoxBoundaryOrientation::POS_Z:
		{
			selectedResolverPtr.reset(new PosZFaceResolver());
			break;
		}

		case MassZoneBoxBoundaryOrientation::NEG_Z:
		{
			selectedResolverPtr.reset(new NegZFaceResolver());
			break;
		}
	}
	selectedResolverPtr->initialize(resolutionSPolyRef, sequencesToResolve, resolutionBoxType, resolutionOrientation);
	selectedResolverPtr->setupBorderLineRangesAndDimLoc();
	selectedResolverPtr->debugPrintOneDimLines(); // only needed for debugging, not always needed.
}


