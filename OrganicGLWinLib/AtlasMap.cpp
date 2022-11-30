#include "stdafx.h"
#include "AtlasMap.h"
#include "stb_image.h"

AtlasMap::AtlasMap()
{

}

AtlasPropertiesGL AtlasMap::readGLData(std::string in_atlasName)
{
	AtlasPropertiesGL properties;
	properties.atlasName = in_atlasName;		// set the atlasName

	// create the input stream for reading
	std::ifstream inputStream;
	std::string atlas_properties = "graphics/textures/atlas/" + in_atlasName + "/properties.txt";
	inputStream.open(atlas_properties);
	char lineArray[1024];			 // current line char buffer
	char base_atlas[1024];			 // char array representing the name of the base atlas texture filename
	char current_tile_texture[1024]; // char array representing the name of the current tile's texture filename

	// read the line, set the atlasBase
	inputStream.getline(lineArray, 1024);
	sscanf(lineArray, "Base Texture: %s", &base_atlas);
	properties.atlasBase = std::string(base_atlas);
	std::cout << "!!! +++ Atlas name is: " << properties.atlasName << std::endl;
	std::cout << "!!! +++ Base texture is: " << properties.atlasBase << std::endl;
	while (!inputStream.eof())	// read until end of file
	{
		TileDataGLWIN currentTileData;
		inputStream.getline(lineArray, 1024);
		sscanf(lineArray, "%d: %d, %d, %s", &currentTileData.materialID, &currentTileData.array_x, &currentTileData.array_y, &current_tile_texture);	// read line data
		currentTileData.texture_file = std::string(current_tile_texture);
		std::cout << "!!! +++ Tile information: " << currentTileData.materialID << ", " << currentTileData.array_x << ", " << currentTileData.array_y << ", " << currentTileData.texture_file << std::endl;
		properties.tileList.push_back(currentTileData);		// insert the tile into the properties
	}
	inputStream.close();
	return properties;
}

