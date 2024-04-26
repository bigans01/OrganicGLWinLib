#include "stdafx.h"
#include "GPUWorldCoordinateProducer.h"

GPUWorldCoordinateProducer::GPUWorldCoordinateProducer(double in_worldX, double in_worldY, double in_worldZ)
{
	
	EnclaveKeyDef::EnclaveKey producedCoordinateBPKey;
	glm::vec3 producedCoordinateLocalized;

	// find the key values
	//producedCoordinateBPKey.x = in_worldX / 32;
	//producedCoordinateBPKey.y = in_worldY / 32;
	//producedCoordinateBPKey.z = in_worldZ / 32;
	producedCoordinateBPKey.x = findTrueCollectionCoord(in_worldX);
	producedCoordinateBPKey.y = findTrueCollectionCoord(in_worldY);
	producedCoordinateBPKey.z = findTrueCollectionCoord(in_worldZ);

	// find the localized coordinate values. We must use different logic for remainders that are negative.
	// This is because the localized coordinate within a blueprint can only range between 0 and 32.0f;
	// Therefore, for the modulo that remains below, if the modulo is negative, we must add 32.0f to get its 
	// "true" localized point.
	float x_remainder = float(fmod(in_worldX, 32));
	float y_remainder = float(fmod(in_worldY, 32));
	float z_remainder = float(fmod(in_worldZ, 32));

	if (x_remainder < 0.0f)
	{
		x_remainder = x_remainder + 32.0f;
	}

	if (y_remainder < 0.0f)
	{
		y_remainder = y_remainder + 32.0f;
	}

	if (z_remainder < 0.0f)
	{
		z_remainder = z_remainder + 32.0f;
	}


	glm::vec3 localizedCoordinate(x_remainder, y_remainder, z_remainder);
	

	GPUWorldCoordinate newCoordinate(producedCoordinateBPKey, localizedCoordinate);
	producedCoordinate = newCoordinate;
}

int GPUWorldCoordinateProducer::findTrueCollectionCoord(double in_worldCoord)
{
	// Based off of OrganicUtils::getPreciseCoordinate, for the blueprint/collection portion.
	float divide = float(in_worldCoord / 32.0f);

	int returnDim = 0;

	// for a dim that is less than 0
	if (in_worldCoord < 0)
	{
		returnDim = 0;
		if ((divide < 0) && in_worldCoord > -32.0f)
		{
			if (fmod(in_worldCoord, 32) != 0)
			{
				returnDim = -1;
			}
			else
			{
				returnDim = int(divide);
			}
		}

		else if ((divide < 0) && in_worldCoord < -32.0f)
		{
			if (fmod(in_worldCoord, 32) != 0)
			{
				returnDim = int(divide) - 1;
			}
			else
			{
				returnDim = int(divide);
			}
		}
	}

	// for a dim that is greater than 0
	if (in_worldCoord > 0)
	{
		returnDim = 0;
		if ((divide > 0) && in_worldCoord > 32.0f)
		{
			if (fmod(in_worldCoord, 32.0f) != 0)
			{
				returnDim = int(divide);
			}
			else
			{
				returnDim = int(divide);
			}
		}
	}

	return returnDim;
}