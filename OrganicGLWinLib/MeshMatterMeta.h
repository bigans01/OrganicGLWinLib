#pragma once

#ifndef MESHMATTERMETA_H
#define MESHMATTERMETA_H

#include "SPoly.h"
#include "MassManipulationMode.h"
#include "MassComparisonResult.h"

class MeshMatterMeta
{
	public:
		MeshMatterMeta() {};
		MeshMatterMeta(int in_referencedSPolyID, SPoly* in_massSPolyRef, MassManipulationMode in_originMassManipulationMode) :
			referencedSPolyID(in_referencedSPolyID),
			massSPolyRef(in_massSPolyRef),
			originManipulationMode(in_originMassManipulationMode)
		{};

		int referencedSPolyID = 0;			// the ID (index) of the referenced SPoly, as it exists within the superceding SPolySet
		SPoly* massSPolyRef = nullptr;
		MassManipulationMode originManipulationMode = MassManipulationMode::NOVAL;
		MassManipulationMode comparedToManipulationMode = MassManipulationMode::NOVAL;
		MassComparisonResult comparisonResult = MassComparisonResult::NOVAL;
		bool determineSPolyExistenceVerdict();
		bool shouldSPolyRemain = true;		// assume that the SPoly will remain (innocent until proven guilty; guilty being contained entirely with a MassZone's produced shell)
};

#endif
