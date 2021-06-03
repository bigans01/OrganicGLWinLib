#include "stdafx.h"
#include "CleaveSequenceResolver.h"

void CleaveSequenceResolver::addSPolyToResolve(SPoly in_sPolyToResolve)
{

}

void CleaveSequenceResolver::addSPolyToCompare(SPoly in_comparingSPoly)
{

}

unsigned short CleaveSequenceResolver::convertBlockCoordsToUShort(int in_x, int in_y, int in_z)
{
	int x = in_x * (numberOfTilesPerDimension*numberOfTilesPerDimension);
	int y = in_y * (numberOfTilesPerDimension);
	return x + y + in_z;
}

EnclaveKeyDef::EnclaveKey CleaveSequenceResolver::convertSPolyPointToBlockCoord(glm::vec3 in_pointToConvert)
{
	// find the coordinates; check to ensure that the values of each aren't equal to the end of the dimension

	int x_block_value = in_pointToConvert.x / tileGridWidth;
	if (x_block_value == numberOfTilesPerDimension)
	{
		x_block_value--;
	}

	int y_block_value = in_pointToConvert.y / tileGridWidth;
	if (y_block_value == numberOfTilesPerDimension)
	{
		y_block_value--;
	}

	int z_block_value = in_pointToConvert.z / tileGridWidth;
	if (z_block_value == numberOfTilesPerDimension)
	{
		z_block_value--;
	}
	
	std::cout << "Original coordinate: " << in_pointToConvert.x << ", " << in_pointToConvert.y << ", " << in_pointToConvert.z << std::endl;
	std::cout << ">> Resolver x coord: " << x_block_value << std::endl;
	std::cout << ">> Resolver y coord: " << y_block_value << std::endl;
	std::cout << ">> Resolver z coord: " << z_block_value << std::endl;

	EnclaveKeyDef::EnclaveKey returnKey(x_block_value, y_block_value, z_block_value);
	return returnKey;
}