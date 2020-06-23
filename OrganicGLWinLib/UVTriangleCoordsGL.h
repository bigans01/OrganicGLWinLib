#pragma once

#ifndef UVTRIANGLECOORDS_H
#define UVTRIANGLECOORDS_H

#include <glm/glm.hpp>

class UVTriangleCoords
{
public:
	glm::vec3 UVpoint[3];
	int U_tile_coord = 0;
	int V_tile_coord = 0;
};

#endif
