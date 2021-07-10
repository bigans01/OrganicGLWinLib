#pragma once

#ifndef UVTRIANGLECOORDS_H
#define UVTRIANGLECOORDS_H

#include "ECBPolyPoint.h"

class UVTriangleCoords
{
public:
	ECBPolyPoint UVpoint[3];
	int U_tile_coord = 0;
	int V_tile_coord = 0;
};

#endif
