#pragma once

#ifndef COPLANARMASSCREATOR_H
#define COPLANARMASSCREATOR_H

#include "CoplanarMassManipulator.h"
#include "CategorizedLinePool.h"
#include "SPolyMorphTracker.h"
#include "SPolyFracturer.h"
#include "STriangleCutter.h"
#include "CuttingTriangleManager.h"
#include "STriangleCutter.h"
#include "ExceptionRecorder.h"

class CoplanarMassCreator : public CoplanarMassManipulator
{
	public:
		void runMassManipulation();
	private:
};

#endif
