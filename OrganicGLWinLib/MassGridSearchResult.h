#pragma once

#ifndef MASSGRIDSEARCHRESULT_H
#define MASSGRIDSEARCHRESULT_H

#include "MassGridArrayCell.h"

class MassGridSearchResult
{
	public:
		MassGridSearchResult() {};
		MassGridSearchResult(bool in_wasSearchKeyValid) : wasSearchKeyValid(in_wasSearchKeyValid) {};
		MassGridSearchResult(bool in_wasSearchKeyValid, MassGridArrayCell* in_cellRef) : 
			wasSearchKeyValid(in_wasSearchKeyValid),
			cellRef(in_cellRef)
		{};
		bool wasSearchKeyValid = false;
		MassGridArrayCell* cellRef = nullptr;
};

#endif
