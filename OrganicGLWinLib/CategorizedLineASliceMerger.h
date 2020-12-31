#pragma once

#ifndef CATEGORIZEDLINEASLICEMERGER_H
#define CATEGORIZEDLINEASLICEMERGER_H

#include "CategorizedLineMergeMachineBase.h"
#include <iostream>

class CleaveSequenceFactory;
class CategorizedLineASliceMerger : public CategorizedLineMergeMachineBase
{
	public:
		void runMerging();
};

#endif