void AtlasMap::buildAtlas(std::string in_atlasFolderName, GLuint* in_atlasTextureRef, float* in_atlasTileWidth, float* in_atlasWidth)
{
	AtlasPropertiesGL properties = readGLData(in_atlasFolderName);

	
	// gather information
	std::cout << "Prior to smart pointer get: " << std::endl;
	std::string atlasFolder = "graphics/textures/atlas/" + properties.atlasName + "/";
	std::cout << "~~~ ++!!!!+--> Atlas folder is: " << atlasFolder << std::endl;
	std::string baseAtlasTexture = "graphics/textures/atlas/" + properties.atlasName + "/" + properties.atlasBase + ".jpg";
	std::cout << "~~~ ++!!!!+--> Base atlas texture is: " << baseAtlasTexture << std::endl;

	std::vector<TileDataGLWIN>::iterator tileBegin = properties.tileList.begin();
	std::vector<TileDataGLWIN>::iterator tileEnd = properties.tileList.end();
	std::string firstTileTexture = "graphics/textures/atlas/" + properties.atlasName + "/" + tileBegin->texture_file + ".jpg";
	std::cout << "~~~ Initial tile texture is: " << firstTileTexture << std::endl;

	// set flip
	stbi_set_flip_vertically_on_load(true);

	//  get data on the atlas texture
	std::string atlasTextureName = baseAtlasTexture;
	int atlas_width, atlas_height, atlas_nrChannels;
	unsigned char* atlas_data = stbi_load(atlasTextureName.c_str(), &atlas_width, &atlas_height, &atlas_nrChannels, 0);

	// get data on the tile texture
	std::string tileTextureName = firstTileTexture;
	int tile_width, tile_height, tile_nrChannels;
	unsigned char* tile_data = stbi_load(tileTextureName.c_str(), &tile_width, &tile_height, &tile_nrChannels, 0);

	std::cout << "!!!! >>>> atlas_width: " << int(atlas_width) << std::endl;
	std::cout << "!!!! >>>> tile_width: " << int(tile_width) << std::endl;


	*in_atlasWidth = float(atlas_width);	// set the atlas width
	*in_atlasTileWidth = float(tile_width); // set the atlas tile width

	// Step 1: initialize the atlas map; add tiles as needed
	AtlasMetaData currentAtlasMeta = findAtlasMetadata(atlas_width, tile_width);	// compare the two textures to get the atlas meta data
	setupTileArray(atlas_width, tile_width);
	for (tileBegin; tileBegin != tileEnd; tileBegin++)
	{
		std::string loadingString = atlasFolder + tileBegin->texture_file + ".jpg";
		insertTileLookup(tileBegin->materialID, tileBegin->array_x, tileBegin->array_y, loadingString);
	}

	// Step 2: set up the initial atlas texture, and set its max level
	glActiveTexture(GL_TEXTURE0);				// be sure to set the appropriate texture unit!
	glGenTextures(1, &*in_atlasTextureRef);		// generate the atlas texture
	glBindTexture(GL_TEXTURE_2D, *in_atlasTextureRef);	// bind it
	int atlasMaxLevelValue = (currentAtlasMeta.atlasMaxLevel - 1) - currentAtlasMeta.mipMapLevelDiff;	// get the deepest level that the texture atlas should go (used below)
	std::cout << "!!!! Atlas max level value is: " << atlasMaxLevelValue << std::endl;
	int atlas_loopLimit = currentAtlasMeta.atlasMaxLevel - currentAtlasMeta.mipMapLevelDiff;		// the loop limit for the calls to glTexImage2D
	int atlas_current_mipmap_dimension = currentAtlasMeta.atlasWidth;		// get the width of the atlas in pixels, as the value to start
	for (int x = 0; x < atlas_loopLimit; x++)
	{
		glTexImage2D(GL_TEXTURE_2D, x, GL_RGBA8, atlas_current_mipmap_dimension, atlas_current_mipmap_dimension, 0, GL_RGB, GL_UNSIGNED_BYTE, atlas_data);	// load atlas data for each mip map level
		atlas_current_mipmap_dimension /= 2;
	}
	//glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, atlasMaxLevelValue);	// set the maximum value for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 7);	// set the maximum value for this texture
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);							// experiment with these hints
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);							// experiment with these hints
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0.05f);					// some progress with this
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 7.0f);						// SIGNIFICANT progress with this

	// TEST 1
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// TEST 2
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// TEST 3
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	// TEST 4
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);



	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	// Step 3: cycle through all tiles in the tileLookup map, and insert them
	auto currentTile = tileLookup.begin();		// get the beginning iterator for the tile lookup
	auto tileMapEnd = tileLookup.end();
	for (currentTile; currentTile != tileMapEnd; ++currentTile)
	{
		TriangleMaterial currentTileID = currentTile->first;									// fetch the tileID for the current looked-up tile
		TileLoadData dataToLoad = getTileLoadData(currentTileID);	// get the load data, to use it later for OpenGL
		std::string loadingTileName = dataToLoad.filename;						// get the filename of the texture to load
		int load_width, load_height, load_nrChannels;							// variables for the call to stbi_load
		unsigned char* load_data = stbi_load(loadingTileName.c_str(), &load_width, &load_height, &load_nrChannels, 0);

		GLuint TextureB;														// the temporary texture to use
		glGenTextures(1, &TextureB);
		glBindTexture(GL_TEXTURE_2D, TextureB);
		int tile_loopLimit = currentAtlasMeta.tileMaxLevel;						// the loop limit for the tiles
		int tile_current_mipmap_dimension = currentAtlasMeta.tileWidth;			// set the base tile width


		

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tile_current_mipmap_dimension, tile_current_mipmap_dimension, 0, GL_RGB, GL_UNSIGNED_BYTE, load_data);		// load tile data for each mip map level
		glGenerateMipmap(GL_TEXTURE_2D);

		unsigned int srcWidth = currentAtlasMeta.tileWidth;	// width/height of the area to be copied; starts at 512 (one-quarter) for a 1024x1024 image
		unsigned int srcHeight = currentAtlasMeta.tileWidth;

		unsigned int dstX = dataToLoad.x_pixel_begin;
		unsigned int dstY = dataToLoad.y_pixel_begin;

		for (unsigned int level = 0; level < tile_loopLimit; level++)
		{
			glCopyImageSubData(TextureB, GL_TEXTURE_2D, level, 0, 0, 0, *in_atlasTextureRef, GL_TEXTURE_2D, level, dstX, dstY, 0, srcWidth, srcHeight, 1);
			srcWidth /= 2;
			srcHeight /= 2;
			if (dstY != 0)		// can't divide by 0
			{
				dstY /= 2;
			}
			if (dstX != 0)		// "" ""
			{
				dstX /= 2;
			}
		}
		stbi_image_free(load_data);		// avoid memory leak
	}

	glBindTexture(GL_TEXTURE_2D, *in_atlasTextureRef);	// bind it
	//glGenerateMipmap(GL_TEXTURE_2D);

	// cleanup memory
	stbi_image_free(tile_data);
	stbi_image_free(atlas_data);

	// return the atlas pixel width
	//return atlas_width;
	
}

