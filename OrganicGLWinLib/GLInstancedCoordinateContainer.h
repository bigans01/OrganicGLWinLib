#pragma once

#ifndef GLINSTANCEDCOORDINATECONTAINER_H
#define GLINSTANCEDCOORDINATECONTAINER_H

//#define GLFW_DLL		// only used when linking to a DLL version of GLFW.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <mutex>

class GLInstancedCoordinateContainer
{
	public:
		std::vector<glm::mat4> coordinateMatrices;
		std::unique_ptr<glm::mat4[]> mat4array;
		void insertNewInstancedCoordinate(glm::vec3 in_vec3)
		{
			glm::mat4 newMat = glm::mat4(1.0f);
			newMat = glm::translate(newMat, in_vec3);
			coordinateMatrices.push_back(newMat);
		}

		void buildArray()
		{
			mat4array.reset(new glm::mat4[coordinateMatrices.size()]);
			auto coordinatesBegin = coordinateMatrices.begin();
			auto coordinatesEnd = coordinateMatrices.end();
			int currentIndex = 0;
			for (coordinatesBegin; coordinatesBegin != coordinatesEnd; coordinatesBegin++)
			{
				mat4array[currentIndex] = *coordinatesBegin;
				currentIndex++;
			}
		}

		int getContainerSize() {
			return coordinateMatrices.size();
		};
};

#endif
