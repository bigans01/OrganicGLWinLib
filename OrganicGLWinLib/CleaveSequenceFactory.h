#pragma once

#ifndef CLEAVESEQUENCEFACTORY_H
#define CLEAVESEQUENCEFACTORY_H

#include "CleaveSequence.h"
#include "CategorizedLine.h"
#include "CategorizedLineSearchResult.h"
#include "IRPointType.h"
#include "SPolyBorderLines.h"
#include "MassManipulationMode.h"
#include "QuatRotationPoints.h"
#include "PointTranslationCheck.h"
#include "QuatRotationManager.h"
#include "CategorizedLinePool.h"
#include "CategorizedLineGroupMap.h"
#include "CategorizedLineMerger.h"
#include "CleaveSequenceMergeMode.h"
#include "CategorizedLineGroupLocation.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "CategorizedLineManager.h"
#include "MessageContainer.h"
#include "InvalidCleaveSequences.h"
#include "ExceptionRecorder.h"

class CleaveSequenceFactory
{
	public:
		void addCategorizedLine(CategorizedLine in_categorizedLine);

		// below: construction of CleaveSequences needs to have an ExceptionRecorder, in the case that CleaveSequence generation fails.
		InvalidCleaveSequences constructAndExportCleaveSequences(std::map<int, CleaveSequence>* in_cleaveMapRef, 
																std::map<int, SPolyBorderLines> in_borderLineArrayRef, 
																MassManipulationMode in_massManipulationMode, 
																CleaveSequenceMergeMode in_cleaveSequenceMergeMode,
																ExceptionRecorder* in_exceptionRecorderRef);
		void printLinesInPool();
		void copyCategorizedLinesFromLinePool(CategorizedLinePool* in_categorizedLinePoolRef);
		void printLineCounts();
		void clearLinePools();
		void loadCategorizedLineMapReferencesIntoQuatPointsExcludeEmptyNormals(QuatRotationPoints* in_quatRotationPointsRef);
		void loadCategorizedLineEmptyNormalsIntoQuatPoints(QuatRotationPoints* in_quatRotationPointsRef);
		bool doesFactoryContainLines();
		void setFactoryDebugLevel(PolyDebugLevel in_polyDebugLevel);
		void setMergerDebugLevel(PolyDebugLevel in_polyDebugLevel);
		void setFactoryBoundaryOrientationOption(BoundaryOrientation in_optionalFactoryOrientation);
		bool hasBadProduction = false;
	private:
		friend class CategorizedLineMerger;
		friend class CategorizedLineMergeMachineBase;
		friend class CategorizedLinePartialBoundMerger;
		friend class CategorizedLineASliceMerger;
		friend class CategorizedLinePlaceboMerger;

		PolyLogger cleaveSequenceFactoryLogger;
		PolyDebugLevel mergerDebugLevel = PolyDebugLevel::NONE;
		BoundaryOrientation optionalFactoryOrientation;

		CategorizedLineManager lineManager;
		CategorizedLineManager originalUnmergedLines;
		std::map<int, CleaveSequence>* cleaveSequenceMapRef = NULL;	 // initialize as null

		void insertNonboundLine(CategorizedLine in_line);
		
		bool determineCyclingDirectionsForCategorizedLines(std::map<int, SPolyBorderLines>);
		

		// fetch functions, that include a record push.
		CategorizedLine fetchAndRemoveNonboundWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef);
		CategorizedLine fetchAndRemovePartialBoundWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef);
		CategorizedLine fetchAndRemoveASliceWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef);
		CategorizedLine fetchAndRemoveASliceSingleInterceptsPointPreciseWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef);
		CategorizedLine fetchAndRemoveInterceptPointPreciseWithGroupMapLocationPush(int in_fetchIndex, std::vector<CategorizedLineGroupLocation>* in_categorizedLineGroupLocationVectorRef);

		

	
		//CategorizedLineSearchResult checkForNextNonboundLine(glm::vec3 in_pointToSearch);
		CategorizedLineSearchResult searchForLastPartialBoundLineForSequence(glm::vec3 in_pointToSearch);
		CategorizedLineSearchResult searchForInterceptPointPreciseCategorizedLine(glm::vec3 in_pointToSearch);

		// scenario processing -- a scenario just describes different cases that occur, for instance the case in which there is at least
		// one CategorizedLine with an IntersectionType of INTERCEPTS_POINT_PRECISE.

		InvalidCleaveSequences handleScenarioTypical(std::map<int, CleaveSequence>* in_cleaveMapRef,
													ExceptionRecorder* in_exceptionRecorderRef);
		InvalidCleaveSequences handleScenarioSingleInterceptsPointPreciseFound(std::map<int, CleaveSequence>* in_cleaveMapRef);
		InvalidCleaveSequences handleScenarioMultipleInterceptsPointPrecise(std::map<int, CleaveSequence>* in_cleaveMapRef);

		Message writeOutCategorizedLines(CleaveSequence* in_currentCleaveSequence, int in_currentIteration);	// write out all relevant CategorizedLine data to a 
																												// single Message, so that it can be used for exception logging or other purposes.
};

#endif
