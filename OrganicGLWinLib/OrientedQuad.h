#pragma once

#ifndef ORIENTEDQUAD_H
#define ORIENTEDQUAD_H

#include "OrientedQuadPoint.h"
#include "OrientedQuadQuadrant.h"
#include "OrientedQuadType.h"
#include <glm/glm.hpp>

class OrientedQuad
{
	public:
		OrientedQuad(glm::vec3 in_startPoint, OrientedQuadType in_quadType, float in_coreDim, glm::vec3 in_dir1, glm::vec3 in_dir2);
		OrientedQuadPoint quadPoints[4];
		OrientedQuadQuadrant quadQuadrant = OrientedQuadQuadrant::NONE;		// none for now, but set later
		
};

#endif
