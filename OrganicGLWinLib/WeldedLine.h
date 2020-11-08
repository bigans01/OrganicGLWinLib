#pragma once

#ifndef WELDEDLINE_H
#define WELDEDLINE_H

#include <glm/common.hpp>

class WeldedLine
{
	public:
		WeldedLine() {};
		WeldedLine(glm::vec3 in_pointA, glm::vec3 in_pointB, glm::vec3 in_emptyNormal) :
			pointA(in_pointA),
			pointB(in_pointB),
			emptyNormal(in_emptyNormal)
		{};
		void swapPointsAndInvertNormal()
		{
			glm::vec3 tempPointA = pointA;				// save values of A, since it's being overwrriten
			pointA = pointB;							// overwrite A with B
			pointB = tempPointA;
			emptyNormal *= -1.0f;
		}
		glm::vec3 pointA, pointB, emptyNormal;
};

#endif
