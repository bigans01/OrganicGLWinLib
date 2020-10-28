#pragma once

#ifndef BORDERLINECYCLER_H
#define BORDERLINECYCLER_H

#include "SPoly.h"
#include "SPolyBorderLines.h"
#include "CategorizedLine.h"
#include "CyclingDirectionFinder.h"
#include "CyclingDirection.h"
#include "BorderLineCycle.h"
#include <chrono>
#include "MassManipulationMode.h"

class BorderLineCycler
{
	public:
		BorderLineCycler(SPoly* in_sPolyRef) : sPolyRef(in_sPolyRef) {};		// initializer list

		BorderLineCycle lineCycle;
		SPoly* sPolyRef;
		CyclingDirection direction = CyclingDirection::FORWARD;		// default is forward
		//CyclingDirection findCyclingDirection(SPolyBorderLines in_borderLineCopy, int in_borderLineIndex, CategorizedLine in_categorizedLineCopy, MassManipulationMode in_massManipulationMode);
		CyclingDirection findCyclingDirection(SPolyBorderLines in_borderLineCopy, int in_borderLineIndex, CategorizedLine in_categorizedLineCopy);
		void buildCycle(SPoly* in_sPolyRef, int in_borderLineStartIndex, int in_borderLineEndIndex);
};

#endif