void AtlasMap::buildAtlasOnTextureUnit(GLenum in_texUnit, std::string in_atlasFolderName, GLuint* in_atlasTextureRef, float* in_atlasTileWidth, float* in_atlasWidth)
{
	AtlasPropertiesGL properties = readGLData(in_atlasFolderName);


	// gather information
	std::cout << "Prior to smart pointer get: " << std::endl;
	std::string atlasFolder = "graphics/textures/atlas/" + properties.atlasName + "/";
	std::cout << "~~~ ++!!!!+--> Atlas folder is: " << atlasFolder << std::endl;
	std::string baseAtlasTexture = "graphics/textures/atlas/" + properties.atlasName + "/" + properties.atlasBase + ".jpg";
	std::cout << "~~~ ++!!!!+--> Base atlas texture is: " << baseAtlasTexture << std::endl;

	std::vector<TileDataGLWIN>::iterator tileBegin = properties.tileList.begin();
	std::vector<TileDataGLWIN>::iterator tileEnd = properties.tileList.end();
	std::string firstTileTexture = "graphics/textures/atlas/" + properties.atlasName + "/" + tileBegin->texture_file + ".jpg";
	std::cout << "~~~ Initial tile texture is: " << firstTileTexture << std::endl;

	// set flip
	stbi_set_flip_vertically_on_load(true);

	//  get data on the atlas texture
	std::string atlasTextureName = baseAtlasTexture;
	int atlas_width, atlas_height, atlas_nrChannels;
	unsigned char* atlas_data = stbi_load(atlasTextureName.c_str(), &atlas_width, &atlas_height, &atlas_nrChannels, 0);

	// get data on the tile texture
	std::string tileTextureName = firstTileTexture;
	int tile_width, tile_height, tile_nrChannels;
	unsigned char* tile_data = stbi_load(tileTextureName.c_str(), &tile_width, &tile_height, &tile_nrChannels, 0);

	std::cout << "!!!! >>>> atlas_width: " << int(atlas_width) << std::endl;
	std::cout << "!!!! >>>> tile_width: " << int(tile_width) << std::endl;


	*in_atlasWidth = float(atlas_width);	// set the atlas width
	*in_atlasTileWidth = float(tile_width); // set the atlas tile width

	// Step 1: initialize the atlas map; add tiles as needed
	AtlasMetaData currentAtlasMeta = findAtlasMetadata(atlas_width, tile_width);	// compare the two textures to get the atlas meta data
	setupTileArray(atlas_width, tile_width);
	for (tileBegin; tileBegin != tileEnd; tileBegin++)
	{
		std::string loadingString = atlasFolder + tileBegin->texture_file + ".jpg";
		insertTileLookup(tileBegin->materialID, tileBegin->array_x, tileBegin->array_y, loadingString);
	}

	// Step 2: set up the initial atlas texture, and set its max level
	glActiveTexture(in_texUnit);				// be sure to set the appropriate texture unit!
	glGenTextures(1, &*in_atlasTextureRef);		// generate the atlas texture
	glBindTexture(GL_TEXTURE_2D, *in_atlasTextureRef);	// bind it
	int atlasMaxLevelValue = (currentAtlasMeta.atlasMaxLevel - 1) - currentAtlasMeta.mipMapLevelDiff;	// get the deepest level that the texture atlas should go (used below)
	std::cout << "!!!! Atlas max level value is: " << atlasMaxLevelValue << std::endl;
	int atlas_loopLimit = currentAtlasMeta.atlasMaxLevel - currentAtlasMeta.mipMapLevelDiff;		// the loop limit for the calls to glTexImage2D
	int atlas_current_mipmap_dimension = currentAtlasMeta.atlasWidth;		// get the width of the atlas in pixels, as the value to start
	for (int x = 0; x < atlas_loopLimit; x++)
	{
		glTexImage2D(GL_TEXTURE_2D, x, GL_RGBA8, atlas_current_mipmap_dimension, atlas_current_mipmap_dimension, 0, GL_RGB, GL_UNSIGNED_BYTE, atlas_data);	// load atlas data for each mip map level
		atlas_current_mipmap_dimension /= 2;
	}
	//glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, atlasMaxLevelValue);	// set the maximum value for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 7);	// set the maximum value for this texture
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);							// experiment with these hints
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);							// experiment with these hints
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0.05f);					// some progress with this
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 7.0f);						// SIGNIFICANT progress with this

	// TEST 1
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// TEST 2
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// TEST 3
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	// TEST 4
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);



	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	// Step 3: cycle through all tiles in the tileLookup map, and insert them
	auto currentTile = tileLookup.begin();		// get the beginning iterator for the tile lookup
	auto tileMapEnd = tileLookup.end();
	for (currentTile; currentTile != tileMapEnd; ++currentTile)
	{
		TriangleMaterial currentTileID = currentTile->first;									// fetch the tileID for the current looked-up tile
		TileLoadData dataToLoad = getTileLoadData(currentTileID);	// get the load data, to use it later for OpenGL
		std::string loadingTileName = dataToLoad.filename;						// get the filename of the texture to load
		int load_width, load_height, load_nrChannels;							// variables for the call to stbi_load
		unsigned char* load_data = stbi_load(loadingTileName.c_str(), &load_width, &load_height, &load_nrChannels, 0);

		GLuint TextureB;														// the temporary texture to use
		glGenTextures(1, &TextureB);
		glBindTexture(GL_TEXTURE_2D, TextureB);
		int tile_loopLimit = currentAtlasMeta.tileMaxLevel;						// the loop limit for the tiles
		int tile_current_mipmap_dimension = currentAtlasMeta.tileWidth;			// set the base tile width




		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tile_current_mipmap_dimension, tile_current_mipmap_dimension, 0, GL_RGB, GL_UNSIGNED_BYTE, load_data);		// load tile data for each mip map level
		glGenerateMipmap(GL_TEXTURE_2D);

		unsigned int srcWidth = currentAtlasMeta.tileWidth;	// width/height of the area to be copied; starts at 512 (one-quarter) for a 1024x1024 image
		unsigned int srcHeight = currentAtlasMeta.tileWidth;

		unsigned int dstX = dataToLoad.x_pixel_begin;
		unsigned int dstY = dataToLoad.y_pixel_begin;

		for (unsigned int level = 0; level < tile_loopLimit; level++)
		{
			glCopyImageSubData(TextureB, GL_TEXTURE_2D, level, 0, 0, 0, *in_atlasTextureRef, GL_TEXTURE_2D, level, dstX, dstY, 0, srcWidth, srcHeight, 1);
			srcWidth /= 2;
			srcHeight /= 2;
			if (dstY != 0)		// can't divide by 0
			{
				dstY /= 2;
			}
			if (dstX != 0)		// "" ""
			{
				dstX /= 2;
			}
		}
		stbi_image_free(load_data);		// avoid memory leak
	}

	glBindTexture(GL_TEXTURE_2D, *in_atlasTextureRef);	// bind it
	//glGenerateMipmap(GL_TEXTURE_2D);

	// cleanup memory
	stbi_image_free(tile_data);
	stbi_image_free(atlas_data);

	// return the atlas pixel width
	//return atlas_width;
}

