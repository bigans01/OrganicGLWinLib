#include "stdafx.h"
#include "CyclingDirectionFinder.h"

CyclingDirectionFinder::CyclingDirectionFinder(SPolyBorderLines in_borderLineCopy, CategorizedLine in_categorizedLineCopy)
{
	borderLine = in_borderLineCopy;
	categorizedLine = in_categorizedLineCopy;

	// push back the quat points
	quatPoints.pointsRef.push_back(&borderLine.pointA);
	quatPoints.pointsRef.push_back(&borderLine.pointB);
	quatPoints.pointsRef.push_back(&categorizedLine.line.pointA);
	quatPoints.pointsRef.push_back(&categorizedLine.line.pointB);
	quatPoints.pointsRef.push_back(&categorizedLine.emptyNormal);

}