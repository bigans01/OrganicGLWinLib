#pragma once

#ifndef BORDERLINECYCLER_H
#define BORDERLINECYCLER_H

#include "SPoly.h"
#include "SPolyBorderLines.h"
#include "CategorizedLine.h"
#include "CyclingDirectionFinder.h"

class BorderLineCycler
{
	public:
		BorderLineCycler(SPoly* in_sPolyRef) : sPolyRef(in_sPolyRef) {};		// initializer list

		SPoly* sPolyRef;
		void findCyclingDirection(SPolyBorderLines* in_borderLineRef, int in_borderLineIndex, CategorizedLine* in_categorizedLineRef);
};

#endif