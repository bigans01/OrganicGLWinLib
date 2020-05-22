#include "stdafx.h"
#include "CleaveSequenceFactory.h"

void CleaveSequenceFactory::addCategorizedLine(CategorizedLine in_categorizedLine)
{
	if (in_categorizedLine.type == IntersectionType::A_SLICE)
	{
		insertAslicedLine(in_categorizedLine);
	}
	else if (in_categorizedLine.type == IntersectionType::PARTIAL_BOUND)
	{
		insertPartialBoundLine(in_categorizedLine);
	}
	else if (in_categorizedLine.type == IntersectionType::NON_BOUND)
	{
		insertNonboundLine(in_categorizedLine);
	}
	else
	{
		std::cout << "!!!! WARNING, other line type discovered..." << std::endl;
	}
}

void CleaveSequenceFactory::insertNonboundLine(CategorizedLine in_line)
{
	nonboundMap[nonboundCount++] = in_line;
}

void CleaveSequenceFactory::insertPartialBoundLine(CategorizedLine in_line)
{
	partialboundMap[partialboundCount++] = in_line;
}

void CleaveSequenceFactory::insertAslicedLine(CategorizedLine in_line)
{
	aslicedMap[aslicedCount++] = in_line;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemoveNonbound(int in_fetchIndex)
{
	CategorizedLine returnLine = nonboundMap[in_fetchIndex];
	nonboundMap.erase(in_fetchIndex);
	nonboundCount--;		// decrement the number of nonbound lines
	return returnLine;
}

CategorizedLine CleaveSequenceFactory::fetchAndRemovePartialBound(int in_fetchIndex)
{
	CategorizedLine returnLine = partialboundMap[in_fetchIndex];
	partialboundMap.erase(in_fetchIndex);
	partialboundCount--; // decrement the number of partial lines
	return returnLine;
}

void CleaveSequenceFactory::constructAndExportCleaveSequences(std::map<int, CleaveSequence>* in_cleaveMapRef)
{
	// only do this if there are partial bound lines or a-sliced lines
	if
	(
		(partialboundCount > 0) || (aslicedCount > 0)
	)
	{
		//std::cout << "## Partial count: " << partialboundCount << std::endl;
		//std::cout << "## Non-bound count: " << nonboundCount << std::endl;
		//std::cout << "## Sliced count: " << aslicedCount << std::endl;

		while (partialboundCount > 0)	// do this until all partial_bound lines have been accounted for. 
		{
			cleaveSequenceMapRef = in_cleaveMapRef;									// set the map reference that we will export results to.

			auto partialBoundMapBegin = partialboundMap.begin();					// get the first line in the partial bound map
			CategorizedLine* partialBoundLineRef = &partialBoundMapBegin->second;	// get a ref to the line
			int firstLineID = partialBoundMapBegin->first;							// store the ID of the first line (for removal later)
			CleaveSequence newSequence;												// the new line sequence that will eventually be inserted back into the referenced SPoly
			insertFirstPartialBoundLineForSequence(&newSequence, firstLineID);		// insert the first partial bound line we find
			glm::vec3 firstPointToSearch = newSequence.fetchPointToSearch();	// get the searchable point from the first partial bound line we found in the previous step
			//std::cout << "!!! Initial point to search is: " << firstPointToSearch.x << ", " << firstPointToSearch.y << ", " << firstPointToSearch.z << std::endl;


			// first, work with the partially bound lines. Get the first available partial bound line in the map, scan for linking nonbound lines, until no more linking lines are found.
			// Then, scan the partialBoundMap until the ending partially bound line is found (the B point of this partial line will equal the B point of a linked nonbound line). 
			// When this is done, take all the lines invovled (both nonbound and partial bound) and put them into a new CleaveSequence. Remmove their original copies from the appropriate maps --
			// partial bound lines will be removed from partialBoundMap (and the counter decremented) and the nonbound lines will be removed from the nonboundMap (also decrementing here)
			bool continueSearch = true;
			CategorizedLineSearchResult result = checkForNextLine(firstPointToSearch);	// search for the first point.
			if (result.wasFound == true)		// insert the first categorized line into the sequence, if it was found:
			{
				newSequence.insertNonboundLine(result.returnLine);	// insert the fetched line into the sequence
				bool continueFlag = true;							// check for the next line, at least once.
				while (continueFlag == true)	// loop until this is false.
				{
					glm::vec3 nextPointToSearch = newSequence.fetchPointToSearch();
					CategorizedLineSearchResult nextResult = checkForNextLine(nextPointToSearch);
					if (nextResult.wasFound == false)
					{
						continueFlag = false;	// end the loop.
					}
					else if (nextResult.wasFound == true)
					{
						newSequence.insertNonboundLine(nextResult.returnLine);
					}
				}
			}

			// once the search for partially bound lines is done, look for the ending partially bound line for the sequence.
			glm::vec3 lastPointToSearch = newSequence.fetchPointToSearch();
			CategorizedLineSearchResult finalResult = searchForLastPartialBoundLineForSequence(lastPointToSearch);
			if (finalResult.wasFound == true)
			{
				//std::cout << "Final partial bound line found! Inserting final line... !!" << std::endl;
				newSequence.insertLastLine(finalResult.returnLine);
				newSequence.sequenceStatus = CleaveSequenceStatus::COMPLETE; // mark it as complete
			}
			else
			{
				//std::cout << "!! Final partial bound line NOT FOUND! " << std::endl;
				newSequence.sequenceStatus = CleaveSequenceStatus::INCOMPLETE; // mark it as complete
			}
			//std::cout << "## Remaining number of partial bounds: " << partialboundCount << std::endl;


			// lastly, if the newSequence is marked as "complete" move it to the referenced sequence. Otherwise, discard it.
			if (newSequence.sequenceStatus == CleaveSequenceStatus::COMPLETE)
			{
				int cleaveMapRefSize = (*in_cleaveMapRef).size();
				//std::cout << "!! Inserting new cleave sequence at index: " << cleaveMapRefSize << std::endl;
				(*in_cleaveMapRef)[cleaveMapRefSize] = newSequence;	// insert the sequence.
				cleaveMapRefSize = (*in_cleaveMapRef).size();
				//std::cout << "Map size is now: " << cleaveMapRefSize << std::endl;
			}
		}
	}
}

void CleaveSequenceFactory::insertFirstPartialBoundLineForSequence(CleaveSequence* in_cleaveSequenceRef, int in_lineIndex)
{
	in_cleaveSequenceRef->insertFirstLine(fetchAndRemovePartialBound(in_lineIndex));
}

CategorizedLineSearchResult CleaveSequenceFactory::searchForLastPartialBoundLineForSequence(glm::vec3 in_pointToSearch)
{
	CategorizedLineSearchResult searchResult;
	if (partialboundCount > 0)
	{
		// search through all the partially bound lines.
		auto partialboundBegin = partialboundMap.begin();
		auto partialboundEnd = partialboundMap.end();
		bool wasFound = false;		// was a match found?
		int foundIndex = 0;			// if it was found, what was the index?
		IRPointType pointCheckResult = IRPointType::NEITHER;	// starts out as NEITHER
		for (partialboundBegin; partialboundBegin != partialboundEnd; partialboundBegin++)
		{
			pointCheckResult = partialboundBegin->second.checkIfPointIsInLine(in_pointToSearch);
			if (pointCheckResult != IRPointType::NEITHER)
			{
				wasFound = true;
				foundIndex = partialboundBegin->first;
				break;
			}

			std::cout << "One iteration passed through partial bounds..." << std::endl;
		}

		// if it was found, do this stuff (should be able to ignore invalid iterators this way)
		if (wasFound == true)
		{
			searchResult.wasFound = true;
			CategorizedLine foundLine = fetchAndRemovePartialBound(foundIndex);
			// remember, when dealing with the ending partial line, the matched point should be "point A" of the border line; if it isn't, we must swap the line.
			if (pointCheckResult == IRPointType::POINT_B)
			{
				foundLine.line.swapToA();	// it's backwards; so swap the lines.

			}
			searchResult.returnLine = foundLine;	// store the appropriate line
			searchResult.nextPointToFind = foundLine.line.pointB;	// set the next point to find.
		}
	}
	else
	{
		searchResult.wasFound = false;
	}
	return searchResult;
}

CategorizedLineSearchResult CleaveSequenceFactory::checkForNextLine(glm::vec3 in_pointToSearch)
{
	CategorizedLineSearchResult searchResult;
	if (nonboundCount > 0)		// search for categorized lines, but only if there are ones to search for.
	{
		// search through all the non bound lines.
		auto nonboundBegin = nonboundMap.begin();
		auto nonboundEnd = nonboundMap.end();
		bool wasFound = false;	// was a match found? 
		int foundIndex = 0;		// if it was found, what was its index?
		IRPointType pointCheckResult = IRPointType::NEITHER;	// starts out as NEITHER
		for (nonboundBegin; nonboundBegin != nonboundEnd; nonboundBegin++)
		{
			pointCheckResult = nonboundBegin->second.checkIfPointIsInLine(in_pointToSearch);
			if (pointCheckResult != IRPointType::NEITHER) // it was found (it's either A or B)
			{
				wasFound = true;					// set the found flag.
				foundIndex = nonboundBegin->first;	// store the index.
				break;								// exit for loop when found.
			}

			std::cout << "One iteration passed through nonbounds..." << std::endl;
		}

		// if it was found, do this stuff (should be able to ignore invalid iterators this way)
		if (wasFound == true)
		{
			searchResult.wasFound = true;			// set the search result found value to be true
			CategorizedLine foundLine = fetchAndRemoveNonbound(foundIndex);		// remove from the nonbound.
			if (pointCheckResult == IRPointType::POINT_B)
			{
				foundLine.line.swapToA();	// it's backwards; so swap the lines.

			}
			searchResult.returnLine = foundLine;	// store the appropriate line
			searchResult.nextPointToFind = foundLine.line.pointB;	// set the next point to find.


			//std::cout << "!! Point (next line) was found! " << std::endl;
			//std::cout << "Point A: " << foundLine.line.pointA.x << ", " << foundLine.line.pointA.y << ", " << foundLine.line.pointA.z << " | Point B: " << foundLine.line.pointB.x << ", " << foundLine.line.pointB.y << ", " << foundLine.line.pointB.z << std::endl;
			//std::cout << "Number of remaining nonbounds: " << nonboundCount << std::endl;
		}

	}
	else
	{
		searchResult.wasFound = false;
	}

	return searchResult;
}

void CleaveSequenceFactory::printLinesInPool()
{
	// print partially bound lines
	if (partialboundCount > 0)
	{
		auto begin = partialboundMap.begin();
		auto end = partialboundMap.end();
		//std::cout << ">>> --- Partial lines: " << std::endl;
		for (begin; begin != end; begin++)
		{
			//std::cout << begin->first << ": point A: " << begin->second.line.pointA.x << ", " << begin->second.line.pointA.y << ", " << begin->second.line.pointA.z << " | point B: " << begin->second.line.pointB.x << ", " << begin->second.line.pointB.y << ", " << begin->second.line.pointB.z << std::endl;
		}
	}

	// print nonbound lines
	if (nonboundCount > 0)
	{
		auto begin = nonboundMap.begin();
		auto end = nonboundMap.end();
		//std::cout << ">>> --- Non-bound lines: " << std::endl;
		for (begin; begin != end; begin++)
		{
			//std::cout << begin->first << ": point A: " << begin->second.line.pointA.x << ", " << begin->second.line.pointA.y << ", " << begin->second.line.pointA.z << " | point B: " << begin->second.line.pointB.x << ", " << begin->second.line.pointB.y << ", " << begin->second.line.pointB.z << std::endl;
		}
	}
}