#pragma once

#ifndef ATLASMAP_H
#define ATLASMAP_H

#include <memory>
#include "AtlasPropertiesGL.h"
#include "AtlasTile.h"
#include "TileMeta.h"
#include "TileLoadData.h"
#include "TexturePoints.h"
#include <filesystem>
#include <fstream>

#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>

//#include "OrganicGLWinUtils.h"
#include "AtlasMetaData.h"


class AtlasMap
{
	friend class OrganicGLWinUtils;
	public:

		AtlasMap();
		AtlasMap(const AtlasMap& in_atlasMapOrig) {};		// needed for use by SmartIntMap in ShaderMachineBase (4/15/2021)
		AtlasMap& operator=(const AtlasMap& atlasMap_b)
		{
			// copy atlas tile data
			atlasTileArrayLength = atlasMap_b.atlasTileArrayLength;
			if (atlasTileArrayLength > 0)
			{
				AtlasTileArray.reset(new AtlasTile[atlasTileArrayLength]);
				for (int x = 0; x < atlasTileArrayLength; x++)
				{
					AtlasTileArray[x] = atlasMap_b.AtlasTileArray[x];
				}
			}

			tilePixelsLength = atlasMap_b.tilePixelsLength;
			tileDimension = atlasMap_b.tileDimension;
			tileCoordinateLength = atlasMap_b.tileCoordinateLength;
			tileLookup = atlasMap_b.tileLookup;

			someval = atlasMap_b.someval;

			return *this;
		}


		std::unique_ptr<AtlasTile[]> AtlasTileArray;	// will contain all tile information (generated at run time)

		void setupTileArray(int in_atlasPixelsLength, int in_tilePixelsLength);
		void insertTileLookup(int in_tileID, int in_tileX, int in_tileY, std::string in_fileName);
		AtlasPropertiesGL readGLData(std::string in_string);
		void buildAtlas(std::string in_atlasFolderName, GLuint* in_atlasTextureRef, float* in_atlasTileWidth, float* in_atlasWidth);
		void buildAtlasOnTextureUnit(GLenum in_texUnit, std::string in_atlasFolderName, GLuint* in_atlasTextureRef, float* in_atlasTileWidth, float* in_atlasWidth);
		TileLoadData getTileLoadData(int in_tileID);
		TexturePoints getUVPointsForTerrainTile(int in_tileID, TexturePoints in_pointsToConvert);
		//TexturePoints getBorderedUVPoints()
		int someval = 0;
		int atlasTileArrayLength = 0;		// value computed by setupTileArray
	private:

		int tilePixelsLength = 0;	// the nunber of pixels of each tile goes, in either dimension; since we square this value to get the area of a tile, we only need to store it once
		int tileDimension = 0;		// the number of tiles on either axis
		float tileCoordinateLength = 0.0f;	// the amount of a single float (1.0f) that a tile will occupy, in either dimension
		std::map<int, TileMeta> tileLookup;		// map for looking up the tile data

		TextureUV scaleDownUVcoords(TextureUV in_textureUV);
		int findTileIndex(int in_tileX, int in_tileY);
		AtlasMetaData findAtlasMetadata(int in_atlasWidth, int in_tileWidth);
};

#endif