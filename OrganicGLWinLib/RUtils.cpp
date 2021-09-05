#include "stdafx.h"
#include "RUtils.h"

EnclaveKeyDef::EnclaveKey RUtils::convertToRasterGridCell(glm::vec3 in_pointToConvert, float in_tileGridWidth, int in_numberOfTilesPerDimension)
{
	// find the coordinates; check to ensure that the values of each aren't equal to the end of the dimension
	/*
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
	*/

	
	int x_block_value = in_pointToConvert.x / in_tileGridWidth;
	if
	(
		(x_block_value != 0)
	)
	{
		x_block_value--;
	}

	int y_block_value = in_pointToConvert.y / in_tileGridWidth;
	if
	(
		(y_block_value != 0)
	)
	{
		y_block_value--;
	}

	int z_block_value = in_pointToConvert.z / in_tileGridWidth;
	if
	(
		(z_block_value != 0)
	)
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

EnclaveKeyDef::EnclaveKey RUtils::findRasterGridCellCentroid(glm::vec3 in_point0,
	glm::vec3 in_point1,
	glm::vec3 in_point2,
	float in_tileGridWidth,
	int in_numberOfTilesPerDimension)
{
	glm::vec3 centroidToConvert;
	centroidToConvert.x = (in_point0.x + in_point1.x + in_point2.x) / 3;
	centroidToConvert.y = (in_point0.y + in_point1.y + in_point2.y) / 3;
	centroidToConvert.z = (in_point0.z + in_point1.z + in_point2.z) / 3;
	return convertToRasterGridCell(centroidToConvert, in_tileGridWidth, in_numberOfTilesPerDimension);
}

glm::vec3 RUtils::convertGridCellLocationToVec3(int in_locationX, int in_locationY, int in_locationZ, float in_cubeDimLength)
{
	float pointX = (in_locationX * in_cubeDimLength) + (in_cubeDimLength / 2);
	float pointY = (in_locationY * in_cubeDimLength) + (in_cubeDimLength / 2);
	float pointZ = (in_locationZ * in_cubeDimLength) + (in_cubeDimLength / 2);
	glm::vec3 returnVec(pointX, pointY, pointZ);
	return returnVec;
}

glm::vec3 RUtils::convertToRasterGridPoint(glm::vec3 in_pointToConvert, 
										   float in_tileDimensionalWeight, 
										   int in_numberOfTilesPerDimension,
										   float in_tileDimWeightToHundredthRatio)
{
	EnclaveKeyDef::EnclaveKey pointCellCoordinate = convertToRasterGridCell(in_pointToConvert, in_tileDimensionalWeight, in_numberOfTilesPerDimension);
	float cellOffset = 0.5f;
	float gridPointX = (pointCellCoordinate.x * in_tileDimensionalWeight) + ( in_tileDimensionalWeight/2);
	float gridPointY = (pointCellCoordinate.y * in_tileDimensionalWeight) + ( in_tileDimensionalWeight/2);
	float gridPointZ = (pointCellCoordinate.z * in_tileDimensionalWeight) + ( in_tileDimensionalWeight/2);
	glm::vec3 returnPoint(gridPointX, gridPointY, gridPointZ);
	//returnPoint *= in_tileDimWeightToHundredthRatio;
	//returnPoint /= in_tileDimWeightToHundredthRatio;
	return returnPoint;
}