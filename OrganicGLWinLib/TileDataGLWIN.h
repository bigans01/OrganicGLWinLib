#pragma once

#include "stdafx.h"

#ifndef TILEDATAGLWIN_H
#define TILEDATAGLWIN_H


class TileDataGLWIN
{
public:
	TriangleMaterial materialID = TriangleMaterial::NOVAL;	// the texture "material" or "block"
	int array_x = 0;	// the x value in the 2d dimensional array, that this will be inserted into
	int array_y = 0;	// "" y value
	std::string texture_file = "";	// the path of the file to be loaded
};

#endif
