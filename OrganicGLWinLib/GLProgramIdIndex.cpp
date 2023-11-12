#include "stdafx.h"
#include "GLProgramIDIndex.h"

void GLProgramIDIndex::insertDefaultEntry(GLuint in_programID)
{
	programMap["default"] = in_programID;
}

void GLProgramIDIndex::insertIndexEntry(std::string in_programName, GLuint in_programID)
{
	programMap[in_programName] = in_programID;
}

GLuint GLProgramIDIndex::selectProgram()
{
	GLuint discoveredID = -1;
	if (!programMap.empty())
	{
		discoveredID = programMap.begin()->second;
	}
	return discoveredID;
}