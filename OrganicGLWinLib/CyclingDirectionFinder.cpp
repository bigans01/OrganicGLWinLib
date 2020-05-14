#include "stdafx.h"
#include "CyclingDirectionFinder.h"

CyclingDirectionFinder::CyclingDirectionFinder(SPolyBorderLines in_borderLineCopy, CategorizedLine in_categorizedLineCopy)
{
	// copy the data from the inputs
	borderLine = in_borderLineCopy;
	categorizedLine = in_categorizedLineCopy;

	// push back the quat points, which come from the copied data
	quatPoints.pointsRef.push_back(&borderLine.pointA);
	quatPoints.pointsRef.push_back(&borderLine.pointB);
	quatPoints.pointsRef.push_back(&categorizedLine.line.pointA);
	quatPoints.pointsRef.push_back(&categorizedLine.line.pointB);

	// check for any translation; the point to check will be the first point of the categorized line. (so a copy of the 3rd point in quatPoints, index 2)
	pointTranslator.performCheck(quatPoints.getPointByIndex(2));
	if (pointTranslator.requiresTranslation == 1)
	{
		quatPoints.applyTranslation(pointTranslator.getTranslationValue());
	}

	// the empty normal doesn't need to be translated.
	quatPoints.pointsRef.push_back(&categorizedLine.emptyNormal);

	// run the appropriate rotation manager function
	rotationManager.initializeAndRunForCyclingDirectionFinder(&quatPoints);

}