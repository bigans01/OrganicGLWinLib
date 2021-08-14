#include "stdafx.h"
#include "RMatterGenerator.h"

void RMatterGenerator::initializeGenerator(int in_generatorTilesPerDimension,
											float in_generatorDimensionLimit,
											int in_generatorMeshesPerDimension,
											int in_pointsPerSlicePointArray,
											MassZoneBoxType in_massZoneBoxType)
{
	generatorTilesPerDimension = in_generatorTilesPerDimension;
	generatorDimensionLimit = in_generatorDimensionLimit;
	generatorMeshesPerDimension = in_generatorMeshesPerDimension;
	generatorPointsPerSlicePointArray = in_pointsPerSlicePointArray;
	generatorMassZoneBoxType = in_massZoneBoxType;

	// ensure the scanner is set up with the proper parameters.
	generatorAreaScanner.setupScanner(generatorTilesPerDimension,
									generatorDimensionLimit,
									generatorMeshesPerDimension,
									generatorPointsPerSlicePointArray);
}

void RMatterGenerator::addSPolyToBorderProducer(SPoly in_sPolyForBorder)
{
	generatorBorderProducer.addInputSPoly(in_sPolyForBorder);
	containsMass = true;
}

bool RMatterGenerator::doesGeneratorContainMass()
{
	return containsMass;
}

void RMatterGenerator::configureInputSPolys()
{
	generatorBorderProducer.configurePolysWithoutNormalCalcs();
}

void RMatterGenerator::generateBorderSPolys()
{
	generatorBorderProducer.produceBorderSPolys(generatorMassZoneBoxType);
}

void RMatterGenerator::moveBorderSPolyResultsToScanner()
{
	auto fetchedSPolyVector = generatorBorderProducer.fetchAllSPolys();
	auto fetchedSPolysBegin = fetchedSPolyVector.begin();
	auto fetchedSPolysEnd = fetchedSPolyVector.end();
	for (; fetchedSPolysBegin != fetchedSPolysEnd; fetchedSPolysBegin++)
	{
		generatorAreaScanner.addSPolyToGrid(*fetchedSPolysBegin);
	}
}

void RMatterGenerator::generateMassInScanner()
{
	generatorAreaScanner.buildGridMassShell();
	generatorAreaScanner.fillGridMass();
}