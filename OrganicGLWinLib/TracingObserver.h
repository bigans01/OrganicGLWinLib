#pragma once

#ifndef TRACINGOBSERVER_H
#define TRACINGOBSERVER_H

#include "WeldedLinePool.h"
#include "QuatRotationPoints.h"
#include "QuatRotationManager.h"
#include "TracingObserverState.h"
#include "WeldedLinePoolGuide.h"
#include "PointTranslationCheck.h"
#include "WeldedTriangleProducer.h"
#include "WeldedTriangleProductionResult.h"
#include "WeldedTriangleContainer.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"

class TracingObserver
{
	public:
		void setWeldedLinePoolRef(WeldedLinePool* in_weldedLinePoolRef);
		void setWeldedTriangleContainerVectorRef(std::vector<WeldedTriangleContainer>* in_weldedTriangleVectorRef);
		void buildNewObservation(WeldedLinePoolGuide in_poolGuide);
		void setObservationLogLevel(PolyDebugLevel in_polyDebugLevel);
		//void buildNewObservation(WeldedLine in_lineOfSight, WeldedLine in_observationEndLine);
		TracingObserverState getCurrentObserverState();
		WeldedTriangleContainer currentContainer;
	private:
		TracingObserverState currentObserverState = TracingObserverState::CONTINUE_OBSERVE;		// default value assumes there is tracing to be done, but can be set to FINISHED if there are only 3 lines in the WeldedPool.
		WeldedLinePool* weldedLinePoolRef = nullptr;
		//WeldedTriangleContainer* weldedTriangleVectorRef = nullptr;
		std::vector<WeldedTriangleContainer>* weldedTriangleVectorContainerRef = nullptr;
		WeldedLine lineOfSight;
		WeldedLine observationEndLine;
		WeldedLinePoolGuide poolGuide;
		float remainingRadians = 1000.0f;		// set the initial value to a really high value, as this will always shrink. If it ever increases, the observer should change its state to LINE_OF_SIGHT_BROKEN
		bool areRemainingRadiansValid = true;			// used to ensure that the current value of remainingRadians is smaller than the last
		bool isLineOfSightMaintained = true;
		void determineObservationRadians();
		void determineObservationState();
		bool insertWeldedTriangleIfLineOfSightIsMaintained();	// compares the current lineOfSight to comparable lines, to see if they intersect.

		PolyLogger tracingObserverLogger;
		PolyDebugLevel tracingObserverLoggerDebugLevel = PolyDebugLevel::NONE;
};

#endif
