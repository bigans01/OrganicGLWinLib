#include "stdafx.h"
#include "ShaderBase.h"

double ShaderBase::lastTime = glfwGetTime();
GLuint* ShaderBase::getTerrainBufferID()
{
	return &terrainBufferID;
}

GLuint* ShaderBase::getTerrainSwapID()
{
	return &terrainSwapID;
}

void ShaderBase::computeMatricesFromInputs()
{
	// glfwGetTime is called only once, the first time this function is called
	//static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);

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
	up = glm::cross(right, direction);

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
}

void ShaderBase::updateMatricesAndDelta()
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


	lastTime = currentTime;
}

GLFWwindow* ShaderBase::getWindow()
{
	return window;
}

void ShaderBase::setupTextureAtlas(AtlasMap* in_atlasMapRef, AtlasPropertiesGL* in_atlasPropertiesGLRef)
{
	OrganicGLWinUtils::setupTextureAtlasJPEG(&textID, in_atlasMapRef, in_atlasPropertiesGLRef, &atlasTileWidth, &atlasWidth);
}

int ShaderBase::getBufferSize()
{
	return OGLMVertexSubBufferSize;
}

glm::vec3* ShaderBase::getPosition()
{
	return &position;
}
glm::vec3* ShaderBase::getDirection()
{
	return &direction;
}