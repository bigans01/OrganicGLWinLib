#pragma once

#ifndef ATLASMETADATA_H
#define ATLASMETADATA_H

class AtlasMetaData
{
public:
	int atlasMaxLevel = 0;  // the deepest level the texture atlas goes, all the way down to 1x1
	int atlasWidth = 0;	// the width of the atlas, in pixels
	int tileMaxLevel = 0;
	int tileWidth = 0; // the width of a single tile that will be in the atlas, in pixels
	int mipMapLevelDiff = 0; // the integer difference in the projected mip map levels of the atlas and tile, calculated as the deepest mip map level of the atlas minus the deepest mip map level of the tile
	int dimensionToSquare = 0; // the number of tiles that will go in either x or y axis
};

#endif
