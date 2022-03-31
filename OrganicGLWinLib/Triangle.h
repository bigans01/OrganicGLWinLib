#pragma once

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include <iostream>

class Triangle
{
	public:
		Triangle() {};
		Triangle(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2)
		{
			points[0] = in_point0;
			points[1] = in_point1;
			points[2] = in_point2;
		}
		glm::vec3 points[3];
		void printPoints()
		{
			for (int x = 0; x < 3; x++)
			{
				std::cout << "(" << points[x].x << ", " << points[x].y << ", " << points[x].z << ")";
			}
		}
};

#endif