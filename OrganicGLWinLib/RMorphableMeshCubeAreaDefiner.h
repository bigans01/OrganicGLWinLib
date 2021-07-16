#pragma once

#ifndef RMORPHABLEMESHCUBEAREADEFINER_H
#define RMORPHABLEMESHCUBEAREADEFINER_H

#include <glm/glm.hpp>
#include "RMorphableMeshPointRequests.h"

class RMorphableMeshCubeAreaDefiner
{
	public:
		RMorphableMeshCubeAreaDefiner() {};
		void setAreaDimensions(glm::vec3 in_cubeCenterPoint, float in_cubeDiameter)
		{
			cubeDiameter = in_cubeDiameter;
			float cubeHalfDiameter = in_cubeDiameter / 2;
			definingPointA = in_cubeCenterPoint;
			definingPointA -= cubeHalfDiameter;		// lower SW

			definingPointB = in_cubeCenterPoint;
			definingPointB += cubeHalfDiameter;		// upper NE
		}
		glm::vec3 definingPointA, definingPointB;

		RMorphableMeshPointRequests getPointRequests()
		{
			RMorphableMeshPointRequests returnRequests;

			// initialize all points to contain definingPointA, before any changes occur.
			for (int x = 0; x < 8; x++)
			{
				returnRequests.points[x] = definingPointA;
			}

															// lower NW (no changes)

			returnRequests.points[1].z += cubeDiameter;		// lower SW

			returnRequests.points[2].x += cubeDiameter;		// lower SE
			returnRequests.points[2].z += cubeDiameter;

			returnRequests.points[3].x += cubeDiameter;		// lower NE

			returnRequests.points[4].y += cubeDiameter;		// upper NW

			returnRequests.points[5].y += cubeDiameter;
			returnRequests.points[5].z += cubeDiameter;		// upper SW

			returnRequests.points[6].x += cubeDiameter;		// upper SE
			returnRequests.points[6].y += cubeDiameter;
			returnRequests.points[6].z += cubeDiameter;		

			returnRequests.points[7].x += cubeDiameter;		// upper NE
			returnRequests.points[7].y += cubeDiameter;


			return returnRequests;
		};
	private:
		float cubeDiameter = 0.0f;
};

#endif
