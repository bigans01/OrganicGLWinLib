#include "stdafx.h"
#include "MeshMatterMeta.h"

bool MeshMatterMeta::determineSPolyExistenceVerdict()
{
	// if origin mass is CREATION and compared-to mass is CREATION --OR-- DESTRUCTION, do these
	if
	(
		(originManipulationMode == MassManipulationMode::CREATION)
		&&
		(
			(comparedToManipulationMode == MassManipulationMode::CREATION)
			||
			(comparedToManipulationMode == MassManipulationMode::DESTRUCTION)
		)
	)
	{
		// the origin SPoly is CONSUMED by the compared-to mass (which is CREATION). We must then discard it.)
		if (comparisonResult == MassComparisonResult::CONSUMED)
		{
			shouldSPolyRemain = false;
		}
		else if (comparisonResult == MassComparisonResult::NOT_CONSUMED)
		{
			shouldSPolyRemain = true;
		}
	}

	// if origin mass is DESTRUCTION and compared-to mass is CREATION, do these
	else if
	(
		(originManipulationMode == MassManipulationMode::DESTRUCTION)
		&&
		(comparedToManipulationMode == MassManipulationMode::CREATION)
	)
	{
		if (comparisonResult == MassComparisonResult::CONSUMED)
		{
			shouldSPolyRemain = true;
		}
		else if (comparisonResult == MassComparisonResult::NOT_CONSUMED)
		{
			shouldSPolyRemain = false;
		}
	}

	return shouldSPolyRemain;
}