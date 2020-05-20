#pragma once

#ifndef CATEGORIZEDLINESEARCHRESULT_H
#define CATEGORIZEDLINESEARCHRESULT_H

#include "CategorizedLine.h"

class CategorizedLineSearchResult
{
	public:
		bool wasFound = false;
		CategorizedLine returnLine;
		glm::vec3 nextPointToFind;		// the next point to find; this value is set appropriately when the search found a matching categorized line.
};

#endif
