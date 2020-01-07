#pragma once
#ifndef GLMULTIDRAWARRAYJOB_H
#define GLMULTIDRAWARRAYJOB_H


#include <GL/glew.h>
//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <GLFW/glfw3.h>
#include <mutex>
#include <iostream>

class GLMultiDrawArrayJob
{
	public:

		GLMultiDrawArrayJob();											// required, because we use unique_ptr in this class
		GLMultiDrawArrayJob(const GLMultiDrawArrayJob& in_jobOrig);		// required, for when returning an instance of this class -- for example, as a return value for a function (again, because we unique_ptr)

		GLuint drawJobBufferID = 0;		// the ID of the OpenGL buffer that this draw will be run on
		GLMultiDrawArrayJob& operator=(const GLMultiDrawArrayJob& job_b)		// for copying one to the other
		{

			//std::cout << "!! Original draw count value: " << drawCount << std::endl;
			std::cout << "!! Calling operator 1" << std::endl;
			drawJobBufferID = job_b.drawJobBufferID;	// copy the buffer ID
			std::cout << "!!!! Old draw count: " << drawCount << std::endl;
			drawCount = job_b.drawCount;
			std::cout << "!!!! New draw count: " << drawCount << std::endl;

			if (drawCount > 0)							// only do the below if there is data to copy.
			{
				multiStartIndices.reset(new GLint[drawCount]);
				multiVertexCount.reset(new GLint[drawCount]);
				/*
				for (int x = 0; x < drawCount; x++)
				{
					multiStartIndices[x] = job_b.multiStartIndices[x];
					multiVertexCount[x] = job_b.multiVertexCount[x];
				}
				*/
			}
			else if (drawCount == 0)
			{
				//std::cout << "!!! Count was empty, resetting unique_ptrs..." << std::endl;
				multiStartIndices.reset();
				multiVertexCount.reset();
			}
			std::cout << "!! End Calling operator 1" << std::endl;
			return *this;
		}

		/*
		GLMultiDrawArrayJob& operator=(GLMultiDrawArrayJob&& job_b)
		{

			//std::cout << "!! Original draw count value: " << drawCount << std::endl;
			std::cout << "!! Calling operator 2" << std::endl;
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
				}
			}
			else if (drawCount == 0)
			{
				//std::cout << "!!! Count was empty, resetting unique_ptrs..." << std::endl;
				multiStartIndices.reset();
				multiVertexCount.reset();
			}
			
			multiStartIndices = std::move(job_b.multiStartIndices);
			multiVertexCount = std::move(job_b.multiVertexCount);
			return *this;
		}
		*/
		

		std::unique_ptr<GLint[]> multiStartIndices;
		std::unique_ptr<GLsizei[]> multiVertexCount;
		int drawCount = 0;

		void updateDrawArrayData(GLuint in_drawJobBufferID, GLint* in_multiStartIndicesData, GLsizei* in_multiVertexCountData, int in_drawCount);
};

#endif
