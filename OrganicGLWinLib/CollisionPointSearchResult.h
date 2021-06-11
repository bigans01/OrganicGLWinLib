#pragma once

#ifndef COLLISIONPOINTSEARCHRESULT_H
#define COLLISIONPOINTSEARCHRESULT_H

#include <glm/glm.hpp>

class CollisionPointSearchResult
{
	public:
		CollisionPointSearchResult() {};
		CollisionPointSearchResult(bool in_wasFound, glm::vec3 in_point) :
			wasPointFound(in_wasFound),
			point(in_point)
		{};
		bool wasPointFound = false;
		glm::vec3 point;
};

#endif


