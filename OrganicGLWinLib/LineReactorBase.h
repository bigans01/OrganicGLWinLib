#pragma once

#ifndef LINEREACTORBASE_H
#define LINEREACTORBASE_H

#include "CategorizedLine.h"
#include "FusionAnalysis.h"
#include "FusedPointMetaPair.h"
#include "PolyLogger.h"

class LineReactorBase
{
	public:
		CategorizedLine resultantLine;
		PolyLogger reactorBaseLogger;

};

#endif
