#pragma once

#ifndef GLPROGRAMIDINDEX_H
#define GLPROGRAMIDINDEX_H

#include <GLFW/glfw3.h>

/*

Description: this class is designed for Gear instances that are used in shader machines, to allow them to switch between
different shader programs. For most Gear objects that only use a single shader program, the selectProgram() function below will suffice;
this function will return the "default" value located in programMap.

If an instantiated Gear only plans to use one shader program, simply call insertDefaultEntry when initializeMachineShader is called.
This MUST be done, otherwise selectProgram() will return a value of -1, which could be considered an error.

*/

class GLProgramIDIndex
{
	public:
		void insertDefaultEntry(GLuint in_programID);	// works for most Gear objects that only use one shader; must be called during initializeMachineShader.

		void insertIndexEntry(std::string in_programName, GLuint in_programID);	// manually inserts a program entry; for use by Gear objects that intend to use
																				// multiple shader programs.
		GLuint selectProgram();	// assuming there is one entry, will return that value; the value represents an ID assigned to an already createdProgram.
							    // otherwise, returns -1 to indicate a bad value.
	private:
		std::unordered_map<std::string, GLuint> programMap;	// a string-keyed map of IDs, where the IDs pertain to programs that have been created.
};

#endif