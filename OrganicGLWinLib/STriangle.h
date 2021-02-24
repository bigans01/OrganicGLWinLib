#pragma once

#ifndef STRIANGLE_H
#define STRIANGLE_H

#include "STriangleLine.h"
#include <glm/glm.hpp>
#include <iostream>

class STriangle
{
public:
	STriangle(glm::vec3 in_pointA, glm::vec3 in_pointB, glm::vec3 in_pointC);
	STriangle();
	STriangleLine triangleLines[3];	// the lines of the STriangle
	void printPoints()
	{
		std::cout << "!! Printing points of STriangle. " << std::endl;
		for (int x = 0; x < 3; x++)
		{
			std::cout << "point " << x << ": " << triangleLines[x].pointA.x << ", " << triangleLines[x].pointA.y << ", " << triangleLines[x].pointA.z << std::endl;
		}
	}
};

#endif
