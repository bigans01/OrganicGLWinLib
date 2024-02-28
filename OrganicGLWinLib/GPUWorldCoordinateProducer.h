#pragma once

#ifndef GPUWORLDCOORDINATEPRODUCER_H
#define GPUWORLDCOORDINATEPRODUCER_H

#include "GPUWorldCoordinate.h"
#include <math.h>

/*

Description:

This class parses 3d coordinates, and produces a corresponding GPUWorldCoordinate, with the correct values.

*/

class GPUWorldCoordinateProducer
{
	public:
		GPUWorldCoordinateProducer(double in_worldX, double in_worldY, double in_worldZ);
		GPUWorldCoordinate producedCoordinate;
	private:
};

#endif
