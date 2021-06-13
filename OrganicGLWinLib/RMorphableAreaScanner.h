#pragma once

#ifndef RMORPHABLEAREASCANNER_H
#define RMORPHABLEAREASCANNER_H

#include "RMorphableMeshGroup.h"
#include <map>
#include <unordered_map>
#include "Rasterized3DMassGrid.h"
#include "RMorphableMesh.h"

class RMorphableAreaScanner
{
	public:
		RMorphableAreaScanner() {};

		// setupScanner must be called before adding any SPolys that are converted to RPolys.
		void setupScanner(int in_tilesPerDimension, 
						  float in_dimensionLimit,
			              int in_meshesPerDimension)		
		{
			scannerCellsPerDimension = in_tilesPerDimension;
			scannerDimLimit = in_dimensionLimit;

			massGrid.setGridParameters(in_tilesPerDimension, in_dimensionLimit);	// Step 1: initialization of grid
			meshesPerDimension = in_meshesPerDimension;
			morphableMeshDimension = scannerDimLimit / meshesPerDimension;	// i.e, 32.0f divided by 8 = 4.0f.
		}

		void addSPolyToGrid(SPoly in_sPolyToAdd);		// Step 2: add all SPolys that will be converted to RPolys, to the grid.
		void buildGridMassShell();						// Step 3: build the mass shell; that is, trace the interior mass of each RTriangle into the massGrid
		void fillGridMass();							// Step 4: fill the mass inside the mass shell
		void scanGridMass();							// Step 5: scan the grid mass, to determine all RMorphableMesh's that have at least one MassGridArrayCell within the mass; 
														// if the "at least one" condition is met, save the RMorphableMesh into a list.

	private:
		std::unordered_map<EnclaveKeyDef::EnclaveKey, RMorphableMesh, EnclaveKeyDef::KeyHasher> ungroupedMeshes;	// where all the meshes go initially, before doing the grouping pass.
		std::map<int, RMorphableMeshGroup> meshGroupMap;
		Rasterized3DMassGrid massGrid;
		int scannerCellsPerDimension = 0;
		float scannerDimLimit = 0.0f;

		int meshesPerDimension = 1;		// default value is 1; number of meshes that go in the x/y/z dimension
		float morphableMeshDimension = 0.0f;
};

#endif
