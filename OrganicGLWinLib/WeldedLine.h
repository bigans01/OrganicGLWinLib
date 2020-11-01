#pragma once

#ifndef WELDEDLINE_H
#define WELDEDLINE_H

#include <glm/common.hpp>

class WeldedLine
{
	public:
		WeldedLine() {};
		WeldedLine(glm::vec3 in_pointA, glm::vec3 in_pointB) :
			pointA(in_pointA),
			pointB(in_pointB)
		{};
		glm::vec3 pointA, pointB;
};

#endif
