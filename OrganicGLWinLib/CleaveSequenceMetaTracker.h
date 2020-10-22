#pragma once

#ifndef CLEAVESEQUENCEMETATRACKER_H
#define CLEAVESEQUENCEMETATRACKER_H

#include <map>
#include "CleaveSequenceMeta.h"

class CleaveSequenceMetaTracker
{
	public:
		void insertNewCleaveSequence(int in_cleaveSequenceMetaID, CleaveSequenceMeta in_cleaveSequenceMeta)
		{
			cleaveSequenceMetaMap[in_cleaveSequenceMetaID] = in_cleaveSequenceMeta;
		}
		CleaveSequenceMeta* fetchCleaveSequence(int in_cleaveSequenceMetaID)
		{
			return &cleaveSequenceMetaMap[in_cleaveSequenceMetaID];
		}
	private:
		std::map<int, CleaveSequenceMeta> cleaveSequenceMetaMap;

};

#endif
