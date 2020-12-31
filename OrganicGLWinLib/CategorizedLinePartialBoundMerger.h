#pragma once

#ifndef CATEGORIZEDLINEPARTIALBOUNDMERGER_H
#define CATEGORIZEDLINEPARTIALBOUNDMERGER_H

#include "CategorizedLineMergeMachineBase.h"
#include <iostream>

class CleaveSequenceFactory;
class CategorizedLinePartialBoundMerger : public CategorizedLineMergeMachineBase
{
	public:
		void runMerging();
};

#endif
