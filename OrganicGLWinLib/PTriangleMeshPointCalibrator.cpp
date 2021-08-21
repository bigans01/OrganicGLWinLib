#include "stdafx.h"
#include "PTriangleMeshPointCalibrator.h"

void PTriangleMeshPointCalibrator::determineCalibrationGridLimit(float in_dimLimit, float in_cellLength, int in_cellsPerDimension)
{
	int maxLimitIndex = in_cellsPerDimension - 1;
	float cellCenterOffset = in_cellLength / 2;
	maxCalibrationGridLimit = (maxLimitIndex * in_cellLength) + cellCenterOffset;
	minCalibrationGridLimit = in_dimLimit - maxCalibrationGridLimit;
}