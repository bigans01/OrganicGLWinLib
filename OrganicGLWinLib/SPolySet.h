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
#include "PolyLogger.h"
#include "FusedIntersectionLine.h"
#include "FusionCandidate.h"
#include "FusionCandidateProducer.h"
#include "FusionCandidateOrigin.h"
#include "FusedPointReactor.h"
#include "SPolyDOSet.h"
#include "MassManipulationMode.h"

class SPolySet
{
public:
	SPolySupergroupManager polyFracturingResults;	// publicly available, can be used for various things.
	// ************************** Recursive template function for enabling debugging on the SPolySet **********************************
	template<typename FirstDebugOption, typename ...RemainingDebugOptions> void setDebugOptions(FirstDebugOption && firstOption, RemainingDebugOptions && ...optionParams)
	{
		// needs work; but if statement is done to avoid user error of parameters not passed in as DebugOption.
		if constexpr
		(
			(std::is_same<FirstDebugOption, DebugOption>::value)
		)
		{
			setOption(std::forward<FirstDebugOption>(firstOption));
			setDebugOptions(std::forward<RemainingDebugOptions>(optionParams)...);
		}
		else
		{
			std::cout << "(SPolySet) Invalid debug option passed in. " << std::endl;
		}
	}
	void setDebugOptions() {};

	// ************************** Recursive template function for enabling debug options on a specific SPoly **********************************
	template<typename FirstDebugOption, typename ...RemainingDebugOptions> void setDebugOptionsForSpecificSPoly(int in_sPolyID, FirstDebugOption && firstOption, RemainingDebugOptions && ...optionParams)
	{
		if constexpr
		(
			(std::is_same<FirstDebugOption, DebugOption>::value)
		)
		{
			setSpecificSPolyOption(in_sPolyID, std::forward<FirstDebugOption>(firstOption));
			setDebugOptionsForSpecificSPoly(in_sPolyID, std::forward<RemainingDebugOptions>(optionParams)...);
		}
		else
		{
			std::cout << "(SPolySet) Invalid debug option passed in. " << std::endl;
		}
	}
	void setDebugOptionsForSpecificSPoly(int in_sPolyID) {};

	// ************************** Recursive template function for enabling debug options on a specific tracked SPoly in an instance of CoplanarRelationships **********************************
	template<typename FirstDebugOption, typename ...RemainingDebugOptions> void setDOForSpecificTrackedSPolyInCoplanarRelationship(int in_trackedSPolyID, FirstDebugOption && firstOption, RemainingDebugOptions && ...optionParams)
	{
		coplanarTracker.insertDOForSpecificTrackedSPoly(in_trackedSPolyID, std::forward<FirstDebugOption>(firstOption));
		setDOForSpecificTrackedSPolyInCoplanarRelationship(in_trackedSPolyID, std::forward<RemainingDebugOptions>(optionParams)...);
	}
	void setDOForSpecificTrackedSPolyInCoplanarRelationship(int in_sPolyID) {};

	// ************************** Recursive template function for enabling debug options on a specific STriangle, in a specific tracked SPoly in an instance of CoplanarRelationships **********************************
	template<typename FirstDebugOption, typename ...RemainingDebugOptions> void setDOForSpecificTrackedSPolySTriangleInCoplanarRelationship(int in_trackedSPolyID, int in_trackedSPolySTriangleID, FirstDebugOption && firstOption, RemainingDebugOptions && ...optionParams)
	{
		coplanarTracker.insertDOForSpecificTrackedSPolySTriangle(in_trackedSPolyID, in_trackedSPolySTriangleID, std::forward<FirstDebugOption>(firstOption));
		setDOForSpecificTrackedSPolySTriangleInCoplanarRelationship(in_trackedSPolyID, in_trackedSPolySTriangleID, std::forward<RemainingDebugOptions>(optionParams)...);
	};
	void setDOForSpecificTrackedSPolySTriangleInCoplanarRelationship(int in_trackedSPolyID, int in_trackedSPolySTriangleID) {};

