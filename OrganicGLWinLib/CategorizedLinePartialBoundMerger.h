#pragma once

#ifndef CATEGORIZEDLINEPARTIALBOUNDMERGER_H
#define CATEGORIZEDLINEPARTIALBOUNDMERGER_H

#include "CategorizedLineMergeMachineBase.h"

class CleaveSequenceFactory;
class CategorizedLinePartialBoundMerger : public CategorizedLineMergeMachineBase
{
	public:
		void extractCategorizedLines();
		void runMerging();
};

#endif
