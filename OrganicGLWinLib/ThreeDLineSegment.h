#pragma once

#ifndef THREEDLINESEGMENT_H
#define	THREEDLINESEGMENT_H

#include <glm/glm.hpp>

class ThreeDLineSegment
{

	public:
		ThreeDLineSegment() {};
		ThreeDLineSegment(glm::vec3 in_pointA, glm::vec3 in_pointB) :
			pointA(in_pointA),
			pointB(in_pointB)
		{};
		glm::vec3 pointA, pointB;
};

#endif
