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
			definingPointA += in_cubeDiameter;

			definingPointB = in_cubeCenterPoint;
			definingPointB -= in_cubeDiameter;
		}
		glm::vec3 definingPointA, definingPointB;
};

#endif
