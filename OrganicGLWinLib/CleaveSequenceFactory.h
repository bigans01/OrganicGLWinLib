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
#include "CategorizedLinePool.h"
#include "CategorizedLineGroupMap.h"
#include "CategorizedLineMerger.h"
#include "CleaveSequenceMergeMode.h"
#include <vector>
#include "CategorizedLineGroupLocation.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "CategorizedLineManager.h"

class CleaveSequenceFactory
{
	public:
		void addCategorizedLine(CategorizedLine in_categorizedLine);
		void constructAndExportCleaveSequences(std::map<int, CleaveSequence>* in_cleaveMapRef, std::map<int, SPolyBorderLines> in_borderLineArrayRef, MassManipulationMode in_massManipulationMode, CleaveSequenceMergeMode in_cleaveSequenceMergeMode);
		void printLinesInPool();
		void copyCategorizedLinesFromLinePool(CategorizedLinePool* in_categorizedLinePoolRef);
		void printLineCounts();
		void clearLinePools();
		void loadCategorizedLineMapReferencesIntoQuatPointsExcludeEmptyNormals(QuatRotationPoints* in_quatRotationPointsRef);
		void loadCategorizedLineEmptyNormalsIntoQuatPoints(QuatRotationPoints* in_quatRotationPointsRef);
		bool doesFactoryContainLines();
		void setFactoryDebugLevel(PolyDebugLevel in_polyDebugLevel);
		void setMergerDebugLevel(PolyDebugLevel in_polyDebugLevel);
		bool hasBadProduction = false;
	private:
		friend class CategorizedLineMerger;
		friend class CategorizedLineMergeMachineBase;
		friend class CategorizedLinePartialBoundMerger;
		friend class CategorizedLineASliceMerger;
		friend class CategorizedLinePlaceboMerger;

		PolyLogger cleaveSequenceFactoryLogger;
		PolyDebugLevel mergerDebugLevel = PolyDebugLevel::NONE;

		/*
		std::map<int, CategorizedLine> nonboundMap;					// contains NON_BOUND categorized lines.
		std::map<int, CategorizedLine> partialboundMap;				// contains PARTIAL_BOUND categorized lines.
		std::map<int, CategorizedLine> aslicedMap;					// contains A_SLICED categorized lines.
		std::map<int, CategorizedLine> interceptsPointPreciseMap;	// contains INTERCEPTS_POINT_PRECISE categorized lines.
		std::map<int, CategorizedLine> aslicedSegmentEndpointMap;
		std::map<int, CategorizedLine> aslicedSingleInterceptsPointPreciseMap;	// contains A_SLICE_SINGLE_INTERCEPTS_POINT_PRECISE categorized lines.
		CategorizedLineGroupMap groupMap;

		int nonboundCount = 0;
		int partialboundCount = 0;
		int aslicedCount = 0;
		int interceptsPointPreciseCount = 0;
		int aslicedSegmentEndpointCount = 0;
		int aslicedSingleInterceptsPointPreciseCount = 0;
		*/


		CategorizedLineManager lineManager;
		std::map<int, CleaveSequence>* cleaveSequenceMapRef = NULL;	 // initialize as null

		void insertNonboundLine(CategorizedLine in_line);
		//void insertPartialBoundLine(CategorizedLine in_line);
		//void insertAslicedLine(CategorizedLine in_line);
		//void insertAslicedSingleInterceptsPointPrecise(CategorizedLine in_line);
		//void insertInterceptsPointPrecise(CategorizedLine in_line);
		
		bool determineCyclingDirectionsForCategorizedLines(std::map<int, SPolyBorderLines>);

		// normal fetch functions.
		//CategorizedLine fetchAndRemoveNonbound(int in_fetchIndex);
		//CategorizedLine fetchAndRemovePartialBound(int in_fetchIndex);
		//CategorizedLine fetchAndRemoveASlice(int in_fetchIndex);
		//CategorizedLine fetchAndRemoveASliceSingleInterceptsPointPrecise(int in_fetchIndex);
		CategorizedLine fetchAndRemoveInterceptPointPrecise(int in_fetchIndex);

		// fetch functions, that include a record push.
		CategorizedLine fetchAndRemoveNonboundWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef);
		CategorizedLine fetchAndRemovePartialBoundWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef);
		CategorizedLine fetchAndRemoveASliceWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef);
		CategorizedLine fetchAndRemoveASliceSingleInterceptsPointPreciseWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef);
		CategorizedLine fetchAndRemoveInterceptPointPreciseWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef);



		//void insertFirstPartialBoundLineForSequence(CleaveSequence* in_cleaveSequenceRef, int in_lineIndex);
		//void insertASliceLineForSequence(CleaveSequence* in_cleaveSequenceRef, int in_lineIndex);
		//void insertASliceSingleInterceptsPointPreciseForSequence(CleaveSequence* in_cleaveSequenceRef, int in_lineIndex);
		//void insertFirstInterceptsPointPreciseForSequence(CleaveSequence* in_cleaveSequenceRef, int in_lineIndex);
		//void invertAllEmptyNormals();
		//CategorizedLineSearchResult checkForNextNonboundLine(glm::vec3 in_pointToSearch);
		CategorizedLineSearchResult searchForLastPartialBoundLineForSequence(glm::vec3 in_pointToSearch);
		CategorizedLineSearchResult searchForInterceptPointPreciseCategorizedLine(glm::vec3 in_pointToSearch);

		// scenario processing -- a scenario just describes different cases that occur, for instance the case in which there is at least
		// one CategorizedLine with an IntersectionType of INTERCEPTS_POINT_PRECISE.

		void handleScenarioTypical(std::map<int, CleaveSequence>* in_cleaveMapRef);
		void handleScenarioSingleInterceptsPointPreciseFound(std::map<int, CleaveSequence>* in_cleaveMapRef);
		void handleScenarioMultipleInterceptsPointPrecise(std::map<int, CleaveSequence>* in_cleaveMapRef);

};

#endif
