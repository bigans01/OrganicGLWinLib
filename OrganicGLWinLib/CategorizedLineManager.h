#pragma once

#ifndef CATEGORIZEDLINEMANAGER_H
#define CATEGORIZEDLINEMANAGER_H

#include "CategorizedLineGroupMap.h"
#include "CategorizedLineContainer.h"
#include "IntersectionType.h"
#include "CategorizedLineGroupLocation.h"
#include "QuatRotationPoints.h"
#include "SPolyBorderLines.h"
#include "CategorizedLineSearchResult.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "CleaveSequence.h"


class CategorizedLineManager
{
	public:
		void setCategorizedLineManagerDebugLevel(PolyDebugLevel in_polyDebugLevel);
		int getCountOfIntersectionType(IntersectionType in_intersectionTypeToSearch);
		int insertLineAndReturnInsertedIndex(CategorizedLine in_categorizedLine);
		bool checkForDuplicateCategorizedLine(CategorizedLine in_duplicateToCheck);
		CategorizedLine fetchAndRemoveLineAtIndex(IntersectionType in_intersectionType, int in_targetIndex);
		CategorizedLine fetchAndRemoveWithGroupMapLocationPush(IntersectionType in_intersectionType,
															  int in_fetchIndex,
															  std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef);
		bool doAnyLinesExist();								// use in: CleaveSequenceFactory::doesFactoryContainLines() --> checks if any lines exist at all in the manager.
		void printLineCountsForEachType();					// use in: CleaveSequenceFactory::printLineCounts() --> prints all line counts in the manager.
		void invertEmptyNormalsInContainers();				// use in: CleaveSequenceFactory::invertAllEmptyNormals() --> inverts the empty normals of all categorized lines
		void loadCategorizedLineEmptyNormals(QuatRotationPoints* in_quatRotationPointsRef);	// use in CleaveSequenceFactory::loadCategorizedLineEmptyNormalsIntoQuatPoints --> loads the empty normals of all ines  into quat points.
		void loadCategorizedLinePoints(QuatRotationPoints* in_quatRotationPointsRef); // use in CleaveSequenceFactory::loadCategorizedLineMapReferencesIntoQuatPointsExcludeEmptyNormals --> loads points, but excludes normals.
		bool determineCyclingDirections(std::map<int, SPolyBorderLines> in_borderLineArrayRef, PolyDebugLevel in_polyDebugLevel);	// use in CleaveSequenceFactory::determineCyclingDirectionsForCategorizedLines --> determines cycling directions for all categorized lines.

		// For the below 3 functions, 
		// if there is no exact match, nearby lines are scanned; i f a nearby line is found, an "altered" version of
		// it is returned, and the original line the altering was based off of is removed (logic for the PointResolution can probably be used here too)
		CategorizedLineSearchResult checkManagerForNextNonboundLine(glm::vec3 in_pointToSearch);	// use in: CleaveSequenceFactory::checkForNextNonboundLine; --> searches for the next non-bound line to use;																									
		CategorizedLineSearchResult searchManagerForLastPartialBoundLineForSequence(glm::vec3 in_pointToSearch);	// use in: CleaveSequenceFactory::searchForLastPartialBoundLineForSequence --> searches for last PARTIAL_BOUND line that would complete a CleaveSequence.
		CategorizedLineSearchResult searchManagerForInterceptPointPreciseCategorizedLine(glm::vec3 in_pointToSearch);	// use in: CleaveSequenceFactory::searchForInterceptPointPreciseCategorizedLine --> searches for a INTERCEPTS_POINT_PRECISE.

		void printAllLines();	// use in: CleaveSequenceFactory::printLinesInPool() --> prints all lines in each CategorizedLineContainer.
		void clearAllLines();	// use in: CleaveSequenceFactory::clearLinePools() --> clears all lines in each CatgorizedLineContainer.
		std::vector<CategorizedLine> fetchAllLines(); // used in: CleaveSequenceFactory::writeOutCategorizedLines, when needing to write output for exception logging.

		std::map<int, CategorizedLine>::iterator getBeginIteratorForType(IntersectionType in_intersectionType);
		std::map<int, CategorizedLine>::iterator getEndIteratorForType(IntersectionType in_intersectionType);

	private:
		friend class CategorizedLineMerger;
		std::map<IntersectionType, CategorizedLineContainer> containerMap;
		CategorizedLineGroupMap managerGrpMap;
		PolyLogger categorizedLineManagerLogger;
		PolyDebugLevel categorizedLineManagerLoggerDebugLevel = PolyDebugLevel::NONE;

};

#endif