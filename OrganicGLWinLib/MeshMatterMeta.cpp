#include "stdafx.h"
#include "MeshMatterMeta.h"

bool MeshMatterMeta::determineSPolyExistenceVerdict(MassManipulationMode in_comparedToMassManipulationMode, MassComparisonResult in_massComparisonResult)
{
	// if origin mass is CREATION and compared-to mass is CREATION --OR-- DESTRUCTION, do these
	// 
	// Whenever an SPoly in CREATION mode is CONSUMED by another cliping shell, it is either 
	//
	// CONSUMED by a clipping shell that is set to CREATION 
	// (we shouldn't see it because it is within the clipping shell's mass)
	//   
	//   -OR-
	//
	// CONSUMED by a clippnig shell that is set to DESTRUCTION.
	// (because the compared-to clipping shell is set to DESTRUCTION, it means it annihilated the SPoly -- again, we shouldn't see it).

	bool shouldSPolyRemain = true;
	if
	(
		(originManipulationMode == MassManipulationMode::CREATION)
	)
	{
		// the origin SPoly is CONSUMED by the compared-to mass (which is CREATION). We must then discard it.
		if (in_massComparisonResult == MassComparisonResult::CONSUMED)
		{
			shouldSPolyRemain = false;
		}
		else if (in_massComparisonResult == MassComparisonResult::NOT_CONSUMED)
		{
			shouldSPolyRemain = true;
		}
	}

	// if origin mass is DESTRUCTION and compared-to mass is CREATION, do these
	//
	// Whenever an SPoly in DESTRUCTION mode is being compared against a clipping shell, it should only ever interact with a
	// existent mass (the compared to clipping shell should be CREATION);
	//
	// There is no point in comparing an SPoly in DESTRUCTION mode to a clipping shell that is set to DESTRUCTION, as this does nothing.
	else if
	(
		(originManipulationMode == MassManipulationMode::DESTRUCTION)
		&&
		(in_comparedToMassManipulationMode == MassManipulationMode::CREATION)
	)
	{
		if (in_massComparisonResult == MassComparisonResult::CONSUMED)
		{
			shouldSPolyRemain = true;
		}
		else if (in_massComparisonResult == MassComparisonResult::NOT_CONSUMED)
		{
			shouldSPolyRemain = false;
		}
	}

	return shouldSPolyRemain;
}