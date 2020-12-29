#pragma once

#ifndef CATEGORIZEDLINEPLACEBOMERGER_H
#define CATEGORIZEDLINEPLACEBOMERGER_H

#include "CategorizedLineMergeMachineBase.h"

class CleaveSequenceFactory;
class CategorizedLinePlaceboMerger : public CategorizedLineMergeMachineBase
{
	public:
		void extractCategorizedLines();
		void runMerging();
};

#endif
