#pragma once

#ifndef SPOLYSET_H
#define SPOLYSET_H

#include "SPoly.h"
#include "IntersectionResult.h"
#include "IntersectionLine.h"
#include "IntersectionLineGroup.h"
#include "CleaveSequenceFactory.h"
#include "CategorizedLine.h"
#include "SliceFinder.h"
#include "SPolyFracturer.h"
#include "SPolyMorphTracker.h"
#include <map>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <chrono>
#include "CoplanarChecker.h"
#include "CoplanarRelationshipTracker.h"
#include "SPolySuperGroupManager.h"
#include "CategorizedLineColinearTester.h"
#include "MassZoneMaster.h"	// the final frontier. Good luck! (1/4/2021)
#include "MassZoneType.h"
#include "DebugOption.h"

class SPolySet
{
public:
	std::map<int, SPoly> secondaryPolys;	// holds up to 16 secondary polys
	SPolyMorphTracker polyMorphTracker;
	int numberOfPolys = 0;		// the number of polys
	SPolySupergroupManager polyFracturingResults;
	CoplanarRelationshipTracker coplanarTracker;
	MassZoneMaster zoneMaster;

	
	// ************************** Template function for enabling debugging on the SPolySet **********************************
	template<typename FirstOption, typename ...RemainingOptions> void setDebugOptions(FirstOption && firstOption, RemainingOptions && ...optionParams)
	{
		setOption(std::forward<FirstOption>(firstOption));
		setDebugOptions(std::forward<RemainingOptions>(optionParams)...);
	}
	void setDebugOptions() {};
	void setOption(DebugOption in_option)
	{
		if (in_option == DebugOption::SPOLYSET_TYPICAL)
		{
			std::cout << "!!! Will set TYPICAL operations to debug mode. " << std::endl;
		}
		else if (in_option == DebugOption::SPOLYSET_BOUNDARIES)
		{
			std::cout << "!!! Will set BOUNDARY operations to debug mode. " << std::endl;
		}
	}
	

	void addPoly(SPoly in_sPoly);
	void configurePolys();
	void configurePolysWithoutNormalCalcs();
	void runPolyComparison(MassZoneBoxType in_massZoneBoxType);
	//void insertPolyFracturingResults(int in_originalSPolyID, std::vector<SPoly>* in_producedSPolyVectorRef);
	void insertPolyFracturingResults(int in_originalSPolyID, SPolySupergroup in_producedSupergroup);
	void insertOriginalPolyAsFracturingResult(int in_originalSPolyID, SPoly in_sPoly);

	int produceCategorizedLinesForHostPoly(SPoly* in_hostPolyPtr, int in_hostPolyAID, SPoly* in_guestPolyPtr, int in_guestPolyID);
	IntersectionResult checkIfLineIntersectsTriangle(STriangle in_triangle, STriangleLine in_line);
	IntersectionResult checkIfRayIntersectsTriangle(STriangle in_triangle, STriangleLine in_line);
	IntersectionResult checkIfRayIntersectsTriangleSpecial(STriangle in_triangle, STriangleLine in_line);
	glm::vec3 cross(glm::vec3 in_A, glm::vec3 in_B);
	float dot(glm::vec3 in_A, glm::vec3 in_B);
	double doubledot(glm::vec3 in_A, glm::vec3 in_B);
	CategorizedLine determineCategorizedLineThroughHostTriangleContext(IntersectionLine in_hostLine, IntersectionLine in_guestLine, int in_groupID, glm::vec3 in_polyBEmptyNormal);
	bool checkForSamePointCondition(IntersectionLine in_lineA, IntersectionLine in_lineB);		// checks whether or not the points in two IntersectionLines are all equal, which will
																								// result in a non-existent intersection, meaning a CategorizedLine can't possibly exist.
	glm::vec3 findSecondPointForLine(glm::vec3 in_beginPoint, glm::vec3 in_candidate1, glm::vec3 in_candidate2);
	glm::vec3 roundPointToHundredths(glm::vec3 in_point);
	int checkIfPointsMatch(glm::vec3 in_pointA, glm::vec3 in_pointB);
	void reset();	// reset the values of the poly set
	float getRadiansForPosZViaY(glm::vec3 in_vec3);
	float getRadiansForPosYViaX(glm::vec3 in_vec3);
	glm::quat createQuaternion(float radians, glm::vec3 in_angle);
	void performFracturing();	// perform the slicing on any polygon that meets the valid condition of needing to be sliced.

	void runTest1();		// runs use case 1 
	void runTest2();		// Summary: contains 2 groups (IDs 1 and 2) that go through a set of polys.
	void runTest3();		// Summary: test for DISCONNECTED cleave type
	void runTest4();

};

#endif
