#pragma once

#ifndef RAY
#define RAY

#include <glm/glm.hpp>

class Ray
{
	public:
		Ray() {};
		Ray(glm::vec3 in_beginPoint, glm::vec3 in_rayDirection)
		{
			raySegmentBeginPoint = in_beginPoint;
			raySegmentEndPoint = (raySegmentBeginPoint + in_rayDirection);
		}
		glm::vec3 raySegmentBeginPoint, raySegmentEndPoint;
};

#endif
