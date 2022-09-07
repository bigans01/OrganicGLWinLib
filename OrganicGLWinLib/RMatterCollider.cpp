#include "stdafx.h"
#include "RMatterCollider.h"

void RMatterCollider::initializeCollider(int in_colliderTilesPerDimension,
	float in_colliderDimensionLimit,
	int in_colliderMeshesPerDimension,
	int in_colliderPointsPerSlicePointArray,
	MassZoneBoxType in_colliderMassZoneBoxType,
	RMatterCollisionMode in_colliderMode)
{
	colliderTilesPerDimension = in_colliderTilesPerDimension;
	colliderDimensionLimit = in_colliderDimensionLimit;			// should always be 1.0f, 4.0f, or 32.0f.
	colliderMeshesPerDimension = in_colliderMeshesPerDimension;
	colliderPointsPerSlicePointArray = in_colliderPointsPerSlicePointArray;
	colliderMassZoneBoxType = in_colliderMassZoneBoxType;
	colliderMode = in_colliderMode;

	// initialize both RMatterGenerators, once collider values are initialized
	oldMatterGenerator.initializeGenerator(colliderTilesPerDimension,
										colliderDimensionLimit,
										colliderMeshesPerDimension,
										colliderPointsPerSlicePointArray,
										colliderMassZoneBoxType);
	newMatterGenerator.initializeGenerator(colliderTilesPerDimension,
										colliderDimensionLimit,
										colliderMeshesPerDimension,
										colliderPointsPerSlicePointArray,
										colliderMassZoneBoxType);
	collidableScanner.setupScanner(colliderTilesPerDimension,
		colliderDimensionLimit,
		colliderMeshesPerDimension,
		colliderPointsPerSlicePointArray,
		in_colliderMassZoneBoxType);
}

void RMatterCollider::addSPolyToMatterGenerator(SPoly in_sPolyToAdd, RMatterAge in_rMatterAge)
{
	switch (in_rMatterAge)
	{
		case RMatterAge::OLD: { oldMatterGenerator.addSPolyToBorderProducer(in_sPolyToAdd); break; };
		case RMatterAge::NEW: { newMatterGenerator.addSPolyToBorderProducer(in_sPolyToAdd); break; };
	}
}

void RMatterCollider::flagOldMatterAsRMatterWrapper()
{
	oldMatterGenerator.flagOldMatterAsRMatter();
}

void RMatterCollider::produceColliderMatterWithHandling(bool in_ignoreOld, bool in_ignoreNew)
{
	bool wasGenerationValid = true;
	MessageContainer oldMatterErrors;
	MessageContainer newMatterErrors;

	if (oldMatterGenerator.containsMass == true)
	{
		if (oldMatterGenerator.isOldMassRMatter == false)
		{
			oldMatterErrors = oldMatterGenerator.generateRMatterShell();
		}
		oldMatterGenerator.moveBorderSPolyResultsToScanner();
	}

	if (newMatterGenerator.containsMass == true)
	{
		newMatterErrors = newMatterGenerator.generateRMatterShell();
		newMatterGenerator.moveBorderSPolyResultsToScanner();
	}

	// if either old or new errors aren't empty, then a mass shell construction failed.
	if
		(
			!(oldMatterErrors.empty())
			||
			!(newMatterErrors.empty())
			)
	{
		wasGenerationValid = false;
		std::cout << "(RMatterCollider): Notice, generation not found as valid. " << std::endl;
	}

	generateMassses();
	if (in_ignoreOld == false)
	{
		appendOldMatterToScanner();
	}
	if (in_ignoreNew == false)
	{
		appendNewMatterToScanner();
	}

	generateCollidedMatterResult();
}

void RMatterCollider::generateAndMoveSPolyResultsToScanners()
{
	if (oldMatterGenerator.containsMass == true)
	{
		if (oldMatterGenerator.isOldMassRMatter == false)
		{
			oldMatterGenerator.generateRMatterShell();
		}
		oldMatterGenerator.moveBorderSPolyResultsToScanner();
	}

	if (newMatterGenerator.containsMass == true)
	{
		newMatterGenerator.generateRMatterShell();
		newMatterGenerator.moveBorderSPolyResultsToScanner();
	}
}

