#pragma once

#ifndef BORDERLINEDATA_H
#define BORDERLINEDATA_H

#include <glm/glm.hpp>

class BorderLineData
{
	public:
		BorderLineData() {};
		BorderLineData(glm::vec3 in_point, int in_borderLineID) :
			point(in_point),
			borderLineID(in_borderLineID)
		{};
		glm::vec3 point;
		int borderLineID = 0;
};

#endif
