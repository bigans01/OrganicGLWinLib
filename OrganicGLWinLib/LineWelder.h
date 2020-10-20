#pragma once

#ifndef LINEWELDER_H
#define LINEWELDER_H

#include "SPoly.h"
#include "BorderLineIDPair.h"
#include "MassManipulationMode.h"
#include "BorderLineCycler.h"

class LineWelder
{
public:
	LineWelder(SPoly* in_sPolyRef) : sPolyRef(in_sPolyRef) 
	{
		currentManipulationMode = sPolyRef->massManipulationSetting;	// set the manipulation mode.
		startWelding();
	};
private:
	SPoly* sPolyRef = nullptr;	// a reference to the SPoly we will be operating on for this LineWelder.
	MassManipulationMode currentManipulationMode = MassManipulationMode::CREATION;	// CREATION is default value, but will be overrriden by the SPoly's value when
																					// the LineWelder is constructed.

	int currentBorderLineID = 0;	// the border line we'll be starting on.
	int endingBorderLineID = 0;		// the border line we'll be ending on (the while loop will end when it hits this)
	void startWelding();
	void findWeldingLines(int in_startingBorderLine, int in_startingCleaveSequenceID, int in_categorizedLineInCleaveSequenceID, CyclingDirection in_cyclingDirection, BorderLinePointPair in_beginningPointPair);
};

#endif
