#include "stdafx.h"
#include "CyclingDirectionFinder.h"

CyclingDirectionFinder::CyclingDirectionFinder(SPolyBorderLines in_borderLineCopy, CategorizedLine in_categorizedLineCopy, MassManipulationMode in_massManipulationMode)
{
	// copy the data from the inputs
	borderLine = in_borderLineCopy;
	categorizedLine = in_categorizedLineCopy;

	// push back the quat points, which come from the copied data
	quatPoints.pointsRef.push_back(&borderLine.pointA);
	quatPoints.pointsRef.push_back(&borderLine.pointB);
	quatPoints.pointsRef.push_back(&categorizedLine.line.pointA);
	quatPoints.pointsRef.push_back(&categorizedLine.line.pointB);

	std::cout << ":::: cycling direction finder, pre translate: " << std::endl;
	std::cout << "border Line point A: " << borderLine.pointA.x << ", " << borderLine.pointA.y << ", " << borderLine.pointA.z << std::endl;
	std::cout << "border Line point B: " << borderLine.pointB.x << ", " << borderLine.pointB.y << ", " << borderLine.pointB.z << std::endl;
	std::cout << "categorized line point A: " << categorizedLine.line.pointA.x << ", " << categorizedLine.line.pointA.y << ", " << categorizedLine.line.pointA.z << std::endl;
	std::cout << "categorized line point B: " << categorizedLine.line.pointB.x << ", " << categorizedLine.line.pointB.y << ", " << categorizedLine.line.pointB.z << std::endl;


	// check for any translation; the point to check will be the first point of the categorized line. (so a copy of the 3rd point in quatPoints, index 2)
	pointTranslator.performCheck(quatPoints.getPointByIndex(2));
	if (pointTranslator.requiresTranslation == 1)
	{
		quatPoints.applyTranslation(pointTranslator.getTranslationValue());
	}

	// invert the normal if the mass manipulation mode is DESTRUCTION.
	if (in_massManipulationMode == MassManipulationMode::DESTRUCTION)
	{
		categorizedLine.emptyNormal *= -1.0f;
	}

	// the empty normal doesn't need to be translated.
	quatPoints.pointsRef.push_back(&categorizedLine.emptyNormal);

	// run the appropriate rotation manager function
	foundDirection = rotationManager.initializeAndRunForCyclingDirectionFinder(&quatPoints);

}