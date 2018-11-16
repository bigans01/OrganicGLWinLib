#include "stdafx.h"
#include "OrganicGLWinUtils.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void OrganicGLWinUtils::createImmutableBufferMode1(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers)
{
	glGenBuffers(1, in_bufferID);					// generate the buffer
	glBindBuffer(GL_ARRAY_BUFFER, *in_bufferID);	// bind the buffer
	const GLbitfield bufferStorageFlags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// set mandatory flags
	glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize*in_numberOfBuffers, NULL, bufferStorageFlags);	// allocate immutable buffer

	// multiple attributes per vertex array means its not tightly packed, so 5th parameter represents the byte offset between consecutive attributes
	/*
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);		
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)12);
	*/
}

void OrganicGLWinUtils::createImmutableBufferMode0(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers)
{
	glGenBuffers(1, in_bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, *in_bufferID);
	const GLbitfield bufferStorageFlags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// set mandatory flags
	glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize*in_numberOfBuffers, NULL, bufferStorageFlags);	// allocate immutable buffer

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
}

void OrganicGLWinUtils::createImmutableBufferMode2(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers, GLuint* in_textureRef)
{
	glGenBuffers(1, in_bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, *in_bufferID);
	const GLbitfield bufferStorageFlags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// set mandatory flags
	glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize*in_numberOfBuffers, NULL, bufferStorageFlags);	// allocate immutable buffer

	
	
	// create texture buffers
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

	/*
	in_windowPtr = glfwCreateWindow(in_windowWidth, in_windowHeight, "Organic", NULL, NULL);	// window dimensions
	if (in_windowPtr == NULL)	// check if window was successfully created, and terminate otherwise
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
	}

	glfwMakeContextCurrent(in_windowPtr);	// set the current context

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "----------------Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		//return -1;
	}
	*/
	//glEnable(GL_DEPTH_TEST); // Enable depth test
	//glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one
	//glfwSetInputMode(in_windowPtr, GLFW_STICKY_KEYS, GL_TRUE);	// Ensure we can capture the escape key being pressed
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
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride = 0 (tightly packed); bytes offset between consecutive generic vertex attributes is 0.
		(void*)0            /* array buffer offset. Number following (void*) indicates offset point to begin reading from in the pointed-to buffer, measured in bytes;
							For instance, if the data begins at byte 10000, you would put (void*)10000 in the array you are reading.
							*/
	);
	//glBindBuffer(GL_DRAW_INDIRECT_BUFFER, *in_indirectBufferID);						// switch to indirect draw buffer
	glMultiDrawArrays(GL_TRIANGLES, in_startArray, in_vertexCount, in_numberOfCollections);		// perform the draw

	glBindBuffer(GL_ARRAY_BUFFER, *in_drawArrayID);										// switch back to the draw array, to disable vertex attributes
	glDisableVertexAttribArray(0);
}

void OrganicGLWinUtils::multiDrawArraysMode1(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, int in_numberOfCollections)
{
	glm::mat4 MVPref = *in_MVPmat4ref;	// send updated MVP transform to shader
	glUniformMatrix4fv(*in_MVPuniformLocation, 1, GL_FALSE, &MVPref[0][0]);
	glBindBuffer(GL_ARRAY_BUFFER, *in_drawArrayID);		// bind to the draw array, to setup vertex attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);		// First attribute: a vec3 representing the point data, before it is translated by MVP.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)12);    // Second attribute: a vec3 representing the output color.
	glMultiDrawArrays(GL_TRIANGLES, in_startArray, in_vertexCount, in_numberOfCollections);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void OrganicGLWinUtils::multiDrawArraysMode2(GLuint* in_drawArrayID, GLint* in_startArray, GLsizei* in_vertexCount, GLuint* in_MVPuniformLocation, glm::mat4* in_MVPmat4ref, GLuint* in_textureRef, GLuint* in_textureUniformRef, int in_numberOfCollections)
{
	glm::mat4 MVPref = *in_MVPmat4ref;	// send updated MVP transform to shader
	glUniformMatrix4fv(*in_MVPuniformLocation, 1, GL_FALSE, &MVPref[0][0]);

	glActiveTexture(GL_TEXTURE0);	// send updated texture uniform to shader
	glBindTexture(GL_TEXTURE_2D, *in_textureRef);
	glUniform1i(*in_textureUniformRef, 0);

	glBindBuffer(GL_ARRAY_BUFFER, *in_drawArrayID);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)12);
	glMultiDrawArrays(GL_TRIANGLES, in_startArray, in_vertexCount, in_numberOfCollections);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}