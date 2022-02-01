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
#include "ECBPolyPoint.h"
#include "IndependentUtils.h"
#include "MessageContainer.h"
#include "Message.h"
#include "MassZoneBoxBoundaryOrientation.h"
#include "InvalidCleaveSequences.h"

class CleaveSequenceFactory
{
	public:
		void addCategorizedLine(CategorizedLine in_categorizedLine);
		InvalidCleaveSequences constructAndExportCleaveSequences(std::map<int, CleaveSequence>* in_cleaveMapRef, 
																std::map<int, SPolyBorderLines> in_borderLineArrayRef, 
																MassManipulationMode in_massManipulationMode, 
																CleaveSequenceMergeMode in_cleaveSequenceMergeMode);
		void printLinesInPool();
		void copyCategorizedLinesFromLinePool(CategorizedLinePool* in_categorizedLinePoolRef);
		void printLineCounts();
		void clearLinePools();
		void loadCategorizedLineMapReferencesIntoQuatPointsExcludeEmptyNormals(QuatRotationPoints* in_quatRotationPointsRef);
		void loadCategorizedLineEmptyNormalsIntoQuatPoints(QuatRotationPoints* in_quatRotationPointsRef);
		bool doesFactoryContainLines();
		void setFactoryDebugLevel(PolyDebugLevel in_polyDebugLevel);
		void setMergerDebugLevel(PolyDebugLevel in_polyDebugLevel);
		void setFactoryBoundaryOrientationOption(MassZoneBoxBoundaryOrientation in_optionalFactoryOrientation);
		bool hasBadProduction = false;
	private:
		friend class CategorizedLineMerger;
		friend class CategorizedLineMergeMachineBase;
		friend class CategorizedLinePartialBoundMerger;
		friend class CategorizedLineASliceMerger;
		friend class CategorizedLinePlaceboMerger;

		PolyLogger cleaveSequenceFactoryLogger;
		PolyDebugLevel mergerDebugLevel = PolyDebugLevel::NONE;
		MassZoneBoxBoundaryOrientation optionalFactoryOrientation;

		CategorizedLineManager lineManager;
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

		InvalidCleaveSequences handleScenarioTypical(std::map<int, CleaveSequence>* in_cleaveMapRef);
		InvalidCleaveSequences handleScenarioSingleInterceptsPointPreciseFound(std::map<int, CleaveSequence>* in_cleaveMapRef);
		InvalidCleaveSequences handleScenarioMultipleInterceptsPointPrecise(std::map<int, CleaveSequence>* in_cleaveMapRef);
};

#endif
