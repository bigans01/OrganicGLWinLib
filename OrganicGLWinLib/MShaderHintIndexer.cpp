#include "stdafx.h"
#include "MShaderHintIndexer.h"

void MShaderHintIndexer::insertTransitionalHint(MShaderHintEnum in_hintToAdd)
{
	transitionalHints.insert(in_hintToAdd);
}

std::set<MShaderHintEnum> MShaderHintIndexer::fetchTransitionalHints()
{
	return transitionalHints;
}