#pragma once

#ifndef ORIENTEDQUAD_H
#define ORIENTEDQUAD_H

#include "OrientedQuadPoint.h"
#include "OrientedQuadQuadrant.h"
#include "OrientedQuadType.h"
#include "OrganicGLWinUtils.h"
#include "QuadTriangles.h"

class OrientedQuad
{
	public:
		OrientedQuad(glm::vec3 in_startPoint, float in_coreDim, glm::vec3 in_dir1, glm::vec3 in_dir2);		// square constructor
		OrientedQuad(glm::vec3 in_startPoint, float in_widthDim, float in_lengthDim, glm::vec3 in_lengthDir, glm::vec3 in_widthDir);
		OrientedQuadPoint quadPoints[4];
		OrientedQuadQuadrant quadQuadrant = OrientedQuadQuadrant::NONE;		// none for now, but set later
		OrientedQuadType quadType = OrientedQuadType::NONE;					// none is default
		
		glm::vec3* getPointRef(int in_pointIndex);
		glm::vec3 getPoint(int in_pointIndex);
		QuadTriangles getQuadTriangles();
};

#endif
