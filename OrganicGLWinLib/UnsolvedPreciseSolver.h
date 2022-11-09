#pragma once

#ifndef UNSOLVEDPRECISESOLVER_H
#define UNSOLVEDPRECISESOLVER_H

#include "CategorizedLine.h"
#include "SPolyBorderLines.h"

class UnsolvedPreciseSolver
{
	public:
		UnsolvedPreciseSolver(CategorizedLine in_lineToSolveFor, glm::vec3 in_pointToSolveFor, std::map<int, SPolyBorderLines>* in_sPolyBorderLinesMapRef) :
			lineToSolveFor(in_lineToSolveFor),
			pointToSolveFor(in_pointToSolveFor),
			sPolyBorderLinesMapRef(in_sPolyBorderLinesMapRef)
		{
			generateSolution();
		}
		CategorizedLine fetchSolution();
	private:
		CategorizedLine lineToSolveFor;
		CategorizedLine producedLine;
		glm::vec3 pointToSolveFor;
		std::map<int, SPolyBorderLines>* sPolyBorderLinesMapRef = nullptr;
		void generateSolution();
};

#endif
