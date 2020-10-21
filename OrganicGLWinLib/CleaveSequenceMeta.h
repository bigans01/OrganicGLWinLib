#pragma once

#ifndef CLEAVESEQUENCEMETA_H
#define CLEAVESEQUENCEMETA_H

#include "CleaveSequenceType.h"

class CleaveSequenceMeta
{
	public:
		CleaveSequenceMeta() {};
		CleaveSequenceMeta(CleaveSequenceType  in_cleaveSequenceType, int in_numberOfLines) :
			sequenceType(in_cleaveSequenceType),
			numberOfLines(in_numberOfLines)
		{};
		CleaveSequenceType sequenceType = CleaveSequenceType::UNDEFINED;	// undefined by default, set by initialization
		int numberOfLines = 0;												// set by initialization
};

#endif
