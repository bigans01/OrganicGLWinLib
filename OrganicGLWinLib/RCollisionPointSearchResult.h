#pragma once

#ifndef RCollisionPointSEARCHRESULT_H
#define RCollisionPointSEARCHRESULT_H

#include <glm/glm.hpp>

class RCollisionPointSearchResult
{
	public:
		RCollisionPointSearchResult() {};
		RCollisionPointSearchResult(bool in_wasFound, glm::vec3 in_point) :
			wasPointFound(in_wasFound),
			point(in_point)
		{};
		bool wasPointFound = false;
		glm::vec3 point;
};

#endif


