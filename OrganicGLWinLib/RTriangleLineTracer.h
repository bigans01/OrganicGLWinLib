#pragma once

#ifndef RTRIANGLELINETRACER_H
#define RTRIANGLELINETRACER_H

#include "RasterCubeTracerBase.h"
#include <iostream>

class RTriangleLineTracer : public RasterCubeTracerBase
{
	public:
		void runTrace();
};

#endif
