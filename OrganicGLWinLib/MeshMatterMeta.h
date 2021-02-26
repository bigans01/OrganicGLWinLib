#pragma once

#ifndef MESHMATTERMETA_H
#define MESHMATTERMETA_H

#include "SPoly.h"
#include "MassManipulationMode.h"
#include "MassComparisonResult.h"

class MeshMatterMeta
{
	public:
		MeshMatterMeta() {};	// required for usage by std::map (tuple?)
	private:
		MeshMatterMeta(int in_referencedSPolyID, SPoly* in_massSPolyRef, MassManipulationMode in_originMassManipulationMode) :
			referencedSPolyID(in_referencedSPolyID),
			massSPolyRef(in_massSPolyRef),
			originManipulationMode(in_originMassManipulationMode)
		{};
		friend class MassZone;
		friend class MassZonePointClipper;
		int referencedSPolyID = 0;			// the ID (index) of the referenced SPoly, as it exists within the superceding SPolySet
		SPoly* massSPolyRef = nullptr;
		MassManipulationMode originManipulationMode = MassManipulationMode::NOVAL;

		// determines whether or not the SPoly referenced by this instance should exist, or be annihilated.
		bool determineSPolyExistenceVerdict(MassManipulationMode in_comparedToMassManipulationMode, MassComparisonResult in_massComparisonResult); 
};

#endif
