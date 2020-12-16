#pragma once

#ifndef CATEGORIZEDLINECOLINEARTESTER_H
#define CATEGORIZEDLINECOLINEARTESTER_H

#include "CategorizedLine.h"
#include "STriangle.h"
#include "OrganicGLWinUtils.h"
#include "QuatRotationManager.h"
#include "QuatRotationPoints.h"
#include "PointTranslationCheck.h"
#include "CoplanarMassManipulator.h"
#include "TwoDLineSegment.h"
#include "TwoDLineSegmentIntersectAnalyzer.h"
#include "TwoDPoint.h"

class CategorizedLineColinearTester
{
	public:
		CategorizedLineColinearTester(CategorizedLine in_candidateLine, STriangle in_hostSTriangle) :
			candidateLine(in_candidateLine),
			hostSTriangle(in_hostSTriangle)
		{
			runColinearTests();
		};
		bool colinearDetected = false;
	private:
		CategorizedLine candidateLine;
		STriangle hostSTriangle;
		QuatRotationManager rotationManager;
		QuatRotationPoints coplanarPoints;
		PointTranslationCheck pointTranslator;
		void runColinearTests();
};

#endif
