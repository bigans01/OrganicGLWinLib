#include "stdafx.h"
#include "ShaderManagerDeferred2Debug.h"

void ShaderManagerDeferred2Debug::initialize(int in_windowWidth, int in_windowHeight)
{

}

void ShaderManagerDeferred2Debug::initializeShader(int in_windowWidth, int in_windowHeight, int in_terrainBufferSize)
{
	width = in_windowWidth;
	height = in_windowHeight;
	OrganicGLWinUtils::initializeLibraryAndSetHints();				// initialization
	window = OrganicGLWinUtils::createGLFWWindow(width, height);	// create the GLFW window
	OrganicGLWinUtils::checkWindowValidity(window);			// CHECK FOR DEFERRED? 
	OrganicGLWinUtils::makeContextCurrent(window);
	OrganicGLWinUtils::initializeGlew();
	OrganicGLWinUtils::setBasicStates();					// CHECK FOR DEFERRED?
	OrganicGLWinUtils::setGLFWInputMode(window);
	OrganicGLWinUtils::setClearColor(0.0f, 0.0f, 0.7f, 0.0f);	// background color
	OrganicGLWinUtils::createAndBindDrawIndirectBuffer(&organicGLIndirectBufferID);		// indirect draw buffer

	// NEW ---> setup IMGui
	OrganicGLWinUtils::IMGuiInit(window);

	// load shaders
	OrganicGLWinUtils::loadShadersViaMode(&shaderProgramID, 5);		// load mode 5 shaders;	(3 will become 5, but only during testing, until OGLM functionality is revisited -- BE SURE to set the appropriate shader back when done!!)
	OGLMVertexSubBufferSize = in_terrainBufferSize * 1000000;		// set terrain buffer size
	OrganicGLWinUtils::createImmutableBufferMode2(&terrainBufferID, &terrainSwapID, OGLMVertexSubBufferSize, &textID);
	mvpHandle = glGetUniformLocation(shaderProgramID, "MVP");
	textureUniform = glGetUniformLocation(shaderProgramID, "terrainTexture");
	worldPosUniform = glGetUniformLocation(shaderProgramID, "worldPosition");
	atlasWidthUniform = glGetUniformLocation(shaderProgramID, "atlasTextureWidth");
	atlasTileWidthUniform = glGetUniformLocation(shaderProgramID, "atlasTileTextureWidth");
	glUseProgram(shaderProgramID);

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	setupRenderQuad();
	setupFBO();
	setupTerrainVAO();
	acquireSubroutineIndices();
}

void ShaderManagerDeferred2Debug::setMatrices()
{
	MVP = projection * view * model;
	mvpHandle = glGetUniformLocation(shaderProgramID, "MVP");	// find the MVP uniform
	glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &MVP[0][0]);		// set the uniform

	glm::mat4 mv = view * model;
	modelViewHandle = glGetUniformLocation(shaderProgramID, "ModelViewMatrix");
	glUniformMatrix4fv(modelViewHandle, 1, GL_FALSE, &mv[0][0]);

	//glUniformMatrix4fv(*in_MVPuniformLocation, 1, GL_FALSE, &MVPref[0][0]);

	//glActiveTexture(GL_TEXTURE0);	// send updated texture uniform to shader
	//glBindTexture(GL_TEXTURE_2D, textID);		// bind to the atlas texture
	//glUniform1i(textureUniform, 0);
	glUniform3fv(worldPosUniform, 1, &position[0]);
	glUniform1f(atlasWidthUniform, atlasWidth);
	glUniform1f(atlasTileWidthUniform, atlasTileWidth);
}

void ShaderManagerDeferred2Debug::shutdownGL()
{

}

void ShaderManagerDeferred2Debug::setupRenderQuad()
{
	OrganicGLWinUtils::createAndBindVertexArray(&quadVaoID);	// create/bind the VAO to quadVaoID
	glGenBuffers(1, &quadBufferID);					// generate the buffer
	glBindBuffer(GL_ARRAY_BUFFER, quadBufferID);	// bind

	GLfloat quadData[] =
	{												// first 3 floats = position, 4th and 5th = texture coords. 
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	0.0f, 0.0f,			// 1st point, lower left
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	0.0f, 0.0f,			// 2nd point, lower right
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,				// 3rd point, upper right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,	0.0f, 0.0f,			// 4th point, lower left
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,				// 5th point, upper right
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f				// 6th point, upper left
	};

	glBufferData(GL_ARRAY_BUFFER, 6 * 7 * sizeof(float), quadData, GL_STATIC_DRAW);		// populate the data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)20);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);			// may not need to reset here, but lets do it anyway
}

