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
		MeshMatterMeta(SPoly* in_massSPolyRef, MassManipulationMode in_originMassManipulationMode) :
			massSPolyRef(in_massSPolyRef),
			originManipulationMode(in_originMassManipulationMode)
		{};
		SPoly* massSPolyRef = nullptr;
		MassManipulationMode originManipulationMode = MassManipulationMode::NOVAL;
		MassManipulationMode comparedToManipulationMode = MassManipulationMode::NOVAL;
		MassComparisonResult comparisonResult = MassComparisonResult::NOVAL;
		bool determineSPolyExistenceVerdict();
		bool shouldSPolyRemain = false;		
};

#endif
