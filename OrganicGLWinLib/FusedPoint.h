#pragma once

#ifndef FUSEDPOINT_H
#define FUSEDPOINT_H

#include <vector>
#include "FusedPointSubData.h"
#include <glm/glm.hpp>
#include <iostream>

class FusedPoint
{
	public:
		glm::vec3 point;
		bool doesPointMatch(glm::vec3 in_pointToMatchAgainst);
		void insertFusedPointSubData(FusedPointSubData in_fusedPointSubData);
		void printData();
	private:
		std::vector<FusedPointSubData> subDataVector;
};

#endif
