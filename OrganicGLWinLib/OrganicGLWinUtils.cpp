#include "stdafx.h"
#include "OrganicGLWinUtils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void OrganicGLWinUtils::createImmutableBufferMode1(GLuint* in_bufferID, GLuint* in_swapBufferID, int in_bufferSize)
{
	const GLbitfield bufferStorageFlags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// set mandatory flags
	glGenBuffers(1, in_bufferID);					// generate the buffer
	glBindBuffer(GL_ARRAY_BUFFER, *in_bufferID);	// bind the buffer
	//glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize*in_numberOfBuffers, NULL, bufferStorageFlags);	// allocate immutable buffer
	glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize, NULL, bufferStorageFlags);	// allocate immutable buffer
	glGenBuffers(1, in_swapBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, *in_swapBufferID);
	glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize, NULL, bufferStorageFlags);


	// multiple attributes per vertex array means its not tightly packed, so 5th parameter represents the byte offset between consecutive attributes
	/*
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);		
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)12);
	*/
	std::cout << ">> Mode 0 GL buffers set up OK. " << std::endl;
}

void OrganicGLWinUtils::createImmutableBufferMode0(GLuint* in_bufferID, GLuint* in_swapBufferID, int in_bufferSize)
{
	const GLbitfield bufferStorageFlags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// set mandatory flags
	glGenBuffers(1, in_bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, *in_bufferID);
	//glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize*in_numberOfBuffers, NULL, bufferStorageFlags);	// allocate immutable buffer
	glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize, NULL, bufferStorageFlags);	// allocate immutable buffer
	glGenBuffers(1, in_swapBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, *in_swapBufferID);
	glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize, NULL, bufferStorageFlags);
	/*
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride = 0 (tightly packed); bytes offset between consecutive generic vertex attributes is 0.
		(void*)0            // array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
							//For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
							//
	);

	*/
	std::cout << ">> Mode 1 GL buffers set up OK. " << std::endl;
}

void OrganicGLWinUtils::createImmutableBufferMode2(GLuint* in_bufferID, GLuint* in_swapBufferID, int in_bufferSize, GLuint* in_textureRef)
{
	const GLbitfield bufferStorageFlags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// set mandatory flags
	glGenBuffers(1, in_bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, *in_bufferID);
	//glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize*in_numberOfBuffers, NULL, bufferStorageFlags);	// allocate immutable buffer
	glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize, NULL, bufferStorageFlags);	// allocate immutable buffer
	glGenBuffers(1, in_swapBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, *in_swapBufferID);
	glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize, NULL, bufferStorageFlags);
	
	
	// create texture buffers
	/*
	glGenTextures(1, &*in_textureRef);
	glBindTexture(GL_TEXTURE_2D, *in_textureRef);

	// load the texture's JPG
	std::string imageName = "graphics/textures/container.jpg";
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(imageName.c_str(), &width, &height, &nrChannels, 0);	// from new stb_image.h (downloaded from ... ?)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);							// experiment with these hints
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 7);
	delete data;
	*/
	std::cout << ">>>> Mode 2 GL buffers set up OK. " << std::endl;
}

void OrganicGLWinUtils::createImmutableBuffer(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers)
{
	glGenBuffers(1, in_bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, *in_bufferID);
	const GLbitfield bufferStorageFlags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// set mandatory flags
	glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize*in_numberOfBuffers, NULL, bufferStorageFlags);	// allocate immutable buffer
}

void OrganicGLWinUtils::createBuffer(GLuint* in_bufferIDref)
{
	glGenBuffers(1, in_bufferIDref);					// generate the buffer
}

void OrganicGLWinUtils::createFBO(GLuint* in_fboIDref)
{
	glGenFramebuffers(1, in_fboIDref);
}

void OrganicGLWinUtils::createImmutableBufferExperimental(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers, GLuint* in_textureRef, GLuint* in_textureRef2)
{
	glGenBuffers(1, in_bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, *in_bufferID);
	const GLbitfield bufferStorageFlags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// set mandatory flags
	glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize*in_numberOfBuffers, NULL, bufferStorageFlags);	// allocate immutable buffer




	// create texture buffers
	glGenTextures(1, &*in_textureRef);
	glBindTexture(GL_TEXTURE_2D, *in_textureRef);



	// load first the texture's JPG
	std::string imageName = "graphics/textures/test3.jpg";
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(imageName.c_str(), &width, &height, &nrChannels, 0);	// from new stb_image.h (downloaded from ... ?)

	int numberOfLevels = 11;	// 10 is for 512 x 512
	int current_width = width;
	int current_height = height;
	std::cout << "Current width: " << current_width << std::endl;
	std::cout << "Current height: " << current_height << std::endl;
	int currentLevel = 0;
	for (int x = 0; x < numberOfLevels; x++)
	{
		glTexImage2D(GL_TEXTURE_2D, currentLevel, GL_RGB, current_width, current_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		currentLevel++;
		current_width /= 2;
		current_height /= 2;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);							// experiment with these hints
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 7);
	delete data;











	// create texture buffers
	glGenTextures(1, &*in_textureRef2);
	glBindTexture(GL_TEXTURE_2D, *in_textureRef2);



	// load first the texture's JPG
	imageName = "graphics/textures/test1.jpg";
	//int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data2 = stbi_load(imageName.c_str(), &width, &height, &nrChannels, 0);	// from new stb_image.h (downloaded from ... ?)

	
	numberOfLevels = 11;		// 11 is for 1024 x 1024
	current_width = width;
	current_height = height;
	currentLevel = 0;
	for (int x = 0; x < numberOfLevels; x++)
	{
		glTexImage2D(GL_TEXTURE_2D, currentLevel, GL_RGB, current_width, current_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
		currentLevel++;
		current_width /= 2;
		current_height /= 2;
	}
	

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
	//glGenerateMipmap(GL_TEXTURE_2D);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);							// experiment with these hints
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 7);
	delete data2;







}

