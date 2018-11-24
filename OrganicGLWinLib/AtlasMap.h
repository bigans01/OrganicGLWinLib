#pragma once

#ifndef ATLASMAP_H
#define ATLASMAP_H

#include <memory>
#include "AtlasTile.h"
#include "TileMeta.h"
#include "TileLoadData.h"
#include <map>
#include <iostream>
#include <string>


class AtlasMap
{
	friend class OrganicGLWinUtils;
	public:
		std::unique_ptr<AtlasTile[]> AtlasTileArray;	// will contain all tile information (generated at run time)
		void setupTileArray(int in_atlasPixelsLength, int in_tilePixelsLength);
		void insertTileLookup(int in_tileID, int in_tileX, int in_tileY, std::string in_fileName);
		TileLoadData getTileLoadData(int in_tileID);
	private:
		int tilePixelsLength = 0;	// the nunber of pixels of each tile goes, in either dimension; since we square this value to get the area of a tile, we only need to store it once
		int tileDimension = 0;		// the number of tiles on either axis
		float tileCoordinateLength = 0.0f;	// the amount of a single float (1.0f) that a tile will occupy, in either dimension
		std::map<int, TileMeta> tileLookup;		// map for looking up the tile data

		int findTileIndex(int in_tileX, int in_tileY);
};

#endif