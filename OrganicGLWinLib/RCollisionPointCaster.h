#pragma once

#ifndef RCOLLISIONPOINTCASTER_H
#define RCOLLISIONPOINTCASTER_H

#include "RasterCubeTracerBase.h"
#include <iostream>
#include "MassGridArray.h"
#include "MassGridSearchResult.h"

class RCollisionPointCaster : public RasterCubeTracerBase
{
	public:
		void runTrace();
		void setGridArrayRef(MassGridArray* in_optionalMassGridArrayRef);
	private:
		MassGridArray* casterMassGridArrayRef = nullptr;
};

#endif
