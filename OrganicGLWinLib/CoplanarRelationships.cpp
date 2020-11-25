#include "stdafx.h"
#include "CoplanarRelationships.h"

void CoplanarRelationships::setTrackedPolyData(int in_trackedPolyID, SPoly* in_trackedSPolyRef)
{
	trackedPolyID = in_trackedPolyID;
	trackedSPolyRef = in_trackedSPolyRef;
}
void CoplanarRelationships::insertRelationship(int in_relatedSPolyID, SPoly* in_relatedSPolyRef)
{
	relationshipMap.insertSPolyRef(in_relatedSPolyID, in_relatedSPolyRef);
}
void CoplanarRelationships::rotateToXYPlaneAndCompare()
{
	// step 1: rotate involved SPolys to the XY plane


	// 1.1: load the points (that is, points of STriangles and SPolyBorderLines) before applying translation.
	// 1.1.1: load points from the trackedSpolyRef
	//trackedSPolyRef->loadTrianglesAndBorderLinesIntoQuatPoints(&coplanarPoints);
	trackedSPolyRef->loadPrimalsTrianglesAndBordersIntoQuatPoints(&coplanarPoints);

	// 1.1.2: load points from the related SPolys
	auto relatedSPolysBegin = relationshipMap.refMap.begin();
	auto relatedSPolysEnd = relationshipMap.refMap.end();
	for (; relatedSPolysBegin != relatedSPolysEnd; relatedSPolysBegin++)
	{
		relatedSPolysBegin->second->loadTrianglesAndBorderLinesIntoQuatPoints(&coplanarPoints);
	}

	// 1.2: translate the first point of the first triangle in the first SPoly to 0.
	pointTranslator.performCheck(trackedSPolyRef->borderLines[0].pointA);
	if (pointTranslator.requiresTranslation == 1)	// almost 100% of the time, this will be run
	{
		std::cout << "!! prime point 0 requires translation!! " << std::endl;
		std::cout << "It's value is: " << trackedSPolyRef->borderLines[0].pointA.x << ", " << trackedSPolyRef->borderLines[0].pointA.y << ", " << trackedSPolyRef->borderLines[0].pointA.z << std::endl;
		coplanarPoints.applyTranslation(pointTranslator.getTranslationValue());
	}
	else
	{
		std::cout << "!! prime point 0 requires no translation. " << std::endl;
	}

	// 1.3 rotate points by the quaternion
	rotationManager.initializeAndRunForZFracture(&coplanarPoints);

	std::cout << "--> printing lines for tracked SPoly " << std::endl;
	trackedSPolyRef->printBorderLines();
	relatedSPolysBegin = relationshipMap.refMap.begin();
	relatedSPolysEnd = relationshipMap.refMap.end();
	for (; relatedSPolysBegin != relatedSPolysEnd; relatedSPolysBegin++)
	{
		std::cout << "-->printing lines for related SPoly " << std::endl;
		relatedSPolysBegin->second->printBorderLines();
	}
	

	// step 2: check if its MassManipulationMode::CREATION or DESTRUCTION.
	//		if CREATION -> use CoplanarMassCreator
	//		if DESTRUCTION -> use CoplanarMassDestroyer

}