#include "stdafx.h"
#include "AtlasMap.h"

void AtlasMap::setupTileArray(int in_atlasPixelsLength, int in_tilePixelsLength)
{
	std::cout << ">>>>>>>>> Initializing tile array...." << std::endl;
	tileDimension = in_atlasPixelsLength / in_tilePixelsLength;		// set the tile dimension
	tilePixelsLength = in_tilePixelsLength;							// set tile pixel length
	tileCoordinateLength = 1.0f / tileDimension;					// get the tile dimension
	AtlasTileArray.reset(new AtlasTile[tileDimension*tileDimension]);		// set up the tiles
	for (int x = 0; x < tileDimension; x++)
	{
		for (int y = 0; y < tileDimension; y++)
		{
			int indexToUse = findTileIndex(x, y);
			AtlasTile* tileRef = &AtlasTileArray[indexToUse];
			tileRef->x_float_offset = (x * tileCoordinateLength);
			tileRef->y_float_offset = (y * tileCoordinateLength);
			tileRef->x_pixel_offset = (x * tilePixelsLength);
			tileRef->y_pixel_offset = (y * tilePixelsLength);
			//std::cout << "Tile (" << x << ", " << y << "): " << " | X float: " << tileRef->x_float_offset << " | Y float: " << tileRef->y_float_offset << " | X pixel: " << tileRef->x_pixel_offset << " | Y pixel: " << tileRef->y_pixel_offset << std::endl;
		}
	}
}

int AtlasMap::findTileIndex(int in_tileX, int in_tileY)
{
	int y_val = in_tileY * tileDimension;
	int x_val = in_tileX;
	return y_val + x_val;
}

void AtlasMap::insertTileLookup(int in_tileID, int in_tileX, int in_tileY, std::string in_fileName)
{
	TileMeta metaToInsert;
	metaToInsert.x_location = in_tileX;
	metaToInsert.y_location = in_tileY;
	metaToInsert.file_location = in_fileName;
	tileLookup[in_tileID] = metaToInsert;
}

TileLoadData AtlasMap::getTileLoadData(int in_tileID)
{
	TileLoadData returnMeta;
	int x_lookup = tileLookup[in_tileID].x_location;
	int y_lookup = tileLookup[in_tileID].y_location;
	int indexToFind = findTileIndex(x_lookup, y_lookup);
	returnMeta.x_pixel_begin = AtlasTileArray[indexToFind].x_pixel_offset;
	returnMeta.y_pixel_begin = AtlasTileArray[indexToFind].y_pixel_offset;
	returnMeta.filename = tileLookup[in_tileID].file_location;
	return returnMeta;
}