#pragma once

#ifndef SPOLYFRACTURER_H
#define SPOLYFRACTURER_H

#include <glm/glm.hpp>
#include "SPoly.h"
#include "PointTranslationCheck.h"
#include "QuatRotationPoints.h"
#include "QuatRotationManager.h"
#include "CleaveSequenceIntersectFinder.h"
#include "SPolyMorphTracker.h"
#include <chrono>
#include <vector>

class SPolyFracturer
{
public:
	SPoly* polyRef;		// a reference to the SPoly to produce a fractured version from
	SPolyMorphTracker* morphTrackerRef;
	PointTranslationCheck pointTranslator; // check for any translation
	QuatRotationPoints quatPoints;
	QuatRotationManager rotationManager;
	std::vector<SPoly> producedPolys;

	SPolyFracturer(SPoly* in_sPolyRef, SPolyMorphTracker* in_morphTrackerRef);
	void runFracturing();	// run the fracturing process
	void applyTranslationToAllPoints(glm::vec3 in_translationOffset);	// applies a point translation to all points (but not the normals)
	void populatePointsForQuaternions();	// populates the point ref vector in rotationPoints with all cleave line points, normals, and border lines; all of these will need to be transformed by the quaternion(s)
	void generatePlanarNormalsForPoly();		// calls calculatePlanarNormal in the SPoly
	void checkForCleaveIntersections();
	void printPointMetaData();
};

#endif