#pragma once

#ifndef SPOLY_H
#define SPOLY_H

#include "STriangle.h"
#include "SPolyBorderLines.h"
#include "CategorizedLine.h"
#include "CleaveSequence.h"
#include "CleaveSequenceFactory.h"
#include "InterceptRegister.h"
#include "EmptyNormalFinder.h"
#include "PointTranslationCheck.h"
#include "IRPointType.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <map>
#include "MassManipulationMode.h"
#include "CleaveSequenceCandidateListMap.h"
#include "CleaveSequenceMetaTracker.h"
#include "CyclingDirection.h"
#include "QuatRotationPoints.h"
#include "CleaveSequenceMergeMode.h"
#include "SPolyDOSet.h"

class SPoly
{
public:
	SPoly();
	//STriangle triangles[8];		// 8 possible triangles
	//SPolyBorderLines borderLines[8];	// the number of secondary poly border lines. For example, a square would have 4 border lines, out of a total of 5 lines.
	//std::map<int, SPolyBorderLines>
	std::map<int, STriangle> triangles;
	std::map<int, SPolyBorderLines> borderLines;

	int numberOfBorderLines = 0;
	int groupID = 0;				// the ID of the group that this poly belongs to
	int originalID = 0;				// the ID assigined to the poly the time it was spawned (optional; may not be used in dev/testing)
	int debugFlag = 0;				// for debugging purposes only
	short numberOfTriangles = 0;	// the number of triangles
	short polygonType = 0;		// what is the polygon's type? triangle_fan? etc...0 is typical triangle fan type
	glm::vec3 primePoint0, primePoint1, primePoint2;	// the very first points of the polygon
	glm::vec3 polyEmptyNormal;		// the polygon's empty normal, which indicates the side of the triangle that contains "empty" space
	//glm::vec3 planarVector;			// a vector that lies on the plane of the triangle, but goes towards the center of the triangle, and is perpendicular to this line
	glm::vec3 massOriginPoint;	// the point representing where the solid mass originates from. for example, from the top of a mountain, etc. (same as massReferencePoint)
	MassManipulationMode massManipulationSetting = MassManipulationMode::CREATION;	// default value is CREATION.

	CleaveSequenceFactory sequenceFactory;
	std::map<int, CategorizedLine> categorizedLineMap;
	std::map<int, CleaveSequence> cleaveMap;
	int currentCleaveIndex = 0;	// the index of the current cleave to work on
	InterceptRegister intercepts; // the interceptRegister instance for this SPoly

	void setEmptyNormal(float in_x, float in_y, float in_z);
	void determinePrimalPoints();
	void determineBorderLines();
	void buildCleaveSequences(CleaveSequenceMergeMode in_cleaveSequenceMergeMode);
	void setMRP(glm::vec3 in_mrp);
	void calculateEmptyNormal();
	void determinePlanarVectors();
	void addBorderLine(STriangleLine in_triangleLine);
	int getNextBorderLineID(int in_currentBorderLineID, CyclingDirection in_direction);	// will get the ID of the next SPolyBorderLine, given the ID of a valid current one, and a CyclingDirection.
	glm::vec3 getBorderLineEndpoint(int in_currentBorderLineID, CyclingDirection in_direction);
	void addTriangle(STriangle in_triangle);
	void addCategorizedLine(CategorizedLine in_line);	// inserts a calibrated line -- calibrated meaning, the begin point of this line equals the end point of the previous
	void addSlicedCleave(int in_currentCleaveIndex, CategorizedLine in_categorizedLine);
	void insertCalibratedLine(CategorizedLine in_line);
	int checkForNewCleaves();	// check if there are newly formed cleaves to process
	void moveLastCleave();	// moves any remaining cleave lines into the cleave mao, if there are any
	void constructCleaveLine();	// constructs a cleave line from all available lines in the categorizedLineMap member
	int determineCleaveTypeAndRegisterCatLines(int in_cleaveIndexID);		// determines if a cleave is DISCONNECTED or CONNECTED type; DISCONNECTED will be run at a later time.
														// a DISCONNECTED cleave is a set of categorized lines, in which none of the said lines has a border line intercept. In other words, the
														// cleave line is contained completely within the area of the polygon
														// the return value indicates if it is CONNECTED (1) or DISCONNECTED (0)
	void loadTrianglesAndBorderLinesIntoQuatPoints(QuatRotationPoints* in_quatRotationPointsRef);
	void loadPrimalsTrianglesAndBordersIntoQuatPoints(QuatRotationPoints* in_quatRotationPointsRef);
	void loadAllIntoQuatPoints(QuatRotationPoints* in_quatRotationPointsRef);
	int loadEmptyNormalsIntoQuatPoints(QuatRotationPoints* in_quatRotationPointsRef);

	void findPlanarVectorForSingleBorderLine(glm::vec3 in_point0, SPolyBorderLines* in_borderLineOneRef);
	void findPlanarVectorsForTwoBorderLines(SPolyBorderLines* in_borderLineOneRef, SPolyBorderLines* in_borderLineTwoRef);
	void findPlanarVectorsForThreeBorderLines(SPolyBorderLines* in_borderLineOneRef, SPolyBorderLines* in_borderLineTwoRef, SPolyBorderLines* in_borderLineThreeRef);
	glm::vec3 findCentroid(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
	void calculatePlanarVector(SPolyBorderLines* in_borderLineRef, glm::vec3 in_centroidPoint);
	glm::vec3 rotateForPlanarVectorCalc(glm::vec3* in_pointA, glm::vec3* in_pointB, glm::vec3* in_centroid);

	void registerIntersectingCatLine(int lineIndex, CategorizedLine in_line);	// prepares to insert the intersecting cat line; will call insertIntersectingCatLine with the appropriate arguments, based on qualifying criteria
	void insertIntersectingCatLine(int in_borderLineID, int in_catLineGroupID, int in_catLineID, IRPointType point_type, glm::vec3 in_point);	// wrapper-style function; inserts the intersecting line into the intercept register (by calling InterceptRegister.insertCatLine)
	void organizeCurrentCleaveLines();
	void printBorderLines();
	void printPlanarVectors();
	void printLines();
	void printAllCleaveLines();
	void setDebugFlag(int in_debugFlagValue);
	CleaveSequenceCandidateListMap buildCleaveSequenceCandidateListMap();		// builds and returns a populaated CleaveSequenceCandidateListMap (for use by LineWelder)
	CleaveSequenceMetaTracker buildCleaveSequenceMetaTracker();

	void clearInterceptRegistry();
	void applyDebugOptions(SPolyDOSet in_sPolyDOSet);
};

#endif
