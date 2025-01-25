#pragma once

#ifndef GLBASICPROGRAMDATA_H
#define GLBASICPROGRAMDATA_H

/*
* 
* Description: a basic data structure that contains the ID of the vertex shader, fragment shader, and program used when creating
* a program with 

*/

class GLBasicProgramData
{
	public:
		GLBasicProgramData() {};
		GLBasicProgramData(GLuint in_vertexShaderId, GLuint in_fragmentShaderId, GLuint in_programId) :
			vertexShaderId(in_vertexShaderId),
			fragmentShaderId(in_fragmentShaderId),
			programId(in_programId) {};

		GLuint vertexShaderId = 0;
		GLuint fragmentShaderId = 0;
		GLuint programId = 0;
};

#endif
