#pragma once

#ifndef RCOLLISIONPOINTCASTER_H
#define RCOLLISIONPOINTCASTER_H

#include "RasterCubeTracerBase.h"
#include <iostream>
#include "MassGridArray.h"
#include "MassGridSearchResult.h"
#include "RUtils.h"
#include "RCollisionPoint.h"

class RCollisionPointCaster : public RasterCubeTracerBase
{
	public:
		void runTrace();
		void setGridArrayRef(MassGridArray* in_optionalMassGridArrayRef);
		void setPointToMoveRef(RCollisionPoint* in_destiantionPointRef);
		glm::vec3 getConvertedTraceEndPoint();
	private:
		MassGridArray* casterMassGridArrayRef = nullptr;
		RCollisionPoint* pointToMoveRef = nullptr;
};

#endif
