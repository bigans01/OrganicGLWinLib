#pragma once

#include "stdafx.h"
#include "TileDataGLWIN.h"

#ifndef ATLASPROPERTIESGL_H
#define ATLASPROPERTIESGL_H

class AtlasPropertiesGL
{
public:
	std::string atlasName = "";
	std::string atlasBase = "";
	std::vector<TileDataGLWIN> tileList;
};

#endif
