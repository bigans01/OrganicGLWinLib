#pragma once

#ifndef CLEAVESEQUENCEFACTORY_H
#define CLEAVESEQUENCEFACTORY_H

#include <map>
#include "CleaveSequence.h"
#include "CategorizedLine.h"
#include "CategorizedLineSearchResult.h"
#include "IRPointType.h"

class CleaveSequenceFactory
{
	public:
		void addCategorizedLine(CategorizedLine in_categorizedLine);
		void constructAndExportCleaveSequences(std::map<int, CleaveSequence>* in_cleaveMapRef);
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

		CategorizedLine fetchAndRemoveNonbound(int in_fetchIndex);
		CategorizedLine fetchAndRemovePartialBound(int in_fetchIndex);
		CategorizedLine fetchAndRemoveInterceptPointPrecise(int in_fetchIndex);

		void insertFirstPartialBoundLineForSequence(CleaveSequence* in_cleaveSequenceRef, int in_lineIndex);
		CategorizedLineSearchResult searchForLastPartialBoundLineForSequence(glm::vec3 in_pointToSearch);
		CategorizedLineSearchResult searchForInterceptPointPreciseCategorizedLine(glm::vec3 in_pointToSearch);

		CategorizedLineSearchResult checkForNextNonboundLine(glm::vec3 in_pointToSearch);

		// scenario processing -- a scenario just describes different cases that occur, for instance the case in which there is at least
		// one CategorizedLine with an IntersectionType of INTERCEPTS_POINT_PRECISE.

		void handleScenarioTypical(std::map<int, CleaveSequence>* in_cleaveMapRef);
		void handleScenarioSingleInterceptsPointPreciseFound(std::map<int, CleaveSequence>* in_cleaveMapRef);
};

#endif
