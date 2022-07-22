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
#include "PointToMassRelationshipMap.h"
#include "BorderLineLinkContainer.h"
#include "QuatUtils.h"
#include "MessageContainer.h"
#include "BoundaryOrientation.h"
#include "InvalidCleaveSequences.h"
#include "SPolyMode.h"
#include "BoundaryPolyIndicator.h"

class SPoly
{
public:
	SPoly();
	void printBorderLines();
	void printPlanarVectors();
	void printLines();
	void printPoints();
	void printAllCleaveLines();
	void addTriangle(STriangle in_triangle);
	void setMRP(glm::vec3 in_mrp);
	void setEmptyNormal(float in_x, float in_y, float in_z);
	void setEmptyNormal(glm::vec3 in_normalVec3);
	void setSPolyMaterial(short in_sPolyMaterialID);
	void setBoundaryIndicatorOrientation(BoundaryOrientation in_boundaryOrientation);
	BoundaryOrientation getBoundaryIndicatorOrientation();
	void insertCleaveSequenceAtIndex(int in_indexToInsertAt, CleaveSequence in_cleaveSequence);
	short getSPolyMaterial();
	glm::vec3 getEmptyNormal();
	void calculateEmptyNormal();
	int calculateAndGetPerfectClampingValue();	// at least one STriangle must be added before calling this function.
	bool isSPolyValid();
	void determineBorderLines();
	void setMode(SPolyMode in_mode);
	SPolyMode getMode();
	int groupID = 0;				// the ID of the group that this poly belongs to
	int debugFlag = 0;				// for debugging purposes only
	std::map<int, STriangle> triangles; // needed by OREReformer in OrganicCoreLib
	std::map<int, SPolyBorderLines> borderLines;
	BoundaryPolyIndicator sPolyBoundaryIndicator;
	MassManipulationMode massManipulationSetting = MassManipulationMode::CREATION;	// default value is CREATION.
private:
	friend class RPoly;
	friend class SPolyScaler;
	friend class SPolySet;
	friend class SPolyShellProducer;
	friend class SPolySupergroup;
	friend class SPolyFracturer;
	friend class BorderLineCycle;
	friend class CleaveSequenceIntersectFinder;
	friend class CoplanarChecker;
	friend class CoplanarCategorizedLineProducer;
	friend class CoplanarRelationships;
	friend class CoplanarMassCreator;
	friend class CoplanarMassManipulator;
	friend class HostLineReactor;
	friend class GuestLineReactor;
	friend class SharedLineReactor;
	friend class LineWelder;
	friend class MassZone;
	friend class MassZoneBox;
	friend class MassZoneBoxBoundary;
	friend class MassZoneBoxBoundarySPolySet;
	friend class MassZonePointClipper;
	friend class TwoDLineSegmentJudge;
	friend class CoplanarFusionMachine;

	SPolyMode mode = SPolyMode::TYPICAL;	// always starts as TYPICAL (as that is whats assumed); however, due to mitigation, it could be MALFORMED_MITIGATION
	int numberOfSPolyBorderLines = 0;
	int originalID = 0;				// the ID assigined to the poly the time it was spawned (optional; may not be used in dev/testing)
	//short polygonType = 0;		// what is the polygon's type? triangle_fan? etc...0 is typical triangle fan type
	
	short sPolyMaterialID = 0;	// the SPoly material, if any is set for this SPoly. (needed by "R" family of classes)
	glm::vec3 primePoint0, primePoint1, primePoint2;	// the very first points of the polygon
	glm::vec3 massOriginPoint;	// the point representing where the solid mass originates from. for example, from the top of a mountain, etc. (same as massReferencePoint)
	short numberOfTriangles = 0;	// the number of triangles
	glm::vec3 polyEmptyNormal;		// the polygon's empty normal, which indicates the side of the triangle that contains "empty" space

	CleaveSequenceFactory sequenceFactory;
	std::map<int, CategorizedLine> categorizedLineMap;
	std::map<int, CleaveSequence> cleaveMap;
	int currentCleaveIndex = 0;	// the index of the current cleave to work on
	InterceptRegister intercepts; // the interceptRegister instance for this SPoly

	std::map<int, int> getBorderLinesForSharedPoint(glm::vec3 in_sharedPoint);


	void determinePrimalPoints();
	InvalidCleaveSequences buildCleaveSequences(CleaveSequenceMergeMode in_cleaveSequenceMergeMode, BoundaryOrientation in_optionalOrientation);
	void determinePlanarVectors();
	void addBorderLine(STriangleLine in_triangleLine);
	int getNextBorderLineID(int in_currentBorderLineID, CyclingDirection in_direction);	// will get the ID of the next SPolyBorderLine, given the ID of a valid current one, and a CyclingDirection.
	glm::vec3 getBorderLineEndpoint(int in_currentBorderLineID, CyclingDirection in_direction);
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
	void loadEmptyNormalsIntoQuatPoints(QuatRotationPoints* in_quatRotationPointsRef);

	void findPlanarVectorForSingleBorderLine(glm::vec3 in_point0, SPolyBorderLines* in_borderLineOneRef);
	void findPlanarVectorsForTwoBorderLines(SPolyBorderLines* in_borderLineOneRef, SPolyBorderLines* in_borderLineTwoRef);
	void findPlanarVectorsForThreeBorderLines(SPolyBorderLines* in_borderLineOneRef, SPolyBorderLines* in_borderLineTwoRef, SPolyBorderLines* in_borderLineThreeRef);
	glm::vec3 findCentroid(glm::vec3 in_point0, glm::vec3 in_point1, glm::vec3 in_point2);
	void calculatePlanarVector(SPolyBorderLines* in_borderLineRef, glm::vec3 in_centroidPoint);
	glm::vec3 rotateForPlanarVectorCalc(glm::vec3* in_pointA, glm::vec3* in_pointB, glm::vec3* in_centroid);

	void registerIntersectingCatLine(int lineIndex, CategorizedLine in_line);	// prepares to insert the intersecting cat line; will call insertIntersectingCatLine with the appropriate arguments, based on qualifying criteria
	void insertIntersectingCatLine(int in_borderLineID, int in_catLineGroupID, int in_catLineID, IRPointType point_type, glm::vec3 in_point);	// wrapper-style function; inserts the intersecting line into the intercept register (by calling InterceptRegister.insertCatLine)
	void organizeCurrentCleaveLines();
	void setDebugFlag(int in_debugFlagValue);
	CleaveSequenceCandidateListMap buildCleaveSequenceCandidateListMap();		// builds and returns a populaated CleaveSequenceCandidateListMap (for use by LineWelder)
	CleaveSequenceMetaTracker buildCleaveSequenceMetaTracker();

	void clearInterceptRegistry();
	void applyDebugOptions(SPolyDOSet in_sPolyDOSet);

	PointToMassRelationshipMap generatePointToMassRelationshipMap();
	BorderLineLinkContainer buildBuildBorderLineLinkContainer();
};

#endif
