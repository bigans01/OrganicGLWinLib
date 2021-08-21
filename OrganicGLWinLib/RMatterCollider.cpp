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
	colliderDimensionLimit = in_colliderDimensionLimit;
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
		colliderPointsPerSlicePointArray);
}

void RMatterCollider::addSPolyToMatterGenerator(SPoly in_sPolyToAdd, RMatterAge in_rMatterAge)
{
	switch (in_rMatterAge)
	{
		case RMatterAge::OLD: { oldMatterGenerator.addSPolyToBorderProducer(in_sPolyToAdd); break; };
		case RMatterAge::NEW: { newMatterGenerator.addSPolyToBorderProducer(in_sPolyToAdd); break; };
	}
}

void RMatterCollider::generateAndMoveSPolyResultsToScanners()
{
	if (oldMatterGenerator.containsMass == true)
	{
		oldMatterGenerator.generateBorderSPolys();
		oldMatterGenerator.moveBorderSPolyResultsToScanner();
	}

	if (newMatterGenerator.containsMass == true)
	{
		newMatterGenerator.generateBorderSPolys();
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
		std::cout << "~~~~ Beginning append of old matter to scanner..." << std::endl;
		collidableScanner.massGrid.appendMatterFromOtherArray(oldMatterGenerator.generatorAreaScanner.fetchMassGridArrayRef());
	}
}

void RMatterCollider::appendNewMatterToScanner()
{
	if (newMatterGenerator.containsMass == true)
	{
		if (colliderMode == RMatterCollisionMode::MATTER)
		{
			std::cout << "~~~~ Beginning append of new matter to scanner..." << std::endl;
			collidableScanner.massGrid.appendMatterFromOtherArray(newMatterGenerator.generatorAreaScanner.fetchMassGridArrayRef());
		}
		else if (colliderMode == RMatterCollisionMode::ANTIMATTER)
		{
			std::cout << "~~~~ Beginning append of new antimatter to scanner..." << std::endl;
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
	std::cout << "+++++++ collidableScanner, totalTriangleCount: " << totalTriangleCount << std::endl;
	std::cout << "+++++++ Number of populated grid cells: " << collidableScanner.fetchMassGridArrayRef()->getNumberOfPopulatedCells() << std::endl;
}

RMorphableAreaScanner* RMatterCollider::getOldMatterScannerRef()
{
	return &oldMatterGenerator.generatorAreaScanner;
}

RMorphableAreaScanner* RMatterCollider::getNewMatterScannerRef()
{
	return &newMatterGenerator.generatorAreaScanner;
}

std::vector<SPoly> RMatterCollider::fetchProducedSPolys()
{
	return collidableScanner.produceSPolysFromPTriangleMeshes();
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
				collidableScanner.clampNonFreeMeshPointsToNaturalLimits();
				break;
			};
		}
	}
}