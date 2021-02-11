#pragma once

#ifndef CUTTRIANGLEGROUPBUILDER_H
#define CUTTRIANGLEGROUPBUILDER_H

#include "PolyDebugLevel.h"
#include "PolyLogger.h"
#include "CutLinePool.h"

class CutTriangleGroupBuilder
{
	public:
		CutTriangleGroupBuilder(PolyDebugLevel in_polyDebugLevel, CutLinePool in_cutLinePool) :
			cutTriangleGroupBuilderLoggerLogLevel(in_polyDebugLevel),
			linePool(in_cutLinePool)
		{
			cutTriangleGroupBuilderLogger.setDebugLevel(in_polyDebugLevel);
		};
	private:
		CutLinePool linePool;
		PolyLogger cutTriangleGroupBuilderLogger;
		PolyDebugLevel cutTriangleGroupBuilderLoggerLogLevel = PolyDebugLevel::NONE;
};

#endif
