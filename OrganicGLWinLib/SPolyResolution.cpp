#include "stdafx.h"
#include "SPolyResolution.h"

void SPolyResolution::calculateResolution()
{
	// first, determine the resolver.
	determineResolver();
	selectedResolverPtr->setResolverRecorderRef(resolutionRecorderRef);
	selectedResolverPtr->setResolverBoundaryOrientation(resolutionOrientation);
	selectedResolverPtr->setResolverEmptyNormal(resolutionMainEmptyNormal);
	selectedResolverPtr->runResolutionAlgorithm();	// run the algorithm
	calculationResult = selectedResolverPtr->fetchResolution();	// fetch the resulting resolution
}

void SPolyResolution::determineResolver()
{
	std::cout << "(SPolyResolution): determining resolver..." << std::endl;
	switch (resolutionOrientation)
	{
		case BoundaryOrientation::POS_X:
		{
			selectedResolverPtr.reset(new PosXFaceResolver());
			break;
		}

		case BoundaryOrientation::NEG_X:
		{
			selectedResolverPtr.reset(new NegXFaceResolver());
			break;
		}

		case BoundaryOrientation::POS_Y:
		{
			selectedResolverPtr.reset(new PosYFaceResolver());
			break;
		}

		case BoundaryOrientation::NEG_Y:
		{
			selectedResolverPtr.reset(new NegYFaceResolver());
			break;
		}

		case BoundaryOrientation::POS_Z:
		{
			selectedResolverPtr.reset(new PosZFaceResolver());
			break;
		}

		case BoundaryOrientation::NEG_Z:
		{
			selectedResolverPtr.reset(new NegZFaceResolver());
			break;
		}
	}
	selectedResolverPtr->initialize(resolutionSPolyRef, sequencesToResolve, resolutionBoxType, resolutionOrientation);
	selectedResolverPtr->setupBorderLineRangesAndDimLoc();
	//selectedResolverPtr->debugPrintOneDimLines(); // only needed for debugging, not always needed.
}

SPolySupergroup SPolyResolution::fetchResolution()
{
	return calculationResult;
}
