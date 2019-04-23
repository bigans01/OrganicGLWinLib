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
	std::cout << ">>>>> Initialization complete..." << std::endl;
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

TexturePoints AtlasMap::getUVPointsForTerrainTile(int in_tileID, TexturePoints in_pointsToConvert)
{
	TexturePoints returnUVcoords = in_pointsToConvert;
	//std::cout << "!!!! Before down scale: " << std::endl;
	//std::cout << "0: " << returnUVcoords.UVpoints[0].U_coord << ", " << returnUVcoords.UVpoints[0].V_coord << std::endl;
	//std::cout << "1: " << returnUVcoords.UVpoints[1].U_coord << ", " << returnUVcoords.UVpoints[1].V_coord << std::endl;
	//std::cout << "2: " << returnUVcoords.UVpoints[2].U_coord << ", " << returnUVcoords.UVpoints[2].V_coord << std::endl;

	returnUVcoords.UVpoints[0] = scaleDownUVcoords(returnUVcoords.UVpoints[0]);
	returnUVcoords.UVpoints[1] = scaleDownUVcoords(returnUVcoords.UVpoints[1]);
	returnUVcoords.UVpoints[2] = scaleDownUVcoords(returnUVcoords.UVpoints[2]);

	//std::cout << "!!!! After down scale: " << std::endl;
	//std::cout << "0: " << returnUVcoords.UVpoints[0].U_coord << ", " << returnUVcoords.UVpoints[0].V_coord << std::endl;
	//std::cout << "1: " << returnUVcoords.UVpoints[1].U_coord << ", " << returnUVcoords.UVpoints[1].V_coord << std::endl;
	//std::cout << "2: " << returnUVcoords.UVpoints[2].U_coord << ", " << returnUVcoords.UVpoints[2].V_coord << std::endl;

	// find the index to look up, and get the offsets
	int x_lookup = tileLookup[in_tileID].x_location;
	int y_lookup = tileLookup[in_tileID].y_location;

	returnUVcoords.U_tile_coord = x_lookup;		// store the tile location values
	returnUVcoords.V_tile_coord = y_lookup;

	int indexToFind = findTileIndex(x_lookup, y_lookup);
	float U_offset = AtlasTileArray[indexToFind].x_float_offset;
	float V_offset = AtlasTileArray[indexToFind].y_float_offset;
	for (int x = 0; x < 3; x++)
	{
		returnUVcoords.UVpoints[x].U_coord += U_offset;			// add the offsets to the converted points
		returnUVcoords.UVpoints[x].V_coord += V_offset;		
	}

	//std::cout << "!!!! After adding offsets: " << std::endl;
	//std::cout << "0: " << returnUVcoords.UVpoints[0].U_coord << ", " << returnUVcoords.UVpoints[0].V_coord << std::endl;
	//std::cout << "1: " << returnUVcoords.UVpoints[1].U_coord << ", " << returnUVcoords.UVpoints[1].V_coord << std::endl;
	//std::cout << "2: " << returnUVcoords.UVpoints[2].U_coord << ", " << returnUVcoords.UVpoints[2].V_coord << std::endl;


	return returnUVcoords;
}

TextureUV AtlasMap::scaleDownUVcoords(TextureUV in_textureUV)
{
	TextureUV returnUV = in_textureUV;
	if (returnUV.U_coord != 0.0f)	// do not divide by 0
	{
		returnUV.U_coord /= tileDimension;	// scale down by the number of tiles in either dimension
	}
	if (returnUV.V_coord != 0.0f)
	{
		returnUV.V_coord /= tileDimension;	// 
	}
	return returnUV;
}