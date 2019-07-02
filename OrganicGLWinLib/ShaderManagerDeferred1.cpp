#include "stdafx.h"
#include "ShaderManagerDeferred1.h"

void ShaderManagerDeferred1::initialize(int in_windowWidth, int in_windowHeight)
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
	OrganicGLWinUtils::loadShadersViaMode(&programID, 4);

	// NEW ---> setup IMGui
	OrganicGLWinUtils::IMGuiInit(window);


	// select the program for use
	glUseProgram(programID);

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	setupRenderQuad();
	setupFBO();
	setupTestTriangle();
	acquireSubroutineIndices();
}

void ShaderManagerDeferred1::shutdownGL()
{

}

void ShaderManagerDeferred1::initializeShader(int in_windowWidth, int in_windowHeight, int in_terrainBufferSize)
{

}

void ShaderManagerDeferred1::setupRenderQuad()
{
	OrganicGLWinUtils::createAndBindVertexArray(&quadVaoID);	// create/bind the VAO to quadVaoID
	glGenBuffers(1, &quadBufferID);					// generate the buffer
	glBindBuffer(GL_ARRAY_BUFFER, quadBufferID);	// bind

	GLfloat quadData[] =
	{												// first 3 floats = position, 4th and 5th = texture coords. 
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,				// 1st point, lower left
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,				// 2nd point, lower right
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,				// 3rd point, upper right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,				// 4th point, lower left
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,				// 5th point, upper right
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f				// 6th point, upper left
	};

	glBufferData(GL_ARRAY_BUFFER, 6 * 5 * sizeof(float), quadData, GL_STATIC_DRAW);		// populate the data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);		// set pointer for position
	glEnableVertexAttribArray(0);														// enable position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)12);	// set pointer for UV
	glEnableVertexAttribArray(1);														// enable UV
	glBindVertexArray(0);		// bind back to default (may not matter anyway)
}

void ShaderManagerDeferred1::setupFBO()
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
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShaderManagerDeferred1::setupTestTriangle()
{
	OrganicGLWinUtils::createAndBindVertexArray(&testTriangleVaoID);
	glGenBuffers(1, &testTriangleBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, testTriangleBufferID);

	GLfloat testTriangleData[] =
	{
		-2.0f, -2.0f, 0.0f, 0.0f, 0.0f,	// lower left
		2.0f, -2.0f, 0.0f, 1.0f, 0.0f,	// lower right
		-2.0f, 2.0f, 0.0f, 0.0f, 1.0f	// uper left
	};
	glBufferData(GL_ARRAY_BUFFER, 3 * 5 * sizeof(float), testTriangleData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);		// set pointer for position
	glEnableVertexAttribArray(0);														// enable position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)12);	// set pointer for UV
	glEnableVertexAttribArray(1);

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

void ShaderManagerDeferred1::acquireSubroutineIndices()
{
	pass1index = glGetSubroutineIndex(programID, GL_FRAGMENT_SHADER, "pass1");
	pass2index = glGetSubroutineIndex(programID, GL_FRAGMENT_SHADER, "pass2");
}

void ShaderManagerDeferred1::setupImGui()
{
	// NEW EXPERIMENTAL IMGUI CODE goes here
	const char* glsl_version = "#version 130";
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);		// new version of init for openGL?
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void ShaderManagerDeferred1::createGBufText(GLenum texUnit, GLenum  format, GLuint &texid)
{
	glActiveTexture(texUnit);
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void ShaderManagerDeferred1::setMatrices()
{
	MVP = projection * view * model;
	mvpHandle = glGetUniformLocation(programID, "MVP");	// find the MVP uniform
	glUniformMatrix4fv(mvpHandle, 1, GL_FALSE, &MVP[0][0]);		// set the uniform

	glm::mat4 mv = view * model;
	modelViewHandle = glGetUniformLocation(programID, "ModelViewMatrix");
	glUniformMatrix4fv(modelViewHandle, 1, GL_FALSE, &mv[0][0]);
}

void ShaderManagerDeferred1::computeFromInputs()
{
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	int isFocused = glfwGetWindowAttrib(window, GLFW_FOCUSED);

	if (isFocused == 1)
	{
		glfwGetCursorPos(window, &xpos, &ypos);

		// Reset mouse position for next frame
		glfwSetCursorPos(window, width / 2, height / 2);





		//std::cout << "OpenGL is FOCUSED!" << std::endl;


		// Compute new orientation
		horizontalAngle += mouseSpeed * float(width / 2 - xpos);
		verticalAngle += mouseSpeed * float(height / 2 - ypos);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);
	}

	//std::cout << "Direction vector: " << direction.x << ", " << direction.y << ", " << direction.z << endl;

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	if (isFocused == 1)
	{
		// Move forward
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			position += direction * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			position -= direction * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			position += right * deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			position -= right * deltaTime * speed;
		}

	}

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


	lastTime = currentTime;
}

void ShaderManagerDeferred1::runPass1()
{
	glBindFramebuffer(GL_FRAMEBUFFER, deferredFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1index);		// set appropriate variables for pass #1
	glBindVertexArray(testTriangleVaoID);										// bind to the basic triangle to draw
	setMatrices();
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glFinish();
}

void ShaderManagerDeferred1::runPass2()
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

void ShaderManagerDeferred1::render()
{
	do {
		computeMatricesFromInputs();
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
	}
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}

void ShaderManagerDeferred1::multiDrawTerrain(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, int in_numberOfCollections)
{

}