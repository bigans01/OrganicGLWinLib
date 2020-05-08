#pragma once

#ifndef STRIANGLE_H
#define STRIANGLE_H

#include "STriangleLine.h"
#include <glm/glm.hpp>

class STriangle
{
public:
	STriangle(glm::vec3 in_pointA, glm::vec3 in_pointB, glm::vec3 in_pointC);
	STriangle();
	STriangleLine triangleLines[3];	// the lines of the STriangle
};

#endif
