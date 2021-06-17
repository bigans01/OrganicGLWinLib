#pragma once

#ifndef RMORPHABLEMESHCUBEAREADEFINER_H
#define RMORPHABLEMESHCUBEAREADEFINER_H

#include <glm/glm.hpp>

class RMorphableMeshCubeAreaDefiner
{
	public:
		RMorphableMeshCubeAreaDefiner() {};
		void setAreaDimensions(glm::vec3 in_cubeCenterPoint, float in_cubeDiameter)
		{
			float cubeHalfDiameter = in_cubeDiameter / 2;
			definingPointA = in_cubeCenterPoint;
			definingPointA -= cubeHalfDiameter;

			definingPointB = in_cubeCenterPoint;
			definingPointB += cubeHalfDiameter;
		}
		glm::vec3 definingPointA, definingPointB;
};

#endif