void OrganicGLWinUtils::createImmutableBufferExperimental2(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers, GLuint* in_textureRef)
{
	glGenBuffers(1, in_bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, *in_bufferID);
	const GLbitfield bufferStorageFlags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// set mandatory flags
	glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize*in_numberOfBuffers, NULL, bufferStorageFlags);	// allocate immutable buffer

	//GLuint TextureA = loadDDS("textureA.DDS");		// load first texture
	GLuint TextureB = loadDDS("graphics/textures/textureB.DDS");		// load second texture

	/**/
	// function variables
	unsigned int srcX = 0;	// for reading the bottom left corner, from the source
	unsigned int srcY = 0;	// ""
	unsigned int srcWidth = 512;	// width/height of the area to be copied; starts at 512 (one-quarter) for a 1024x1024 image
	unsigned int srcHeight = 512;

	unsigned int dstX = 0;
	unsigned int dstY = 384;

	
	for (unsigned int level = 0; level < 10; level++)
	{
		glCopyImageSubData(TextureB, GL_TEXTURE_2D, level, srcX, srcY, 0, *in_textureRef, GL_TEXTURE_2D, level, dstX, dstY, 0, srcWidth, srcHeight, 1);
		srcWidth /= 2;
		srcHeight /= 2;
		dstY /= 2;
	}
	

	glBindBuffer(GL_ARRAY_BUFFER, *in_bufferID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);							// experiment with these hints
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

}

void OrganicGLWinUtils::createImmutableBufferExperimental3(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers, GLuint* in_textureRef)
{
	//glGenBuffers(1, in_bufferID);
	//glBindBuffer(GL_ARRAY_BUFFER, *in_bufferID);
	//const GLbitfield bufferStorageFlags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// set mandatory flags
	//glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize*in_numberOfBuffers, NULL, bufferStorageFlags);	// allocate immutable buffer


	GLuint TextureB;
	glGenTextures(1, &TextureB);
	glBindTexture(GL_TEXTURE_2D, TextureB);

	std::string imageName = "graphics/textures/test5.jpg";
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(imageName.c_str(), &width, &height, &nrChannels, 0);	// from new stb_image.h (downloaded from ... ?)

	int numberOfLevels = 10;
	int current_width = 512;
	int current_height = 512;
	int currentLevel = 0;
	for (int x = 0; x < numberOfLevels; x++)
	{
		glTexImage2D(GL_TEXTURE_2D, currentLevel, GL_RGBA8, current_width, current_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		currentLevel++;
		std::cout << "----------------" << std::endl;
		std::cout << "(B) current width: " << current_width << std::endl;
		std::cout << "(B) current height: " << current_height << std::endl;
		current_width /= 2;
		current_height /= 2;

	}

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 9);			// needs to be set to 9 if doing only down to 2x2 texture size (numberOfLevels = 10)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);							// experiment with these hints
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);



	// function variables
	unsigned int srcX = 0;	// for reading the bottom left corner, from the source
	unsigned int srcY = 0;	// ""
	unsigned int srcWidth = 512;	// width/height of the area to be copied; starts at 512 (one-quarter) for a 1024x1024 image
	unsigned int srcHeight = 512;

	unsigned int dstX = 0;
	unsigned int dstY = 384;


	for (unsigned int level = 0; level < 10; level++)
	{
		glCopyImageSubData(TextureB, GL_TEXTURE_2D, level, srcX, srcY, 0, *in_textureRef, GL_TEXTURE_2D, level, dstX, dstY, 0, srcWidth, srcHeight, 1);
		srcWidth /= 2;
		srcHeight /= 2;
		dstY /= 2;
	}


	//glBindBuffer(GL_ARRAY_BUFFER, *in_bufferID);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);							// experiment with these hints
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

AtlasMetaData OrganicGLWinUtils::findAtlasMetadata(int in_atlasWidth, int in_tileWidth)
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
	returnData.dimensionToSquare = pow(2.0f, (returnData.mipMapLevelDiff));

	std::cout << "Tile width: " << returnData.tileWidth << std::endl;

	return returnData;

}

