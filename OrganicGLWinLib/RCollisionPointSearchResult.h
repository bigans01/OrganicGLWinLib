#pragma once

#ifndef RCollisionPointSEARCHRESULT_H
#define RCollisionPointSEARCHRESULT_H

#include <glm/glm.hpp>

class RCollisionPointSearchResult
{
	public:
		RCollisionPointSearchResult() {};
		RCollisionPointSearchResult(bool in_wasFound, int in_arrayIndex, glm::vec3 in_point) :
			wasPointFound(in_wasFound),
			arrayIndex(in_arrayIndex),
			point(in_point)
		{};
		bool wasPointFound = false;
		int arrayIndex = 0;
		glm::vec3 point;
};

#endif