void ShaderManagerDeferred2Debug::setupFBO()
{
	// set up the deferred FBO
	GLuint depthBuf, posTex, colorTex;
	glGenFramebuffers(1, &deferredFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);

	// depth buffer
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	// create textures for position and color (bindings 1 and 2, respectively).
	// unit 0 is reserved for original texture (albedo) lookup
	createGBufText(GL_TEXTURE1, GL_RGB32F, posTex);		// g buffer for position = unit 1
	createGBufText(GL_TEXTURE2, GL_RGB8, colorTex);		// g buffer for color = unit 2

	// attach textures to the frame buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, posTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, colorTex, 0);

	GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(3, drawBuffers);

	// check the buffer status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);				// reset back to the default OpenGL FBO
}

void ShaderManagerDeferred2Debug::setupTerrainVAO()
{
	OrganicGLWinUtils::createAndBindVertexArray(&terrainVaoID);	// create/bind the VAO to quadVaoID
	glBindBuffer(GL_ARRAY_BUFFER, terrainBufferID);	// bind to the immutable buffer before setting the attribs
	GLfloat testTriangleData[] =
	{
		-2.0f, -2.0f, 0.0f, 0.0f, 0.0f,	0.0f, 0.0f, // lower left
		2.0f, -2.0f, 0.0f, 1.0f, 0.0f,	0.0f, 0.0f,// lower right
		-2.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,	// uper left
	};
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * 7 * sizeof(float), testTriangleData);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 7, (void*)20);
	glEnableVertexAttribArray(2);

	// set the triangle's texture in texture unit 0
	glActiveTexture(GL_TEXTURE0);		// set active texture as unit 0
	glGenTextures(1, &mainAlbedoText);
	glBindTexture(GL_TEXTURE_2D, mainAlbedoText);
	std::string testTexture = "testTexture.jpg";	// basic test texture
	std::cout << "Attempting jpg load..." << std::endl;
	int testTextureWidth;
	int testTextureHeight;
	int testTextureNRChannels;
	unsigned char* testTextureData = stbi_load(testTexture.c_str(), &testTextureWidth, &testTextureHeight, &testTextureNRChannels, 0);	// retrieve the data from the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, testTextureWidth, testTextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, testTextureData);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void ShaderManagerDeferred2Debug::acquireSubroutineIndices()
{
	pass1index = glGetSubroutineIndex(shaderProgramID, GL_FRAGMENT_SHADER, "pass1");
	pass2index = glGetSubroutineIndex(shaderProgramID, GL_FRAGMENT_SHADER, "pass2");
}

void ShaderManagerDeferred2Debug::createGBufText(GLenum texUnit, GLenum  format, GLuint &texid)
{
	glActiveTexture(texUnit);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void ShaderManagerDeferred2Debug::render()
{
	do {
		computeCameraDirectionAndPosition();
		updateMatricesAndDelta();

		// IMGUI prep
		ImGui_ImplOpenGL3_NewFrame();		// (required)
		ImGui_ImplGlfw_NewFrame();	// setup the new frame (required)
		ImGui::NewFrame();

		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);	// not sure what 2nd argument is here?
		bool window_val = true;
		ImGui::Begin("First Window Ever", &window_val);	// needs to accept a bool of true? hmmm ok
		ImGui::Text("Options:");
		ImGui::End();


		runPass1();
		glFlush();
		runPass2();

		// IMGUI render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}

void ShaderManagerDeferred2Debug::multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
{
	//updateMatricesForPass1();
	runPass1();
	glFlush();
	//std::cout << "Hey here we go..." << std::endl;
	runPass2();
}

void ShaderManagerDeferred2Debug::runPass1()
{
	//std::cout << "number of collections is " << in_numberOfCollections << "!! " << std::endl;
	//std::cout << "vertex count is: " << in_vertexCount[0] << std::endl;	// 1131
	glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1index);		// set appropriate variables for pass #1
	glBindVertexArray(terrainVaoID);								// bind to the terrain VAO
	setMatrices();
	//glMultiDrawArrays(GL_TRIANGLES, in_startArray, in_vertexCount, in_numberOfCollections);		// draw the terrain
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glFinish();
}

void ShaderManagerDeferred2Debug::runPass2()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	view = glm::mat4(1.0);
	model = glm::mat4(1.0);
	projection = glm::mat4(1.0);
	setMatrices();
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2index);		// set appropriate variables for pass #1
	glBindVertexArray(quadVaoID);
	glDrawArrays(GL_TRIANGLES, 0, 6);		// draw the quad
}

/*
void ShaderManagerDeferred2Debug::updateMatricesForPass1()
{
	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// projection matrix : 45° Field of view, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 400.0f);

	// Camera matrix
	view = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	model = glm::mat4(1.0);
	MVP = projection * view * model;
}
*/