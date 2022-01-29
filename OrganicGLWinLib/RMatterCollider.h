#pragma once

#ifndef RMATTERCOLLIDER_H
#define RMATTERCOLLIDER_H

#include "RMatterGenerator.h"
#include "RMatterAge.h"
#include "MassZoneBoxType.h"
#include "RMatterCollisionMode.h"
#include <vector>
#include "SPoly.h"
#include <set>
#include "RMatterManipulationOption.h"
#include "MessageContainer.h"

class RMatterCollider
{
	public:
		RMatterCollider() {};

		template<typename FirstManipulationOption, typename ...RemainingManipulationOptions> void setRMatterManipulationOption(FirstManipulationOption && firstOption,
																														RemainingManipulationOptions && ...remainingOptions)
		{
			matterManipulationOptions.insert(firstOption);
			setRMatterManipulationOption(std::forward<RemainingManipulationOptions>(remainingOptions)...);
		};
		void setRMatterManipulationOption() {};

		void initializeCollider(int in_colliderTilesPerDimension,
								float in_colliderDimensionLimit,
								int in_colliderMeshesPerDimension,
								int in_colliderPointsPerSlicePointArray,
								MassZoneBoxType in_colliderMassZoneBoxType,
								RMatterCollisionMode in_colliderMode);

		// "Smart" generation; will attempt to handle errors to produce a result; it also handles the 
		// execution of Steps 1-5 in its own way, based on if there are matter production errors.
		void produceColliderMatterWithHandling(bool in_ignoreOld, bool in_ignoreNew);


		void addSPolyToMatterGenerator(SPoly in_sPolyToAdd, RMatterAge in_rMatterAge);		// Step 1: run this function for each SPoly that needs to be added to the collider
		void generateAndMoveSPolyResultsToScanners();										// Step 2: once all SPolys have been added, run the SPolyShellProducer in each RMatterGenerator, 
																							// and move the results of that producer to the scanner in each RMatterGenerator.

		void generateMassses();																// Step 3: call generateMassInScanner() in each RMatterGenerator
		void flagOldMatterAsRMatterWrapper();	// signifies that the matter inserted into oldMatterGenerator 
												// was actually produced by a call to OREMatterCollider::extractResultsAndSendToORE; 
												// this means that the production of border SPolys in the oldMatterGenerator's instance of SPolyShellProducer should be ignored
												// (but input polys sent to the SPolyShellProducer should remain)

		// Step 4: append matter to the collidableScanner.
		void appendOldMatterToScanner();					
		void appendNewMatterToScanner();

		// Step 5: Produce the PTriangleMesh that exists in the collidableScanner, by running 
		void generateCollidedMatterResult();


		RMorphableAreaScanner* getOldMatterScannerRef();
		RMorphableAreaScanner* getNewMatterScannerRef();

		RMatterGenerator* getOldGeneratorRef();
		RMatterGenerator* getNewGeneratorRef();

		RMorphableAreaScanner collidableScanner;
		std::vector<SPoly> fetchProducedSPolys();	// used by OrganicCoreLib to get the resulting SPolys produced by the collider
	private:
		void applyRMatterManipulationOptions();	// iterates through each member of matterManipulationOptions, if there are any, 
												// and applies it to the collidableScanner. collidableScanner.scanGridMass() must be called before this is called.

		RMatterCollisionMode colliderMode = RMatterCollisionMode::NOVAL;

		int colliderTilesPerDimension = 0;
		float colliderDimensionLimit = 0.0f;
		int colliderMeshesPerDimension = 0;
		int colliderPointsPerSlicePointArray = 0;
		MassZoneBoxType colliderMassZoneBoxType = MassZoneBoxType::NOVAL;

		RMatterGenerator oldMatterGenerator;
		RMatterGenerator newMatterGenerator;
		std::set<RMatterManipulationOption> matterManipulationOptions;
		

};

#endif
