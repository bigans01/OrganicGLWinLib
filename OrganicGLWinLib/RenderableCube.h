#pragma once

#ifndef RENDERABLECUBE_H
#define RENDERABLECUBE_H

#include "RenderablePrimitiveBase.h"

class RenderableCube : public RenderablePrimitiveBase
{
	public:
		void buildTriangles();
	private:
		glm::vec3 lowerNE;
		glm::vec3 lowerSE;
		glm::vec3 lowerSW;
		glm::vec3 lowerNW;
		glm::vec3 upperNE;
		glm::vec3 upperSE;
		glm::vec3 upperSW;
		glm::vec3 upperNW;

		void setupCorners();

};

#endif