void OrganicGLWinUtils::computeMatricesFromInputs(GLFWwindow* in_windowRef, float* in_horizontalAngle, float* in_verticalAngle, glm::vec3* in_position, float*  in_initialFoV, float* in_speed, float in_mouseSpeed, glm::mat4* in_projectionMatrix, glm::mat4* in_viewMatrix, glm::mat4* in_MVPref)
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(in_windowRef, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(in_windowRef, 1024 / 2, 768 / 2);

	// Compute new orientation
	*in_horizontalAngle += in_mouseSpeed * float(1024 / 2 - xpos);
	*in_verticalAngle += in_mouseSpeed * float(768 / 2 - ypos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(*in_verticalAngle) * sin(*in_horizontalAngle),
		sin(*in_verticalAngle),
		cos(*in_verticalAngle) * cos(*in_horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(*in_horizontalAngle - 3.14f / 2.0f),
		0,
		cos(*in_horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	if (glfwGetKey(in_windowRef, GLFW_KEY_UP) == GLFW_PRESS) {
		*in_position += direction * deltaTime * *in_speed;
	}
	// Move backward
	if (glfwGetKey(in_windowRef, GLFW_KEY_DOWN) == GLFW_PRESS) {
		*in_position -= direction * deltaTime * *in_speed;
	}
	// Strafe right
	if (glfwGetKey(in_windowRef, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		*in_position += right * deltaTime * *in_speed;
	}
	// Strafe left
	if (glfwGetKey(in_windowRef, GLFW_KEY_LEFT) == GLFW_PRESS) {
		*in_position -= right * deltaTime * *in_speed;
	}

	float FoV = *in_initialFoV;  // - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	*in_projectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 400.0f);
	// Camera matrix
	*in_viewMatrix = glm::lookAt(
		*in_position,           // Camera is here
		*in_position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);


	glm::mat4 Model = glm::mat4(1.0);
	*in_MVPref = *in_projectionMatrix * *in_viewMatrix * Model;

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

int OrganicGLWinUtils::setupTextureAtlasJPEG(GLuint* in_atlasTextureRef, AtlasMap* in_atlasRef, AtlasPropertiesGL* in_atlasPropertiesGLRef, float* in_atlasTileWidth, float* in_atlasWidth)
{
	// gather file information
	std::cout << "Prior to smart pointer get: " << std::endl;
	std::string atlasFolder = "graphics/textures/atlas/" + in_atlasPropertiesGLRef->atlasName + "/";
	std::cout << "~~~ ++!!!!+--> Atlas folder is: " << atlasFolder << std::endl;
	std::string baseAtlasTexture = "graphics/textures/atlas/" + in_atlasPropertiesGLRef->atlasName + "/" + in_atlasPropertiesGLRef->atlasBase + ".jpg";
	std::cout << "~~~ ++!!!!+--> Base atlas texture is: " << baseAtlasTexture << std::endl;

	std::vector<TileDataGLWIN>::iterator tileBegin = in_atlasPropertiesGLRef->tileList.begin();
	std::vector<TileDataGLWIN>::iterator tileEnd = in_atlasPropertiesGLRef->tileList.end();
	std::string firstTileTexture = "graphics/textures/atlas/" + in_atlasPropertiesGLRef->atlasName + "/" + tileBegin->texture_file + ".jpg";
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
	in_atlasRef->setupTileArray(atlas_width, tile_width);
	for (tileBegin; tileBegin != tileEnd; tileBegin++)
	{ 
			std::string loadingString = atlasFolder + tileBegin->texture_file + ".jpg";
			in_atlasRef->insertTileLookup(tileBegin->materialID, tileBegin->array_x, tileBegin->array_y, loadingString);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 7.0f);	// set the maximum value for this texture
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
	std::map<int, TileMeta>::iterator currentTile = in_atlasRef->tileLookup.begin();		// get the beginning iterator for the tile lookup
	std::map<int, TileMeta>::iterator tileMapEnd = in_atlasRef->tileLookup.end();
	for (currentTile; currentTile != tileMapEnd; ++currentTile)
	{
		int currentTileID = currentTile->first;									// fetch the tileID for the current looked-up tile
		TileLoadData dataToLoad = in_atlasRef->getTileLoadData(currentTileID);	// get the load data, to use it later for OpenGL
		std::string loadingTileName = dataToLoad.filename;						// get the filename of the texture to load
		int load_width, load_height, load_nrChannels;							// variables for the call to stbi_load
		unsigned char* load_data = stbi_load(loadingTileName.c_str(), &load_width, &load_height, &load_nrChannels, 0);

		GLuint TextureB;														// the temporary texture to use
		glGenTextures(1, &TextureB);
		glBindTexture(GL_TEXTURE_2D, TextureB);
		int tile_loopLimit = currentAtlasMeta.tileMaxLevel;						// the loop limit for the tiles
		int tile_current_mipmap_dimension = currentAtlasMeta.tileWidth;			// set the base tile width
		/*
		for (int x = 0; x < tile_loopLimit; x++)
		{
			glTexImage2D(GL_TEXTURE_2D, x, GL_RGBA8, tile_current_mipmap_dimension, tile_current_mipmap_dimension, 0, GL_RGB, GL_UNSIGNED_BYTE, load_data);		// load tile data for each mip map level
			tile_current_mipmap_dimension /= 2;									
		}
		*/
		
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
	return atlas_width;

}

int OrganicGLWinUtils::setupTextureAtlasJPEG2(GLuint* in_atlasTextureRef, AtlasMap* in_atlasRef, AtlasPropertiesGL* in_atlasPropertiesGLRef)
{
	std::cout << "Prior to smart pointer get: " << std::endl;
	std::string atlasFolder = "graphics/textures/atlas/" + in_atlasPropertiesGLRef->atlasName + "/";
	std::cout << "~~~ !!!!+--> Atlas folder is: " << atlasFolder << std::endl;
	std::string baseAtlasTexture = "graphics/textures/atlas/" + in_atlasPropertiesGLRef->atlasName + "/" + in_atlasPropertiesGLRef->atlasBase + ".jpg";
	std::cout << "~~~ !!!!+--> Base atlas texture is: " << baseAtlasTexture << std::endl;

	std::vector<TileDataGLWIN>::iterator tileBegin = in_atlasPropertiesGLRef->tileList.begin();
	std::vector<TileDataGLWIN>::iterator tileEnd = in_atlasPropertiesGLRef->tileList.end();
	std::string firstTileTexture = "graphics/textures/atlas/" + in_atlasPropertiesGLRef->atlasName + "/" + tileBegin->texture_file + ".jpg";
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

	// Step 1: initialize the atlas map; add tiles as needed
	AtlasMetaData currentAtlasMeta = findAtlasMetadata(atlas_width, tile_width);	// compare the two textures to get the atlas meta data
	in_atlasRef->setupTileArray(atlas_width, tile_width);
	for (tileBegin; tileBegin != tileEnd; tileBegin++)
	{
		std::string loadingString = atlasFolder + tileBegin->texture_file + ".jpg";
		in_atlasRef->insertTileLookup(tileBegin->materialID, tileBegin->array_x, tileBegin->array_y, loadingString);
	}


	// Step 2: set up the initial atlas texture, and set its max level
	glGenTextures(1, &*in_atlasTextureRef);		// generate the atlas texture
	glBindTexture(GL_TEXTURE_2D, *in_atlasTextureRef);	// bind it
	int atlasMaxLevelValue = (currentAtlasMeta.atlasMaxLevel - 1) - currentAtlasMeta.mipMapLevelDiff;	// get the deepest level that the texture atlas should go (used below)
	int atlas_loopLimit = currentAtlasMeta.atlasMaxLevel - currentAtlasMeta.mipMapLevelDiff;		// the loop limit for the calls to glTexImage2D
	int atlas_current_mipmap_dimension = currentAtlasMeta.atlasWidth;		// get the width of the atlas in pixels, as the value to start
	
	for (int x = 0; x < atlas_loopLimit; x++)
	{
		glTexImage2D(GL_TEXTURE_2D, x, GL_RGBA8, atlas_current_mipmap_dimension, atlas_current_mipmap_dimension, 0, GL_RGB, GL_UNSIGNED_BYTE, atlas_data);	// load atlas data for each mip map level
		atlas_current_mipmap_dimension /= 2;
	}
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, atlas_current_mipmap_dimension, atlas_current_mipmap_dimension, 0, GL_RGB, GL_UNSIGNED_BYTE, atlas_data);	// load atlas data for each mip map level
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, atlasMaxLevelValue);	// set the maximum value for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);							// experiment with these hints
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);			// GL_LINEAR_MIPMAP_LINEAR is REQUIRED FOR ATLAS!!!!!

	// return the atlas pixel width
	return atlas_width;

}

void OrganicGLWinUtils::createAndBindVertexArray(GLuint* in_bufferID)
{
	glGenVertexArrays(1, in_bufferID);
	glBindVertexArray(*in_bufferID);
}

void OrganicGLWinUtils::createAndBindDrawIndirectBuffer(GLuint* in_bufferID)
{
	glGenBuffers(1, in_bufferID);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, *in_bufferID);
}

GLFWwindow* OrganicGLWinUtils::createGLFWWindow(int in_width, int in_height)
{
	return glfwCreateWindow(in_width, in_height, "Organic Testing", NULL, NULL);
}

void OrganicGLWinUtils::setGLFWInputMode(GLFWwindow* in_window)
{
	glfwSetInputMode(in_window, GLFW_STICKY_KEYS, GL_TRUE);
}

void OrganicGLWinUtils::loadShadersViaMode(GLuint* in_programID, int in_mode)
{
	if (in_mode == 0)
	{
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/Mode0_VertexShader.vertexshader", "graphics/shaders/Mode0_FragmentShader.fragmentshader");
	}
	else if (in_mode == 1)
	{
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/Mode1_VertexShader.vertexshader", "graphics/shaders/Mode1_FragmentShader.fragmentshader");
	}
	else if (in_mode == 2)
	{
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/Mode2_VertexShader.vertexshader", "graphics/shaders/Mode2_FragmentShader.fragmentshader");
	}
	else if (in_mode == 3)
	{
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/Mode3_VertexShader.vertexshader", "graphics/shaders/Mode3_FragmentShader.fragmentshader");
		//*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/Mode5_Deferred2_VS.vertexshader", "graphics/shaders/Mode5_Deferred2_FS.fragmentshader");
	}
	else if (in_mode == 4)
	{
		std::cout << "Loading deferred shaders..." << std::endl;
		//*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/Mode4_DeferredVS.vertexshader", "graphics/shaders/Mode4_DeferredFS.fragmentshader");
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/Mode4_Deferred2_VS.vertexshader", "graphics/shaders/Mode4_Deferred2_FS.fragmentshader");
	}
	else if (in_mode == 5)
	{
		//*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/Mode5_Deferred2_VS.vertexshader", "graphics/shaders/Mode5_Deferred2_FS.fragmentshader");
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/Mode4_DeferredVS.vertexshader", "graphics/shaders/Mode4_DeferredFS.fragmentshader");
	}
}

void OrganicGLWinUtils::loadShadersViaMode(GLuint* in_programID, std::string in_programName)
{
	if (in_programName == "TerrainGearT1")
	{
		std::cout << "##### ATTEMPTING LOAD FOR TerrainGearT1 " << std::endl;
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/TerrainGearT1.vertexshader", "graphics/shaders/TerrainGearT1.fragmentshader");
	}
	else if (in_programName == "TerrainGearT2")
	{
		std::cout << "##### ATTEMPTING LOAD FOR TerrainGearT2 " << std::endl;
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/TerrainGearT2.vertexshader", "graphics/shaders/TerrainGearT2.fragmentshader");
	}

	else if (in_programName == "HighlighterGearT1")
	{
		std::cout << "##### ATTEMPTING LOAD FOR HighlighterGearT1 " << std::endl;
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/HighlighterGearT1.vertexshader", "graphics/shaders/HighlighterGearT1.fragmentshader");
	}
	else if (in_programName == "TerrainComputeGearT1")
	{
		std::cout << "##### ATTEMPTING LOAD FOR TerrainComputeGearT1 " << std::endl;
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/TerrainComputeGearT1.vertexshader", "graphics/shaders/TerrainComputeGearT1.fragmentshader");
	}
	else if (in_programName == "DeferredComputeResultsGearT1")
	{
		std::cout << "##### ATTEMPTING LOAD FOR DeferredComputeResultsGearT1 " << std::endl;
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/DeferredComputeResultsGearT1.vertexshader", "graphics/shaders/DeferredComputeResultsGearT1.fragmentshader");
	}

	else if (in_programName == "TerrainForwardGearT1")
	{
		std::cout << "##### ATTEMPTING LOAD FOR TerrainForwardGearT1 " << std::endl;
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/TerrainForwardGearT1.vertexshader", "graphics/shaders/TerrainForwardGearT1.fragmentshader");
	}

	else if (in_programName == "TerrainSimpleGearT1")
	{
		std::cout << "##### ATTEMPTING LOAD FOR TerrainSimpleGearT1 " << std::endl;
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/TerrainSimpleGearT1.vertexshader", "graphics/shaders/TerrainSimpleGearT1.fragmentshader");
	}
	else if (in_programName == "TerrainDebugGearT1")
	{
		std::cout << "##### ATTEMPTING LOAD FOR TerrainDebugGearT1 " << std::endl;
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/TerrainDebugGearT1.vertexshader", "graphics/shaders/TerrainDebugGearT1.fragmentshader");
	}
	else if (in_programName == "TerrainPrimitiveGearT1")
	{
		std::cout << "##### ATTEMPTING LOAD FOR TerrainPrimitiveGearT1 " << std::endl;
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/TerrainPrimitiveGearT1.vertexshader", "graphics/shaders/TerrainPrimitiveGearT1.fragmentshader");
	}
	else if (in_programName == "InstancedHighlighterGearT1")
	{
		std::cout << "##### ATTEMPTING LOAD FOR InstancedHighlighterGearT1" << std::endl;
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/InstancedHighlighterGearT1.vertexshader", "graphics/shaders/InstancedHighlighterGearT1.fragmentshader");
	}
	else if (in_programName == "TerrainLightingComputeGearT1")
	{
		std::cout << "##### ATTEMPTING LOAD FOR TerrainLightingComputeGearT1" << std::endl;
		*in_programID = OrganicShaderLoader::LoadShaders("graphics/shaders/TerrainLightingComputeGearT1.vertexshader", "graphics/shaders/TerrainLightingComputeGearT1.fragmentshader");
	}
}

void OrganicGLWinUtils::loadComputeShader(GLuint* in_programID, std::string in_computeShaderName)
{
	if (in_computeShaderName == "DeferredComputeGearT1")
	{
		std::cout << "##### ATTEMPTING LOAD COMPUTE SHADER FOR DeferredComputeT1 " << std::endl;
		*in_programID = OrganicShaderLoader::LoadComputeShader("graphics/shaders/DeferredComputeGearT1.computeshader");
	}
	else if (in_computeShaderName == "DeferredLightingComputeGearT1")
	{
		std::cout << "##### ATTEMPTING LOAD COMPUTE SHADER FOR DeferredComputeT1 " << std::endl;
		*in_programID = OrganicShaderLoader::LoadComputeShader("graphics/shaders/DeferredLightingComputeGearT1.computeshader");
	}
	else if (in_computeShaderName == "ComputeCopyRBGFromTextureToImageGearT1")
	{
		std::cout << "##### ATTEMPTING LOAD COMPUTE SHADER FOR ComputeCopyRBGFromTextureToImageGearT1 " << std::endl;
		*in_programID = OrganicShaderLoader::LoadComputeShader("graphics/shaders/ComputeCopyRBGFromTextureToImageGearT1.computeshader");
	}

}

void OrganicGLWinUtils::initializeLibraryAndSetHints()
{
	if (!glfwInit())	// initialize GLFW library
	{
		fprintf(stderr, "||||||||||||||||Failed to initialize GLFW\n");
		getchar();
		//return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void OrganicGLWinUtils::checkWindowValidity(GLFWwindow* in_window)
{
	if (in_window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		//return -1;
	}
}

void OrganicGLWinUtils::makeContextCurrent(GLFWwindow* in_window)
{
	glfwMakeContextCurrent(in_window);
}

void OrganicGLWinUtils::setBasicStates()
{
	glEnable(GL_DEPTH_TEST); // Enable depth test
	glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one
}

void OrganicGLWinUtils::setClearColor(float in_red, float in_green, float in_blue, float in_alpha)
{
	glClearColor(in_red, in_green, in_blue, in_alpha);
}

void OrganicGLWinUtils::copyToBuffer(GLuint* in_readBufferID, GLuint* in_writeBufferID, int in_readByteOffset, int in_readByteSize, int in_writeByteOffset)
{
	glBindBuffer(GL_COPY_READ_BUFFER, *in_readBufferID);	// set the read buffer
	glBindBuffer(GL_COPY_WRITE_BUFFER, *in_writeBufferID);
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, in_readByteOffset, in_writeByteOffset, in_readByteSize);
}

void OrganicGLWinUtils::initializeGlew()
{
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "----------------Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		//return -1;
	}
}

void OrganicGLWinUtils::multiDrawArraysMode0(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, int in_numberOfCollections)
{
	//float testFloat = in_MVPmat4ref[0][0];
	glm::mat4 MVPref = *in_MVPmat4ref;	// send updated MVP transform to shader
	glUniformMatrix4fv(*in_MVPuniformLocation, 1, GL_FALSE, &MVPref[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, *in_drawArrayID);		// bind to the draw array, to setup vertex attributes
	
	//glBindBuffer(GL_DRAW_INDIRECT_BUFFER, *in_indirectBufferID);						// switch to indirect draw buffer
	glMultiDrawArrays(GL_TRIANGLES, in_startArray, in_vertexCount, in_numberOfCollections);		// perform the draw

	//glBindBuffer(GL_ARRAY_BUFFER, *in_drawArrayID);										// switch back to the draw array, to disable vertex attributes
	//glDisableVertexAttribArray(0);
}

void OrganicGLWinUtils::multiDrawArraysMode1(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, int in_numberOfCollections)
{
	glm::mat4 MVPref = *in_MVPmat4ref;	// send updated MVP transform to shader
	glUniformMatrix4fv(*in_MVPuniformLocation, 1, GL_FALSE, &MVPref[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, *in_drawArrayID);		// bind to the draw array, to setup vertex attributes

	glMultiDrawArrays(GL_TRIANGLES, in_startArray, in_vertexCount, in_numberOfCollections);

}

void OrganicGLWinUtils::multiDrawArraysMode2(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, GLuint* in_textureRef, GLuint* in_textureUniformRef, int in_numberOfCollections)
{
	glm::mat4 MVPref = *in_MVPmat4ref;	// send updated MVP transform to shader
	glUniformMatrix4fv(*in_MVPuniformLocation, 1, GL_FALSE, &MVPref[0][0]);

	glActiveTexture(GL_TEXTURE0);	// send updated texture uniform to shader
	glBindTexture(GL_TEXTURE_2D, *in_textureRef);
	glUniform1i(*in_textureUniformRef, 0);
	glBindBuffer(GL_ARRAY_BUFFER, *in_drawArrayID);

	glMultiDrawArrays(GL_TRIANGLES, in_startArray, in_vertexCount, in_numberOfCollections);
}

void OrganicGLWinUtils::multiDrawArraysMode3(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, GLuint* in_textureRef, GLuint* in_textureUniformRef, int in_numberOfCollections, GLuint* in_textureWidthRef, glm::vec3* in_textureWidth, GLuint* in_atlasWidthRef, float in_atlasWidth, GLuint* in_atlasTileWidthRef, float in_atlasTileWidth)
{
	glm::mat4 MVPref = *in_MVPmat4ref;	// send updated MVP transform to shader
	glm::vec3 vecRef = *in_textureWidth;
	glUniformMatrix4fv(*in_MVPuniformLocation, 1, GL_FALSE, &MVPref[0][0]);

	glActiveTexture(GL_TEXTURE0);	// send updated texture uniform to shader
	glBindTexture(GL_TEXTURE_2D, *in_textureRef);
	glUniform1i(*in_textureUniformRef, 0);
	//glUniform1f(*in_textureWidthRef, in_textureWidth);
	glUniform3fv(*in_textureWidthRef, 1, &vecRef[0]);
	glUniform1f(*in_atlasWidthRef, in_atlasWidth);
	glUniform1f(*in_atlasTileWidthRef, in_atlasTileWidth);
	glBindBuffer(GL_ARRAY_BUFFER, *in_drawArrayID);

	glMultiDrawArrays(GL_TRIANGLES, in_startArray, in_vertexCount, in_numberOfCollections);
}

void OrganicGLWinUtils::shutdownOpenGLBasic(GLuint* in_terrainBufferID, GLuint* in_vertexArrayID, GLuint* in_programID)
{
	glDeleteBuffers(1, in_terrainBufferID);
	glDeleteVertexArrays(1, in_vertexArrayID);
	glDeleteProgram(*in_programID);
	glfwTerminate();
}

GLuint OrganicGLWinUtils::loadDDS(const char* imagepath)
{
	unsigned char header[124];

	FILE *fp;

	/* try to open the file */
	fp = fopen(imagepath, "rb");
	if (fp == NULL) {
		printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
		return 0;
	}

	/* verify the type of file */
	char filecode[4];
	fread(filecode, 1, 4, fp);
	if (strncmp(filecode, "DDS ", 4) != 0) {
		fclose(fp);
		return 0;
	}

	/* get the surface desc */
	fread(&header, 124, 1, fp);

	unsigned int height = *(unsigned int*)&(header[8]);
	unsigned int width = *(unsigned int*)&(header[12]);
	unsigned int linearSize = *(unsigned int*)&(header[16]);
	unsigned int mipMapCount = *(unsigned int*)&(header[24]);
	unsigned int fourCC = *(unsigned int*)&(header[80]);


	unsigned char * buffer;
	unsigned int bufsize;
	/* how big is it going to be including all mipmaps? */
	bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
	buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
	fread(buffer, 1, bufsize, fp);
	/* close the file pointer */
	fclose(fp);

	unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
	unsigned int format;
	switch (fourCC)
	{
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		break;
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		break;
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		break;
	default:
		free(buffer);
		return 0;
	}

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
	unsigned int offset = 0;

	/* load the mipmaps */
	for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
	{
		unsigned int size = ((width + 3) / 4)*((height + 3) / 4)*blockSize;
		glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
			0, size, buffer + offset);

		offset += size;
		width /= 2;
		height /= 2;

		// Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
		if (width < 1) width = 1;
		if (height < 1) height = 1;

	}

	free(buffer);

	return textureID;
}

void OrganicGLWinUtils::IMGuiInit(GLFWwindow* in_window)
{
	const char* glsl_version = "#version 130";
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(in_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void OrganicGLWinUtils::IMGuiNewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();		// (required)
	ImGui_ImplGlfw_NewFrame();	// setup the new frame (required)
	ImGui::NewFrame();

}

void OrganicGLWinUtils::IMGuiTestText()
{
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);	// not sure what 2nd argument is here?
	bool window_val = true;
	ImGui::Begin("First Window Ever", &window_val);	// needs to accept a bool of true? hmmm ok
	ImGui::Text("Options:");
	ImGui::End();
}

void OrganicGLWinUtils::IMGuiTextButton()
{
	ImGui::SetNextWindowPos(ImVec2(5, 10));
	ImGui::SetNextWindowSize(ImVec2(100, 60), ImGuiSetCond_FirstUseEver);
	bool window_val = true;
	ImGui::Begin("Test Button", &window_val);
	if (ImGui::Button("click here"))
	{
		std::cout << "Button was clicked! " << std::endl;
	}
	ImGui::End();
}

void OrganicGLWinUtils::IMGuiRenderAndDraw()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void OrganicGLWinUtils::IMGuiPrepWorldLocation(float world_precise[3], int world_organicLoc[9])
{
	//SetNextWindowPos(const ImVec2& pos, ImGuiCond cond = 0, const ImVec2& pivot = ImVec2(0,0));
	ImGui::SetNextWindowPos(ImVec2(700, 10));
	ImGui::SetNextWindowSize(ImVec2(300, 120), ImGuiSetCond_FirstUseEver);
	bool window_val = true;
	ImGui::Begin("World Location", &window_val);
	ImGui::Text("Collection: %d, %d, %d", world_organicLoc[0], world_organicLoc[1], world_organicLoc[2]);
	ImGui::Text("Enclave   : %d, %d, %d", world_organicLoc[3], world_organicLoc[4], world_organicLoc[5]);
	ImGui::Text("Block     : %d, %d, %d", world_organicLoc[6], world_organicLoc[7], world_organicLoc[8]);
	ImGui::Text("Absolute  : %f, %f, %f", world_precise[0], world_precise[1], world_precise[2]);
	ImGui::End();
}

void OrganicGLWinUtils::IMGuiPrepBlockLocation(int world_organicLoc[9])
{
	ImGui::SetNextWindowPos(ImVec2(700, 150));
	ImGui::SetNextWindowSize(ImVec2(300, 120), ImGuiSetCond_FirstUseEver);
	bool window_val = true;
	ImGui::Begin("Block Location", &window_val);
	ImGui::Text("Collection: %d, %d, %d", world_organicLoc[0], world_organicLoc[1], world_organicLoc[2]);
	ImGui::Text("Enclave   : %d, %d, %d", world_organicLoc[3], world_organicLoc[4], world_organicLoc[5]);
	ImGui::Text("Block     : %d, %d, %d", world_organicLoc[6], world_organicLoc[7], world_organicLoc[8]);
	ImGui::End();
}

glm::quat OrganicGLWinUtils::createQuaternion(float in_degrees, glm::vec3 in_unitVector)
{
	const float fullRadians360 = 6.28319;
	float radianMultiplier = in_degrees / 360.0f;
	float degreesToRadians = radianMultiplier * fullRadians360;

	glm::quat returnQuat = glm::quat(cos(degreesToRadians / 2), sin(degreesToRadians / 2)*in_unitVector.x, sin(degreesToRadians / 2)*in_unitVector.y, sin(degreesToRadians / 2)*in_unitVector.z);

	return returnQuat;
}

glm::vec3 OrganicGLWinUtils::roundVec3ToHundredths(glm::vec3 in_vec3)
{
	glm::vec3 returnVec = in_vec3;
	returnVec.x = floor(in_vec3.x * 100 + 0.5) / 100;
	returnVec.y = floor(in_vec3.y * 100 + 0.5) / 100;
	returnVec.z = floor(in_vec3.z * 100 + 0.5) / 100;
	return returnVec;
}

glm::vec3 OrganicGLWinUtils::findTriangleCentroid(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2)
{
	glm::vec3 foundCentroid;
	foundCentroid.x = (in_point0.x + in_point1.x + in_point2.x) / 3;
	foundCentroid.y = (in_point0.y + in_point1.y + in_point2.y) / 3;
	foundCentroid.z = (in_point0.z + in_point1.z + in_point2.z) / 3;
	//std::cout << "Centroid X: " << foundCentroid.x << std::endl;
	//std::cout << "Centroid y: " << foundCentroid.y << std::endl;
	return foundCentroid;
}

glm::vec3 OrganicGLWinUtils::findTriangleCentroidDebug(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2)
{
	std::cout << "##### CALLED CENTROID DEBUG " << std::endl;
	std::cout << "point 0 x: " << in_point0.x << std::endl;
	std::cout << "point 1 x: " << in_point1.x << std::endl;
	std::cout << "point 2 x: " << in_point2.x << std::endl;


	glm::vec3 foundCentroid;
	foundCentroid.x = (in_point0.x + in_point1.x + in_point2.x) / 3;
	foundCentroid.y = (in_point0.y + in_point1.y + in_point2.y) / 3;
	foundCentroid.z = (in_point0.z + in_point1.z + in_point2.z) / 3;


	//std::cout << "Centroid X: " << foundCentroid.x << std::endl;
	//std::cout << "Centroid y: " << foundCentroid.y << std::endl;
	return foundCentroid;
}

glm::vec3 OrganicGLWinUtils::findTriangleNormal(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2)
{
	glm::vec3 returnVec;
	PointTranslationCheck pointTranslator;
	QuatRotationPoints quatPoints;

	// copy points 
	glm::vec3 point0Copy = in_point0;
	glm::vec3 point1Copy = in_point1;
	glm::vec3 point2Copy = in_point2;

	quatPoints.pointsRefVector.push_back(&point0Copy);
	quatPoints.pointsRefVector.push_back(&point1Copy);
	quatPoints.pointsRefVector.push_back(&point2Copy);

	// check for any translation
	pointTranslator.performCheck(quatPoints.getFirstPoint());
	if (pointTranslator.requiresTranslation == 1)
	{
		quatPoints.applyTranslation(pointTranslator.getTranslationValue());
		//std::cout << "!!! Points were translated; their values are: " << std::endl;
		//std::cout << "0: " << point0Copy.x << ", " << point0Copy.y << ", " << point0Copy.z << std::endl;
		//std::cout << "1: " << point1Copy.x << ", " << point1Copy.y << ", " << point1Copy.z << std::endl;
		//std::cout << "2: " << point2Copy.x << ", " << point2Copy.y << ", " << point2Copy.z << std::endl;
	}

	// calculate the normal, after translating point 0 of the triangle to point 0 (so that points 1 and 2 become appropriate values to get the cross product from)
	returnVec = glm::normalize(glm::cross(point1Copy, point2Copy));
	//std::cout << "Calculated triangle normal is: " << returnVec.x << ", " << returnVec.y << ", " << returnVec.z << std::endl;

	return returnVec;
}

bool OrganicGLWinUtils::checkIfPointLiesWithinTriangle(glm::vec3 in_pointToCheck, glm::vec3 in_trianglePoint0, glm::vec3 in_trianglePoint1, glm::vec3 in_trianglePoint2)
{
	bool isWithinTriangle = false;
	bool planeArrayCheckResult[3];
	struct TriangleLineSpec
	{
		ThreeDLineSegment segment;
		glm::vec3 centroidFacingNormal;
	};

	ThreeDLineSegment threeDline1(in_trianglePoint0, in_trianglePoint1);
	TriangleLineSpec line1;
	line1.segment = threeDline1;

	ThreeDLineSegment threeDline2(in_trianglePoint1, in_trianglePoint2);
	TriangleLineSpec line2;
	line2.segment = threeDline2;

	ThreeDLineSegment threeDline3(in_trianglePoint2, in_trianglePoint0);
	TriangleLineSpec line3;
	line3.segment = threeDline3;

	TriangleLineSpec lines[3];
	lines[0] = line1;
	lines[1] = line2;
	lines[2] = line3;

	glm::vec3 centroid = findTriangleCentroid(lines[0].segment.pointA, lines[1].segment.pointA, lines[2].segment.pointA);

	//std::cout << ">>>>>>>> point to check is: " << in_pointToCheck.x << ", " << in_pointToCheck.y << ", " << in_pointToCheck.z << std::endl;
	//std::cout << ">> point 0 is: " << line1.segment.pointA.x << ", " << line1.segment.pointA.y << ", " << line1.segment.pointA.z << std::endl;
	//std::cout << ">> point 1 is: " << line2.segment.pointA.x << ", " << line2.segment.pointA.y << ", " << line2.segment.pointA.z << std::endl;
	//std::cout << ">> point 2 is: " << line3.segment.pointA.x << ", " << line3.segment.pointA.y << ", " << line3.segment.pointA.z << std::endl;
	//std::cout << ">> centroid is: " << centroid.x << ", " << centroid.y << ", " << centroid.z << std::endl;
	

	// find the centroid-facing normal for each TriangleLineSpec
	for (int x = 0; x < 3; x++)
	{
		glm::vec3 pointACopy = lines[x].segment.pointA;
		glm::vec3 pointBCopy = lines[x].segment.pointB;
		glm::vec3 centroidPointCopy = centroid;

		// first, check for point translation
		PointTranslationCheck pointCheck;
		pointCheck.performCheck(pointACopy);
		if (pointCheck.requiresTranslation == 1)
		{
			glm::vec3 translationValue = pointCheck.getTranslationValue();
			pointACopy += translationValue;
			pointBCopy += translationValue;
			centroidPointCopy += translationValue;
		}

		//std::cout << ">>>>>: point A Copy, post translate: " << pointACopy.x << ", " << pointACopy.y << ", " << pointACopy.z << std::endl;
		//std::cout << ">>>>>: point B Copy, post translate: " << pointBCopy.x << ", " << pointBCopy.y << ", " << pointBCopy.z << std::endl;
		//std::cout << ">>>>>: centroid copy, post translate: " << centroidPointCopy.x << ", " << centroidPointCopy.y << ", " << centroidPointCopy.z << std::endl;

		QuatRotationPoints rotationPoints;
		rotationPoints.pointsRefVector.push_back(&pointACopy);
		rotationPoints.pointsRefVector.push_back(&pointBCopy);
		rotationPoints.pointsRefVector.push_back(&centroidPointCopy);

		QuatRotationManager rotationManager;
		rotationManager.initializeAndRunForFindingBorderLineEmptyNormal(&rotationPoints);

		//std::cout << ">>>>>: centroid copy, post translate (2): " << centroidPointCopy.x << ", " << centroidPointCopy.y << ", " << centroidPointCopy.z << std::endl;

		lines[x].centroidFacingNormal = centroidPointCopy;
	}


	for (int x = 0; x < 3; x++)
	{
		
		glm::vec3 pointACopy = lines[x].segment.pointA;
		glm::vec3 pointBCopy = lines[x].segment.pointB;
		glm::vec3 pointToCheck = in_pointToCheck;
		glm::vec3 centroidFacingNormal = lines[x].centroidFacingNormal;

		//std::cout << ">>> pushing back this value for centroid facing normal: " << centroidFacingNormal.x << ", " << centroidFacingNormal.y << ", " << centroidFacingNormal.z << std::endl;

		// first, check for point translation; only translate pointA/B, and the point to check against.
		PointTranslationCheck pointCheck;
		pointCheck.performCheck(pointACopy);
		if (pointCheck.requiresTranslation == 1)
		{
			glm::vec3 translationValue = pointCheck.getTranslationValue();
			pointACopy += translationValue;
			pointBCopy += translationValue;
			pointToCheck += translationValue;
		}

		QuatRotationPoints rotationPoints;
		rotationPoints.pointsRefVector.push_back(&pointACopy);
		rotationPoints.pointsRefVector.push_back(&pointBCopy);
		rotationPoints.pointsRefVector.push_back(&pointToCheck);
		rotationPoints.pointsRefVector.push_back(&centroidFacingNormal);		// fetched by rotationManager, to check for flipping on Z-axis

		QuatRotationManager rotationManager;
		planeArrayCheckResult[x] = rotationManager.initializeAndRunForCheckingIfPointIswithinPlane(&rotationPoints);	// the point to check will be rounded to nearest hundredths, 
																														// during the quat rotation
		
	}

	int withinPlaneCount = 0;
	for (int x = 0; x < 3; x++)
	{
		if (planeArrayCheckResult[x] == true)
		{
			//std::cout << "!!! found as being within plane! " << std::endl;

			withinPlaneCount++;
		}
	}

	if (withinPlaneCount == 3)
	{
		//std::cout << "OrganicGLWinUtils::checkIfPointLiesWithinTriangle --> Point is WITHIN triangle. " << std::endl;
		isWithinTriangle = true;
	}

	return isWithinTriangle;
}

bool OrganicGLWinUtils::checkIfPointLiesWithinTriangleWithRotateToZ(glm::vec3 in_pointToCheck, glm::vec3 in_trianglePoint0, glm::vec3 in_trianglePoint1, glm::vec3 in_trianglePoint2)
{
	bool isWithinTriangle = false;
	bool planeArrayCheckResult[3];
	struct TriangleLineSpec
	{
		ThreeDLineSegment segment;
		glm::vec3 centroidFacingNormal;
	};

	ThreeDLineSegment threeDline1(in_trianglePoint0, in_trianglePoint1);
	TriangleLineSpec line1;
	line1.segment = threeDline1;

	ThreeDLineSegment threeDline2(in_trianglePoint1, in_trianglePoint2);
	TriangleLineSpec line2;
	line2.segment = threeDline2;

	ThreeDLineSegment threeDline3(in_trianglePoint2, in_trianglePoint0);
	TriangleLineSpec line3;
	line3.segment = threeDline3;

	TriangleLineSpec lines[3];
	lines[0] = line1;
	lines[1] = line2;
	lines[2] = line3;

	glm::vec3 centroid = findTriangleCentroid(lines[0].segment.pointA, lines[1].segment.pointA, lines[2].segment.pointA);

	//std::cout << "(With Rotating to Z) >>>>>>>> point to check is: " << in_pointToCheck.x << ", " << in_pointToCheck.y << ", " << in_pointToCheck.z << std::endl;
	//std::cout << "(With Rotating to Z) >> point 0 is: " << line1.segment.pointA.x << ", " << line1.segment.pointA.y << ", " << line1.segment.pointA.z << std::endl;
	//std::cout << "(With Rotating to Z) >> point 1 is: " << line2.segment.pointA.x << ", " << line2.segment.pointA.y << ", " << line2.segment.pointA.z << std::endl;
	//std::cout << "(With Rotating to Z) >> point 2 is: " << line3.segment.pointA.x << ", " << line3.segment.pointA.y << ", " << line3.segment.pointA.z << std::endl;
	//std::cout << "(With Rotating to Z) >> centroid is: " << centroid.x << ", " << centroid.y << ", " << centroid.z << std::endl;


	// find the centroid-facing normal for each TriangleLineSpec
	for (int x = 0; x < 3; x++)
	{
		glm::vec3 pointACopy = lines[x].segment.pointA;
		glm::vec3 pointBCopy = lines[x].segment.pointB;
		glm::vec3 centroidPointCopy = centroid;

		// first, check for point translation
		PointTranslationCheck pointCheck;
		pointCheck.performCheck(pointACopy);
		if (pointCheck.requiresTranslation == 1)
		{
			glm::vec3 translationValue = pointCheck.getTranslationValue();
			pointACopy += translationValue;
			pointBCopy += translationValue;
			centroidPointCopy += translationValue;
		}

		//std::cout << ">>>>>: point A Copy, post translate: " << pointACopy.x << ", " << pointACopy.y << ", " << pointACopy.z << std::endl;
		//std::cout << ">>>>>: point B Copy, post translate: " << pointBCopy.x << ", " << pointBCopy.y << ", " << pointBCopy.z << std::endl;
		//std::cout << ">>>>>: centroid copy, post translate: " << centroidPointCopy.x << ", " << centroidPointCopy.y << ", " << centroidPointCopy.z << std::endl;

		QuatRotationPoints rotationPoints;
		rotationPoints.pointsRefVector.push_back(&pointACopy);
		rotationPoints.pointsRefVector.push_back(&pointBCopy);
		rotationPoints.pointsRefVector.push_back(&centroidPointCopy);

		QuatRotationManager rotationManager;
		//rotationManager.initializeAndRunForFindingBorderLineEmptyNormal(&rotationPoints);
		rotationManager.initializeAndRunForFindingBorderLineEmptyNormalWithRotateToZ(&rotationPoints);


		//std::cout << ">>>>>: centroid copy, post translate (2): " << centroidPointCopy.x << ", " << centroidPointCopy.y << ", " << centroidPointCopy.z << std::endl;

		lines[x].centroidFacingNormal = centroidPointCopy;
	}


	for (int x = 0; x < 3; x++)
	{

		glm::vec3 pointACopy = lines[x].segment.pointA;
		glm::vec3 pointBCopy = lines[x].segment.pointB;
		glm::vec3 pointToCheck = in_pointToCheck;
		glm::vec3 centroidFacingNormal = lines[x].centroidFacingNormal;

		//std::cout << ">>> pushing back this value for centroid facing normal: " << centroidFacingNormal.x << ", " << centroidFacingNormal.y << ", " << centroidFacingNormal.z << std::endl;

		// first, check for point translation; only translate pointA/B, and the point to check against.
		PointTranslationCheck pointCheck;
		pointCheck.performCheck(pointACopy);
		if (pointCheck.requiresTranslation == 1)
		{
			glm::vec3 translationValue = pointCheck.getTranslationValue();
			pointACopy += translationValue;
			pointBCopy += translationValue;
			pointToCheck += translationValue;
		}

		QuatRotationPoints rotationPoints;
		rotationPoints.pointsRefVector.push_back(&pointACopy);
		rotationPoints.pointsRefVector.push_back(&pointBCopy);
		rotationPoints.pointsRefVector.push_back(&pointToCheck);
		rotationPoints.pointsRefVector.push_back(&centroidFacingNormal);		// fetched by rotationManager, to check for flipping on Z-axis

		QuatRotationManager rotationManager;
		planeArrayCheckResult[x] = rotationManager.initializeAndRunForCheckingIfPointIswithinPlaneWithRotateToZ(&rotationPoints);	// the point to check will be rounded to nearest hundredths, 
																														// during the quat rotation

	}

	int withinPlaneCount = 0;
	for (int x = 0; x < 3; x++)
	{
		if (planeArrayCheckResult[x] == true)
		{
			//std::cout << "!!! found as being within plane! " << std::endl;

			withinPlaneCount++;
		}
	}

	if (withinPlaneCount == 3)
	{
		//std::cout << "OrganicGLWinUtils::checkIfPointLiesWithinTriangle --> Point is WITHIN triangle. " << std::endl;
		isWithinTriangle = true;
	}

	return isWithinTriangle;
}

TwoDPoint OrganicGLWinUtils::convertGlmVec3To2D(glm::vec3 in_glmvec3)
{
	TwoDPoint returnPoint(in_glmvec3.x, in_glmvec3.y);
	return returnPoint;
}

glm::vec3 OrganicGLWinUtils::convert2DToGlmVec3(TwoDPoint in_twoDPoint)
{
	glm::vec3 returnVec(in_twoDPoint.x, in_twoDPoint.y, 0);
	return returnVec;
}
