#include "stdafx.h"
#include "CoplanarRelationships.h"


CoplanarRelationships::CoplanarRelationships()
{

}


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
	// arguments:
	// 1.) a reference to the tracked SPoly
	// 2.) a copy of the relationship map
	// 3.) a referenece to this instance's instantiaton of coplanarPoints. This is because we will first append the new CategorizedLines produced by the 
	// CoplanarMassManipulator, apply the quaternion to them, then remove the references to the empty normal, before translating the points of the categorized lines.
	// All of this data should be insreted at the end of the coplanarPoints.

	// 2.1: set the manipulator, run as CREATION or DESTRUCTION after initializing.
	if (trackedSPolyRef->massManipulationSetting == MassManipulationMode::CREATION)
	{
		std::cout << "!!!! MM Mode is set as creation; processing via CoplanarMassCreator..." << std::endl;
		manipulator.reset(new CoplanarMassCreator());
		manipulator->initialize(trackedSPolyRef, relationshipMap, &coplanarPoints);
	}

	// 2.2: when the manipulator is done, apply the reverse of the quaternion rotation.

	// 2.3: before translating back, remove references to the empty normals of the newly produced categorized lines.

	// 2.4: lastly, translate all involved SPolys back to their original position.
}