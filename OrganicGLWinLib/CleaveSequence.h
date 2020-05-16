#pragma once

#ifndef CLEAVESEQUENCE_H
#define CLEAVESEQUENCE_H

#include <map>
#include "CategorizedLine.h"

class CleaveSequence
{
public:
	std::map<int, CategorizedLine> cleavingLines;
	int cleaveIndex = 0;
};

#endif