void AtlasMap::setupTileArray(int in_atlasPixelsLength, int in_tilePixelsLength)
{
	std::cout << ">>>>>>>>> Initializing tile array...." << std::endl;
	tileDimension = in_atlasPixelsLength / in_tilePixelsLength;		// set the tile dimension
	tilePixelsLength = in_tilePixelsLength;							// set tile pixel length
	tileCoordinateLength = 1.0f / tileDimension;					// get the tile dimension
	AtlasTileArray.reset(new AtlasTile[tileDimension*tileDimension]);		// set up the tiles
	atlasTileArrayLength = tileDimension * tileDimension;			// make sure to set this value
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

AtlasMetaData AtlasMap::findAtlasMetadata(int in_atlasWidth, int in_tileWidth)
{
	// find exponent for power of 2 for atlas (this will be the currentLevel)
	int currentAtlasLevel = 0;
	int currentAtlasWidth = in_atlasWidth;
	int currentAtlasResult = 0;
	while (currentAtlasResult != 1)
	{
		currentAtlasResult = currentAtlasWidth / 2;
		currentAtlasWidth = currentAtlasResult;
		currentAtlasLevel++;

	}
	currentAtlasLevel++; // increment by one, to include the initial level 0
	std::cout << "Atlas has this many levels: " << currentAtlasLevel << std::endl;

	int currentTileLevel = 0;
	int currentTileWidth = in_tileWidth;
	int currentTileResult = 0;
	while (currentTileResult != 1)
	{
		currentTileResult = currentTileWidth / 2;
		currentTileWidth = currentTileResult;
		currentTileLevel++;
	}
	currentTileLevel++;
	std::cout << "Tile has this many levels: " << currentTileLevel << std::endl;
	std::cout << "Difference in levels: " << currentAtlasLevel - currentTileLevel << std::endl;

	AtlasMetaData returnData;
	returnData.atlasMaxLevel = currentAtlasLevel;
	returnData.atlasWidth = in_atlasWidth;
	returnData.tileWidth = in_tileWidth;
	returnData.tileMaxLevel = currentTileLevel;
	returnData.mipMapLevelDiff = (currentAtlasLevel - currentTileLevel);
	returnData.dimensionToSquare = int(pow(2.0f, (returnData.mipMapLevelDiff)));

	std::cout << "Tile width: " << returnData.tileWidth << std::endl;

	return returnData;

}

void AtlasMap::insertTileLookup(TriangleMaterial in_tileID, int in_tileX, int in_tileY, std::string in_fileName)
{
	TileMeta metaToInsert;
	metaToInsert.x_location = in_tileX;
	metaToInsert.y_location = in_tileY;
	metaToInsert.file_location = in_fileName;
	tileLookup[in_tileID] = metaToInsert;
}

TileLoadData AtlasMap::getTileLoadData(TriangleMaterial in_tileID)
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

TexturePoints AtlasMap::getUVPointsForTerrainTile(TriangleMaterial in_tileID, TexturePoints in_pointsToConvert)
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

	int someVal = 3; 
	//std::cout << "!!! pre-crash, (3) " << std::endl;
	//std::cin >> someVal;

	// find the index to look up, and get the offsets
	int x_lookup = tileLookup[in_tileID].x_location;
	int y_lookup = tileLookup[in_tileID].y_location;

	returnUVcoords.U_tile_coord = x_lookup;		// store the tile location values
	returnUVcoords.V_tile_coord = y_lookup;

	//std::cout << "!!! pre-crash, (4) " << std::endl;
	//std::cout << "Atlas tile array length: " << atlasTileArrayLength << std::endl;
	//std::cout << "Atlas tile ID to get is: " << in_tileID << std::endl;
	//std::cout << "Tile lookup size was: " << tileLookup.size() << std::endl;
	//std::cout << "X lookup was: " << x_lookup << std::endl;
	//std::cout << "Y lookup was: " << y_lookup << std::endl;
	//std::cin >> someVal;

	int indexToFind = findTileIndex(x_lookup, y_lookup);
	float U_offset = AtlasTileArray[indexToFind].x_float_offset;
	float V_offset = AtlasTileArray[indexToFind].y_float_offset;

	//std::cout << "!!! pre-crash, (5) " << std::endl;
	//std::cin >> someVal;

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