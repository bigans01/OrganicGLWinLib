#pragma once

#ifndef USABLECLEAVESEQUENCECALCULATOR_H
#define USABLECLEAVESEQUENCECALCULATOR_H

#include "CleaveSequence.h"
#include "CategorizedLine.h"
#include "CleaveSequenceCandidateList.h"
#include "DistanceToPoint.h"
#include "PolyLogger.h"
#include "PolyDebugLevel.h"
#include "QuatUtils.h"

class UsableCleaveSequenceCalculator
{
	public:
		UsableCleaveSequenceCalculator(int in_beginningCleaveSequenceID,
									   int in_borderLineIDToJudgeFor,
									   CategorizedLine in_judgementLine,
									   glm::vec3 in_judgementOriginPoint,	// would be initial value of currentLeadingPoint from LineWelder
			                           CleaveSequenceCandidateList* in_cleaveSequenceCandidateListRef,
									   std::map<int, CleaveSequence>* in_cleaveMapRef,
			                           PolyDebugLevel in_loggerDebugLevel) :
			judgingForCleaveSequenceID(in_beginningCleaveSequenceID),
			borderLineIDToJudgeFor(in_borderLineIDToJudgeFor),
			judgingLine(in_judgementLine),
			judgementOriginPoint(in_judgementOriginPoint),
			cleaveSequenceCandidateListRef(in_cleaveSequenceCandidateListRef),
			cleaveMapRef(in_cleaveMapRef),
			usableLoggerDebugLevel(in_loggerDebugLevel)
		{
			usableLogger.setDebugLevel(usableLoggerDebugLevel);
			runJudgement();
		};
		int judgingForCleaveSequenceID = 0;		// the cleave sequence ID we are judging for.
		int borderLineIDToJudgeFor = 0;			// the border line the judgement is being performed on
		CategorizedLine judgingLine;
		glm::vec3 judgementOriginPoint;
		CleaveSequenceCandidateList* cleaveSequenceCandidateListRef = nullptr;
		std::map<int, CleaveSequence>* cleaveMapRef = nullptr; // set upon initialization
		std::set<int> unusableCleaveSequences;
		OperableIntSet getUnusables();
	private:
		void runJudgement();
		PolyLogger usableLogger;
		PolyDebugLevel usableLoggerDebugLevel = PolyDebugLevel::NONE;
		bool executeJudgementOnCandidate(CategorizedLine in_lineToJudge);
		OperableIntSet unusableSet;				// contains IDs of CleaveSequences that shouldn't be used
};

#endif