	// generic functions for public interface
	int addPoly(SPoly in_sPoly);	// adds a new SPoly, and returns the index in the secondaryPolys map that it was inserted in.
	void configurePolys();
	void configurePolysWithoutNormalCalcs();
	void runPolyComparison(MassZoneBoxType in_massZoneBoxType);
	void performFracturing();	// perform the slicing on any polygon that meets the valid condition of needing to be sliced.
	void reset();	// reset the values of the poly set

	// hard-codable testing functions; currently unused, but available
	void runTest1();		// runs use case 1 
	void runTest2();		// Summary: contains 2 groups (IDs 1 and 2) that go through a set of polys.
	void runTest3();		// Summary: test for DISCONNECTED cleave type
	void runTest4();

private:
	CoplanarRelationshipTracker coplanarTracker;					// stores any coplanar relationships that exist between two or more polys
	MassZoneMaster zoneMaster;										// manages control of the old and new MassZones
	PolyLogger comparisonLogger;									// debug output for comparison operations
	PolyDebugLevel mainFracturerDebugLevel = PolyDebugLevel::NONE;	// debug level that is used if no specific SPoly fracturer debug options are set; i.e, setting this to Debug puts
																	// all calls of SPolyFracturer in SPolySet to DEBUG mode.
	SPolyDOSet debugOptionsAllSPolys;								// a set of debug options that would be applied to all SPolys; 
	std::map<int, SPolyDOSet> specificSPolyOptionMap;				// a map containing SPolyDOSets for specific SPolys having certain IDs
	std::map<int, SPoly> secondaryPolys;							// stores instances of SPoly that have been added to the SPolySet, via addPoly.
	SPolyMorphTracker polyMorphTracker;				// NOTE: potentially unused; needs review.
	int numberOfPolys = 0;							// the number of polys


	void applyDebugOptionsToSpecificSPolys();
	void insertPolyFracturingResults(int in_originalSPolyID, SPolySupergroup in_producedSupergroup);
	void insertOriginalPolyAsFracturingResult(int in_originalSPolyID, SPoly in_sPoly);
	int produceCategorizedLinesForHostPoly(SPoly* in_hostPolyPtr, int in_hostPolyAID, SPoly* in_guestPolyPtr, int in_guestPolyID);

	IntersectionResult checkIfLineIntersectsTriangle(STriangle in_triangle, STriangleLine in_line);
	IntersectionResult checkIfRayIntersectsTriangle(STriangle in_triangle, STriangleLine in_line);
	IntersectionResult checkIfRayIntersectsTriangleSpecial(STriangle in_triangle, STriangleLine in_line);

	glm::vec3 cross(glm::vec3 in_A, glm::vec3 in_B);
	float dot(glm::vec3 in_A, glm::vec3 in_B);
	double doubledot(glm::vec3 in_A, glm::vec3 in_B);
	glm::vec3 roundPointToHundredths(glm::vec3 in_point);
	int checkIfPointsMatch(glm::vec3 in_pointA, glm::vec3 in_pointB);
	glm::quat createQuaternion(float radians, glm::vec3 in_angle);

	float getRadiansForPosZViaY(glm::vec3 in_vec3);
	float getRadiansForPosYViaX(glm::vec3 in_vec3);

	MassManipulationMode getManipulationModeForSPolyGroup(int in_sPolyGroupID); // gets the MMM for a group of SPolys (passed in value should be 0 or 1)

	void setOption(DebugOption in_option);
	void setSpecificSPolyOption(int in_sPolyID, DebugOption in_option);
	PolyDebugLevel checkForSPolyOptionInSpecificSPoly(int in_sPolyID, SPolyDO in_sPolyDO);
	void removeSPolysFlaggedAsPurgable();
};

#endif
