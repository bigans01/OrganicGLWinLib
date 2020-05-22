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

class BorderLineCycler
{
	public:
		BorderLineCycler(SPoly* in_sPolyRef) : sPolyRef(in_sPolyRef) {};		// initializer list

		BorderLineCycle lineCycle;
		SPoly* sPolyRef;
		CyclingDirection direction = CyclingDirection::FORWARD;		// default is forward
		void findCyclingDirection(SPolyBorderLines* in_borderLineRef, int in_borderLineIndex, CategorizedLine* in_categorizedLineRef);
		void buildCycle(SPoly* in_sPolyRef, int in_borderLineStartIndex, int in_borderLineEndIndex);
};

#endif