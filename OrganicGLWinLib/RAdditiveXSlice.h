#pragma once

#ifndef RADDITIVEXSLICE_H
#define RADDITIVEXSLICE_H

#include "RAdditiveSliceBase.h"

class RAdditiveXSlice : public RAdditiveSliceBase
{
	public:
		void buildInitialPointSets();
		void buildPointSets();
		void buildPTriangles();
	private:
		int yLimitMin = 1000;
		int yLimitMax = 0;

		int zLimitMin = 1000;
		int zLimitMax = 0;

		void generateSetBRCollisionPoints(int in_numberOfPointsBetweenCorners,
			glm::vec3 in_corner0,
			glm::vec3 in_corner1,
			glm::vec3 in_corner2,
			glm::vec3 in_corner3);

};

#endif
