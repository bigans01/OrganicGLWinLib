#pragma once

#ifndef GLMULTIDRAWARRAYPARAMS_H
#define GLMULTIDRAWARRAYPARAMS_H

#include <GLFW/glfw3.h>

class GLMultiDrawArrayParams
{
	public:
		GLMultiDrawArrayParams() {};
		GLMultiDrawArrayParams(GLint in_vertexStart, GLint in_numberOfVertices, int in_arraySize, int in_totalFloats) :
			vertexStart(in_vertexStart),
			numberOfVertices(in_numberOfVertices),
			arraySize(in_arraySize),
			totalFloats(in_totalFloats)
		{};
		GLint vertexStart = 0;
		GLint numberOfVertices = 0;
		int arraySize = 1;
		int totalFloats = 0;
};

#endif
