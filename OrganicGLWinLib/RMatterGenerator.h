#pragma once

#ifndef RMATTERGENERATOR_H
#define RMATTERGENERATOR_H

#include "BorderSPolyProducer.h"
#include "RMorphableAreaScanner.h"
#include "MassZoneBoxType.h"

class RMatterGenerator
{
	public:
		RMatterGenerator() {};
		
		void initializeGenerator(int in_generatorTilesPerDimension,
								 float in_generatorDimensionLimit,
								 int in_generatorMeshesPerDimension,
							     int in_pointsPerSlicePointArray,
								 MassZoneBoxType in_massZoneBoxType);
		void addSPolyToBorderProducer(SPoly in_sPolyForBorder);		// Step 1: note, the SPoly to be added must have its empty normal and materialID set/defined already before inserting.

		void configureInputSPolys();								// Step 2: produce the border lines and primal points for each SPoly.
		void generateBorderSPolys();								// Step 3: call BorderSPolyProducer::producerBorderSPolys in generatorBorderProducer.
		void moveBorderSPolyResultsToScanner();						// Step 4: take all the SPolys from the producer and move them into the generatorAreaScanner.
		void generateMassInScanner();								// Setp 5: Generate the mass in the scanner, but don't generate the PTriangleMesh; 
																	// this would be used when the mass in this instance has to be collided with another generator's mass.
		bool doesGeneratorContainMass();
		void printFetchableSPolys();
		void flagOldMatterAsRMatter();

	private:
		friend class RMatterCollider;

		// the following members are set by call to initializeGenerator
		int generatorTilesPerDimension = 0;
		float generatorDimensionLimit = 0.0f;
		int generatorMeshesPerDimension = 0;
		int generatorPointsPerSlicePointArray = 0;
		MassZoneBoxType generatorMassZoneBoxType = MassZoneBoxType::NOVAL;	
		bool containsMass = false;	// set to true as soon as addSPolyToBorderProducer is called.
		bool isOldMassRMatter = false;

		BorderSPolyProducer generatorBorderProducer;		// a complete shell, resulting from the input SPolys + the produced polys of the border producer, 
															// must be passed to the generatorAreaScanner.
		RMorphableAreaScanner generatorAreaScanner;			// will produce the matter that is referenced by the parent class owning this instance of RMatterGenerator.
};

#endif
