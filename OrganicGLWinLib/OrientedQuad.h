#pragma once

#ifndef ORIENTEDQUAD_H
#define ORIENTEDQUAD_H

#include "OrientedQuadPoint.h"
#include "OrientedQuadQuadrant.h"

class OrientedQuad
{
	public:
		OrientedQuadPoint quadPoints[4];
		OrientedQuadQuadrant quadQuadrant = OrientedQuadQuadrant::NONE;		// none for now, but set later
		
};

#endif
