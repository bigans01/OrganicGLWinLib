#pragma once

#ifndef BORDERLINEPOINTPAIR_H
#define BORDERLINEPOINTPAIR_H

#include <glm/glm.hpp>

class BorderLinePointPair
{
	public:
		BorderLinePointPair() {};
		BorderLinePointPair(glm::vec3 in_pointA, glm::vec3 in_pointB) : pointA(in_pointA), pointB(in_pointB) {}
		glm::vec3 pointA, pointB;
};

#endif
