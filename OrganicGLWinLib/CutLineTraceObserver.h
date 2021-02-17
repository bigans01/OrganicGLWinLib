#pragma once

#ifndef CUTLINETRACEOBSERVER_H
#define CUTLINETRACEOBSERVER_H

#include "CutLinePool.h"
#include <vector>
#include "CutTriangleContainer.h"
#include "CutLinePoolGuide.h"
#include "TracingObserverState.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "CutTriangleProducer.h"

class CutLineTraceObserver
{
	private:
		friend class CutTriangleGroupBuilder;

		CutTriangleContainer currentContainer;
		TracingObserverState currentObserverState = TracingObserverState::CONTINUE_OBSERVE;
		CutLinePool* cutLinePoolRef = nullptr;
		std::vector<CutTriangleContainer>* cutTriangleContainerVectorRef = nullptr;
		CutLine lineOfSight;
		CutLine observationEndLine;
		CutLinePoolGuide poolGuide;
		float remainingRadians = 1000.0f;				// set the initial value to a really high value, as this will always shrink. If it ever increases, the observer should change its state to LINE_OF_SIGHT_BROKEN
		bool areRemainingRadiansValid = true;			// used to ensure that the current value of remainingRadians is smaller than the last
		bool isLineOfSightMaintained = true;
		PolyLogger cutLineTraceObserverLogger;
		PolyDebugLevel cutLineTraceObserverLoggerDebugLevel = PolyDebugLevel::NONE;

		void determineObservationRadians();
		void determineObservationState();
		bool insertWeldedTriangleIfLineOfSightIsMaintained();
		void setCutLinePoolRef(CutLinePool* in_cutLinePoolRef);
		void setCutTriangleContainerVectorRef(std::vector<CutTriangleContainer>* in_cutTriangleContainerVectorRef);
		void buildNewCutObservation(CutLinePoolGuide in_cutLinePoolGuide);
		void setObservationLogLevel(PolyDebugLevel in_polyDebugLevel);
		TracingObserverState getCurrentObserverState();
};

#endif