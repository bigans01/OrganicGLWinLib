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

void RMatterGenerator::handleGeneratorDebugOption(DebugOption in_firstOption)
{
	switch (in_firstOption)
	{
		case DebugOption::BORDERSPOLYPRODUCER_SHOW_EXTRACTABLE_SHELL_OUTPUT:
		{
			generatorBorderProducer.setBorderDebugOption(in_firstOption);
			break;
		}
		case DebugOption::BORDERSPOLYPRODUCER_PRINT_OUTPUT_SPOLYS:
		{
			generatorBorderProducer.setBorderDebugOption(in_firstOption);
			break;
		}
		case DebugOption::BORDERSPOLYPRODUCER_PRINT_MASSZONEBOX_TOUCHED_FACES:
		{
			generatorBorderProducer.setBorderDebugOption(in_firstOption);
			break;
		}
		case DebugOption::RMATTERGENERATOR_PRINT_MASS_SHELL_BUILD_TIMES:
		{
			generatorAreaScanner.setScannerDOGeneric(in_firstOption);
			break;
		}
	}
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

void RMatterGenerator::printFetchableSPolys()
{
	auto fetchedSPolyVector = generatorBorderProducer.fetchAllSPolys();
	auto fetchedSPolysBegin = fetchedSPolyVector.begin();
	auto fetchedSPolysEnd = fetchedSPolyVector.end();
	int currentSPoly = 0; 
	for (; fetchedSPolysBegin != fetchedSPolysEnd; fetchedSPolysBegin++)
	{
		std::cout << ":: SPoly " << currentSPoly++ << std::endl;
		fetchedSPolysBegin->printPoints();
		glm::vec3 currentSPolyNormal = fetchedSPolysBegin->getEmptyNormal();
		std::cout << ":: current SPoly empty normal: " << currentSPolyNormal.x << ", " << currentSPolyNormal.y << ", " << currentSPolyNormal.z << std::endl;
	}
}

void RMatterGenerator::generateMassInScanner()
{
	generatorAreaScanner.buildGridMassShell();
	generatorAreaScanner.fillGridMass();
}

void RMatterGenerator::flagOldMatterAsRMatter()
{
	isOldMassRMatter = true;
}