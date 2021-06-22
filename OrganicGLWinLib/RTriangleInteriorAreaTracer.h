#pragma once

#ifndef RTRIANGLEINTERIORAREATRACER_H
#define RTRIANGLEINTERIORAREATRACER_H

#include "MassGridArray.h"
#include "RasterCubeTracerBase.h"

class RTriangleInteriorAreaTracer : public RasterCubeTracerBase
{
	public:
		void setGridArrayRef(MassGridArray* in_massGridArrayRef);
		void runTrace();
	private:
		MassGridArray* gridArrayRef = nullptr;

};

#endif