void RMatterCollider::generateMassses()
{
	if (oldMatterGenerator.containsMass == true)
	{
		oldMatterGenerator.generateMassInScanner();
	}
	if (newMatterGenerator.containsMass == true)
	{
		newMatterGenerator.generateMassInScanner();
	}
}

void RMatterCollider::appendOldMatterToScanner()
{
	if (oldMatterGenerator.containsMass == true)
	{
		//std::cout << "~~~~ Beginning append of old matter to scanner..." << std::endl;
		collidableScanner.massGrid.appendMatterFromOtherArray(oldMatterGenerator.generatorAreaScanner.fetchMassGridArrayRef());
	}
}

void RMatterCollider::appendNewMatterToScanner()
{
	if (newMatterGenerator.containsMass == true)
	{
		if (colliderMode == RMatterCollisionMode::MATTER)
		{
			//std::cout << "~~~~ Beginning append of new matter to scanner..." << std::endl;
			collidableScanner.massGrid.appendMatterFromOtherArray(newMatterGenerator.generatorAreaScanner.fetchMassGridArrayRef());
		}
		else if (colliderMode == RMatterCollisionMode::ANTIMATTER)
		{
			//std::cout << "~~~~ Beginning append of new antimatter to scanner..." << std::endl;
			collidableScanner.massGrid.appendAntiMatterFromOtherArray(newMatterGenerator.generatorAreaScanner.fetchMassGridArrayRef());
		}
	}
}

void RMatterCollider::generateCollidedMatterResult()
{
	collidableScanner.scanGridMass();					// generate each RMorphableMeshGroup
	applyRMatterManipulationOptions();					// apply options to each RMorphableMeshGroup, if any exist

	// the folloing block is test output; remove when needed.
	int totalTriangleCount = 0;
	auto currentMeshBegin = collidableScanner.meshGroupMap.begin();
	auto currentMeshEnd = collidableScanner.meshGroupMap.end();
	for (; currentMeshBegin != currentMeshEnd; currentMeshBegin++)
	{
		totalTriangleCount += currentMeshBegin->second.groupMesh.getMeshPTriangleCount();
	}
	//std::cout << "+++++++ collidableScanner, totalTriangleCount: " << totalTriangleCount << std::endl;
	//std::cout << "+++++++ Number of populated grid cells: " << collidableScanner.fetchMassGridArrayRef()->getNumberOfPopulatedCells() << std::endl;
}

RMorphableAreaScanner* RMatterCollider::getOldMatterScannerRef()
{
	return &oldMatterGenerator.generatorAreaScanner;
}

RMorphableAreaScanner* RMatterCollider::getNewMatterScannerRef()
{
	return &newMatterGenerator.generatorAreaScanner;
}

RMatterGenerator* RMatterCollider::getOldGeneratorRef()
{
	return &oldMatterGenerator;
}

RMatterGenerator* RMatterCollider::getNewGeneratorRef()
{
	return &newMatterGenerator;
}

std::vector<SPoly> RMatterCollider::fetchProducedSPolys()
{
	return collidableScanner.produceSPolysFromPTriangleMeshes();
}

Operable3DEnclaveKeySet RMatterCollider::fetchProducedWholeBlocks()
{
	return collidableScanner.wholeBlocks;
}

void RMatterCollider::applyRMatterManipulationOptions()
{
	auto optionsBegin = matterManipulationOptions.begin();
	auto optionsEnd = matterManipulationOptions.end();
	for (; optionsBegin != optionsEnd; optionsBegin++)
	{
		switch (*optionsBegin)
		{
			case RMatterManipulationOption::CLAMP_NONFREE_GRID_POINTS_TO_NATURAL_LIMITS:	
			{
				// Because we are attempting to clamp to natural limits, we can also set the BoundaryIndicator in each PTriangle.
				// So, for each RMorphableMeshGroup in the collidableScanner, we will need to call a function on each RMorphableMeshGroup instance,
				// that will attempt to set boundaries on each PTriangle in the underlying PTriangleMesh (the groupMesh member in RMorphableMeshGroup).
				// We will let the call to the function below handle this for us.
				collidableScanner.clampNonFreeMeshPointsToNaturalLimits();
				break;
			};
		}
	}
}