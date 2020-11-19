#pragma once

#ifndef CLEAVESEQUENCEFACTORY_H
#define CLEAVESEQUENCEFACTORY_H

#include <map>
#include "CleaveSequence.h"
#include "CategorizedLine.h"
#include "CategorizedLineSearchResult.h"
#include "IRPointType.h"
#include "SPolyBorderLines.h"
#include "MassManipulationMode.h"
#include <glm/glm.hpp>
#include "QuatRotationPoints.h"
#include "PointTranslationCheck.h"
#include "QuatRotationManager.h"

class CleaveSequenceFactory
{
	public:
		void addCategorizedLine(CategorizedLine in_categorizedLine);
		//void constructAndExportCleaveSequences(std::map<int, CleaveSequence>* in_cleaveMapRef, SPolyBorderLines* in_borderLineArrayRef);
		void constructAndExportCleaveSequences(std::map<int, CleaveSequence>* in_cleaveMapRef, std::map<int, SPolyBorderLines> in_borderLineArrayRef, MassManipulationMode in_massManipulationMode);
		void printLinesInPool();


	private:
		std::map<int, CategorizedLine> nonboundMap;	// contains NON_BOUND categorized lines.
		std::map<int, CategorizedLine> partialboundMap;	// contains PARTIAL_BOUND categorized lines.
		std::map<int, CategorizedLine> aslicedMap;	// contains A_SLICED categorized lines.
		std::map<int, CategorizedLine> interceptsPointPreciseMap; // contains INTERCEPTS_POINT_PRECISE categorized lines.

		int nonboundCount = 0;
		int partialboundCount = 0;
		int aslicedCount = 0;
		int interceptsPointPreciseCount = 0;
		std::map<int, CleaveSequence>* cleaveSequenceMapRef = NULL;	 // initialize as null

		void insertNonboundLine(CategorizedLine in_line);
		void insertPartialBoundLine(CategorizedLine in_line);
		void insertAslicedLine(CategorizedLine in_line);
		void insertInterceptsPointPrecise(CategorizedLine in_line);
		void clipTwinCategorizedLinesofInterceptPointPrecise();

		void determineCyclingDirectionsForCategorizedLines(std::map<int, SPolyBorderLines>);

		CategorizedLine fetchAndRemoveNonbound(int in_fetchIndex);
		CategorizedLine fetchAndRemovePartialBound(int in_fetchIndex);
		CategorizedLine fetchAndRemoveInterceptPointPrecise(int in_fetchIndex);

		void insertFirstPartialBoundLineForSequence(CleaveSequence* in_cleaveSequenceRef, int in_lineIndex);
		void invertAllEmptyNormals();
		CategorizedLineSearchResult searchForLastPartialBoundLineForSequence(glm::vec3 in_pointToSearch);
		CategorizedLineSearchResult searchForInterceptPointPreciseCategorizedLine(glm::vec3 in_pointToSearch, std::map<int, SPolyBorderLines>);

		CategorizedLineSearchResult checkForNextNonboundLine(glm::vec3 in_pointToSearch);

		// scenario processing -- a scenario just describes different cases that occur, for instance the case in which there is at least
		// one CategorizedLine with an IntersectionType of INTERCEPTS_POINT_PRECISE.

		void handleScenarioTypical(std::map<int, CleaveSequence>* in_cleaveMapRef);
		void handleScenarioSingleInterceptsPointPreciseFound(std::map<int, CleaveSequence>* in_cleaveMapRef, std::map<int, SPolyBorderLines> in_borderLineArrayRef);

		// this function may not be needed; potentially removable after review.
		std::map<MassManipulationMode, int> generateManipulationDirectionsForIntersectsPointPrecise(SPolyBorderLines in_borderLineA, int in_borderLineAID, SPolyBorderLines in_borderLineB, int in_borderLineBID, glm::vec3 in_categorizedLineNormal);
};

#endif
