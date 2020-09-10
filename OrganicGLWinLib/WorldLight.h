#pragma once

#ifndef WORLDLIGHT_H
#define WORLDLIGHT_H

#include <glm/glm.hpp>

class WorldLight
{
	public:
		glm::vec3 position;
		glm::vec3 color;
		float linear = 0;
		float quadratic = 0;
		float radius = 0;
};

#endif
