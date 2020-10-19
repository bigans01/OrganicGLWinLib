#pragma once

#ifndef CYCLINGDIRECTIONFINDER_H
#define CYCLINGDIRECTIONFINDER_H

#include "SPolyBorderLines.h"
#include "CategorizedLine.h"
#include "QuatRotationPoints.h"
#include "QuatRotationManager.h"
#include "PointTranslationCheck.h"
#include "CyclingDirection.h"
#include "MassManipulationMode.h"

class CyclingDirectionFinder
{
public:
	CyclingDirectionFinder(SPolyBorderLines in_borderLineCopy, CategorizedLine in_categorizedLineCopy, MassManipulationMode in_massManipulationMode);


	glm::vec3 borderLinePointA;
	glm::vec3 borderLinePointB;
	glm::vec3 categorizedLinePointA;
	glm::vec3 categorizedLinePointB;
	glm::vec3 categorizedLineEmptyNormal;
	QuatRotationPoints quatPoints;
	QuatRotationManager rotationManager;
	PointTranslationCheck pointTranslator;
	SPolyBorderLines borderLine;
	CategorizedLine categorizedLine;
	CyclingDirection foundDirection;

};

#endif
