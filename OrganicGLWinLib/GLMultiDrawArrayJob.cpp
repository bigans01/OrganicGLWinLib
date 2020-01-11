#include "stdafx.h"
#include "GLMultiDrawArrayJob.h"

GLMultiDrawArrayJob::GLMultiDrawArrayJob()
{

}

GLMultiDrawArrayJob::GLMultiDrawArrayJob(const GLMultiDrawArrayJob& job_b)
{
	//std::cout << "Return constructor called" << std::endl;
	//std::cout << "!! Original draw count value: " << drawCount << std::endl;
	drawJobBufferID = job_b.drawJobBufferID;	// copy the buffer ID
	drawCount = job_b.drawCount;


	if (drawCount > 0)							// only do the below if there is data to copy.
	{
		multiStartIndices.reset(new GLint[drawCount]);
		multiVertexCount.reset(new GLint[drawCount]);
		for (int x = 0; x < drawCount; x++)
		{
			multiStartIndices[x] = job_b.multiStartIndices[x];
			multiVertexCount[x] = job_b.multiVertexCount[x];

			//std::cout << "------- Copied value for start indices is: " << multiStartIndices[x] << std::endl;
			//std::cout << "------- Copied value for vertex count is: " << multiVertexCount[x] << std::endl;
		}
	}
	else if (drawCount == 0)
	{
		//std::cout << "!!! Count was empty, resetting unique_ptrs..." << std::endl;
		multiStartIndices.reset();
		multiVertexCount.reset();
	}
	//std::cout << "!! New draw count value: " << drawCount << std::endl;
}

void GLMultiDrawArrayJob::updateDrawArrayData(GLuint in_drawJobBufferID, GLint* in_multiStartIndicesData, GLsizei* in_multiVertexCountData, int in_drawCount)
{
	drawJobBufferID = in_drawJobBufferID;
	drawCount = in_drawCount;
	//std::cout << ">>>> new draw count will be: " << drawCount << std::endl;
	multiStartIndices.reset(new GLint[drawCount]);
	multiVertexCount.reset(new GLint[drawCount]);
	for (int x = 0; x < drawCount; x++)
	{
		multiStartIndices[x] = in_multiStartIndicesData[x];
		multiVertexCount[x] = in_multiVertexCountData[x];
	}
}