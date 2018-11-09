#include "stdafx.h"
#include "OrganicGLWinUtils.h"

void OrganicGLWinUtils::createImmutableBufferMode1(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers)
{
	glGenBuffers(1, in_bufferID);					// generate the buffer
	glBindBuffer(GL_ARRAY_BUFFER, *in_bufferID);	// bind the buffer
	const GLbitfield bufferStorageFlags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// set mandatory flags
	glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize*in_numberOfBuffers, NULL, bufferStorageFlags);	// allocate immutable buffer

	// multiple attributes per vertex array means its not tightly packed, so 5th parameter represents the byte offset between consecutive attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)0);		
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)12);
}

void OrganicGLWinUtils::createImmutableBufferMode0(GLuint* in_bufferID, int in_bufferSize, int in_numberOfBuffers)
{
	glGenBuffers(1, in_bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, *in_bufferID);
	const GLbitfield bufferStorageFlags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;	// set mandatory flags
	glBufferStorage(GL_ARRAY_BUFFER, in_bufferSize*in_numberOfBuffers, NULL, bufferStorageFlags);	// allocate immutable buffer
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