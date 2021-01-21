#pragma once

#ifndef FUSEDPOINTREACTORRESULT_H
#define FUSEDPOINTREACTORRESULT_H

#include "CategorizedLine.h"

class FusedPointReactorResult
{
	public:
		bool wasLineProduced = false;
		CategorizedLine resultingLine;
};

#endif
