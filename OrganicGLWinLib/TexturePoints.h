#pragma once

#ifndef TEXTUREPOINTS_H
#define TEXTUREPOINTS_H

#include "TextureUV.h"

class TexturePoints
{
public:
	TextureUV UVpoints[3];
	int U_tile_coord = 0;		// these values will be passed to the shader
	int V_tile_coord = 0;
};

#endif
