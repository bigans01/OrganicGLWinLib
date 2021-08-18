#pragma once

#ifndef RMATTERCOLLIDER_H
#define RMATTERCOLLIDER_H

#include "RMatterGenerator.h"
#include "RMatterAge.h"
#include "MassZoneBoxType.h"
#include "RMatterCollisionMode.h"
#include <vector>
#include "SPoly.h"

class RMatterCollider
{
	public:
		RMatterCollider() {};
		void initializeCollider(int in_colliderTilesPerDimension,
								float in_colliderDimensionLimit,
								int in_colliderMeshesPerDimension,
								int in_colliderPointsPerSlicePointArray,
								MassZoneBoxType in_colliderMassZoneBoxType,
								RMatterCollisionMode in_colliderMode);
		void addSPolyToMatterGenerator(SPoly in_sPolyToAdd, RMatterAge in_rMatterAge);		// Step 1: run this function for each SPoly that needs to be added to the collider
		void generateAndMoveSPolyResultsToScanners();										// Step 2: once all SPolys have been added, run the BorderSPolyProducer in each RMatterGenerator, 
																							// and move the results of that producer to the scanner in each RMatterGenerator.

		void generateMassses();																// Step 3: call generateMassInScanner() in each RMatterGenerator

		// Step 4: append matter to the collidableScanner.
		void appendOldMatterToScanner();					
		void appendNewMatterToScanner();

		// Step 5: Produce the PTriangleMesh that exists in the collidableScanner, by running 
		void generateCollidedMatterResult();

		RMorphableAreaScanner* getOldMatterScannerRef();
		RMorphableAreaScanner* getNewMatterScannerRef();

		RMorphableAreaScanner collidableScanner;
		std::vector<SPoly> fetchProducedSPolys();
	private:
		RMatterCollisionMode colliderMode = RMatterCollisionMode::NOVAL;

		int colliderTilesPerDimension = 0;
		float colliderDimensionLimit = 0.0f;
		int colliderMeshesPerDimension = 0;
		int colliderPointsPerSlicePointArray = 0;
		MassZoneBoxType colliderMassZoneBoxType = MassZoneBoxType::NOVAL;

		RMatterGenerator oldMatterGenerator;
		RMatterGenerator newMatterGenerator;
		

};

#endif
