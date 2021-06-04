#include "stdafx.h"
#include "RUtils.h"

EnclaveKeyDef::EnclaveKey RUtils::convertToRasterGridCell(glm::vec3 in_pointToConvert, float in_tileGridWidth, int in_numberOfTilesPerDimension)
{
	// find the coordinates; check to ensure that the values of each aren't equal to the end of the dimension

	int x_block_value = in_pointToConvert.x / in_tileGridWidth;
	if (x_block_value == in_numberOfTilesPerDimension)
	{
		x_block_value--;
	}

	int y_block_value = in_pointToConvert.y / in_tileGridWidth;
	if (y_block_value == in_numberOfTilesPerDimension)
	{
		y_block_value--;
	}

	int z_block_value = in_pointToConvert.z / in_tileGridWidth;
	if (z_block_value == in_numberOfTilesPerDimension)
	{
		z_block_value--;
	}

	//std::cout << "Original coordinate: " << in_pointToConvert.x << ", " << in_pointToConvert.y << ", " << in_pointToConvert.z << std::endl;
	//std::cout << ">> Raster grid x coord: " << x_block_value << std::endl;
	//std::cout << ">> Raster grid y coord: " << y_block_value << std::endl;
	//std::cout << ">> Raster grid z coord: " << z_block_value << std::endl;

	EnclaveKeyDef::EnclaveKey returnKey(x_block_value, y_block_value, z_block_value);
	return returnKey;
}

glm::vec3 RUtils::convertToRasterGridPoint(glm::vec3 in_pointToConvert, 
										   float in_tileDimensionalWeight, 
										   int in_numberOfTilesPerDimension,
										   float in_tileDimWeightToHundredthRatio)
{
	EnclaveKeyDef::EnclaveKey pointCellCoordinate = convertToRasterGridCell(in_pointToConvert, in_tileDimensionalWeight, in_numberOfTilesPerDimension);
	float cellOffset = 0.5f;
	float gridPointX = pointCellCoordinate.x + cellOffset;
	float gridPointY = pointCellCoordinate.y + cellOffset;
	float gridPointZ = pointCellCoordinate.z + cellOffset;
	glm::vec3 returnPoint(gridPointX, gridPointY, gridPointZ);
	returnPoint *= in_tileDimWeightToHundredthRatio;
	return returnPoint;
}