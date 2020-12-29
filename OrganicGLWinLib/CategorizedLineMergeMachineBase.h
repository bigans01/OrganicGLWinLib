#pragma once

#ifndef CATEGORIZEDLINEMERGEMACHINEBASE_H
#define CATEGORIZEDLINEMERGEMACHINEBASE_H

#include "CategorizedLineGroup.h"

class CleaveSequenceFactory;
class CategorizedLineMergeMachineBase
{
	public:
		void initialize(CleaveSequenceFactory* in_cleaveSequenceFactoryRef, CategorizedLineGroup* in_categorizedLineGroupRef)
		{
			cleaveSequenceFactoryRef = in_cleaveSequenceFactoryRef;
			categorizedLineGroupRef = in_categorizedLineGroupRef;
		};

		virtual void extractCategorizedLines() = 0;
		virtual void runMerging() = 0;		// defined by children;

		CleaveSequenceFactory* cleaveSequenceFactoryRef = nullptr;
		CategorizedLineGroup* categorizedLineGroupRef = nullptr;


};

#endif
