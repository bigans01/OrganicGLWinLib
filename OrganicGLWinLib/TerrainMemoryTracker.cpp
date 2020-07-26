#include "stdafx.h"
#include "TerrainMemoryTracker.h"

void TerrainMemoryTracker::setTriangleSize(int in_vertexSize)	// set the value of triangleSizeInGLBytes
{
	triangleSizeInGLBytes = in_vertexSize * 3;	// actual size of a triangle is 3 * the tuple width (tuple width = total memory of one VAO (?))
}

void TerrainMemoryTracker::jobFlagAsReplaceable(EnclaveKeyDef::EnclaveKey in_collectionKey)
{
	if (!terrainTracker.empty())	// don't bother to do this, if the map is empty
	{
		std::map<int, TerrainMemoryMeta>::iterator terrainBegin = terrainTracker.begin();
		std::map<int, TerrainMemoryMeta>::iterator terrainEnd = terrainTracker.end();
		for (terrainBegin; terrainBegin != terrainEnd; terrainBegin++)
		{
			if (terrainBegin->second.collectionKey == in_collectionKey) // we found the key, time to do stuff
			{
				terrainBegin->second.replaceable = 0;	// flag it as being replaceable.
			}
		}
	}
}

int TerrainMemoryTracker::insertNewCollection(TerrainJobResults in_terrainJobResults)
{
	TerrainMemoryMeta tempMemoryMeta;
	if (terrainTracker.empty())		// if it's the very first one, insert this
	{
		tempMemoryMeta.collectionKey = in_terrainJobResults.collectionKey;
		tempMemoryMeta.byteOffsetBegin = 0;
		tempMemoryMeta.byteSize = in_terrainJobResults.numberOfTrianglesProduced*triangleSizeInGLBytes;			// insert the total bytes that the mesh occupies
		tempMemoryMeta.byteOffsetEnd = in_terrainJobResults.numberOfTrianglesProduced*triangleSizeInGLBytes;
		terrainTracker[maxIndex] = tempMemoryMeta;																// load finished tempMemoryMeta into the map
		maxIndex++;																								// make sure to increment the maxIndex
		return tempMemoryMeta.byteOffsetEnd;																	// could be begin/end here, doesn't matter
	}
	else	// otherwise, if it isn't empty, do this
	{
		// first, check for any replaceable elements.
		std::map<int, TerrainMemoryMeta>::iterator terrainBegin = terrainTracker.begin();
		std::map<int, TerrainMemoryMeta>::iterator terrainEnd = terrainTracker.end();
		int wasReplaceableFound = 0;		// will indicate if there was a replaceable element found
		for (terrainBegin; terrainBegin != terrainEnd; terrainBegin++)
		{
			if (terrainBegin->second.replaceable == 0) // we found the key, time to do stuff
			{
				wasReplaceableFound = 1;	// set replaceable flag
				break;
			}
		}

		// now, check for any elements if they already existed
		std::map<int, TerrainMemoryMeta>::iterator terrainBegin2 = terrainTracker.begin();
		std::map<int, TerrainMemoryMeta>::iterator terrainEnd2 = terrainTracker.end();
		int alreadyExists = 0;
		int alreadyExistingID = 0;
		for (terrainBegin2; terrainBegin2 != terrainEnd2; terrainBegin2++)
		{
			if (terrainBegin2->second.collectionKey == in_terrainJobResults.collectionKey)
			{
				alreadyExists = 1;
				alreadyExistingID = terrainBegin2->first;
				break;
			}
		}


		// if there was a replaceable element in the map, but no matching collection key was found in the map, let's take the terrainBegin iterator that pointed to the first replaceable and update the contents with new data.
		if (wasReplaceableFound == 1 && alreadyExists == 0)
		{
			// calculate the triangle difference
			int triangleDifference = 0;																										// the difference in the number of bytes, between the newly inserted element and the old one
			triangleDifference = (in_terrainJobResults.numberOfTrianglesProduced*triangleSizeInGLBytes) - terrainBegin->second.byteSize;	// calculate the difference

			// shift the byteOffsetBegin and byteOffsetEnd of all elements above the current one (for example, if we have 9 elements, and we are replacing the 6th, you'd have to shift 7,8, and 9)
			if (triangleDifference != 0) // ...but only if the difference isn't 0.
			{
				std::map<int, TerrainMemoryMeta>::iterator shiftBegin = terrainBegin;	// copy terrainBegin's iterator to shiftBegin.
				shiftBegin++;															// iterate to the next one.
				while (shiftBegin != terrainEnd)										// only loop until we are at the end; won't even iterate if shiftBegin is the last element in the map
				{
					shiftBegin->second.byteOffsetBegin += triangleDifference;
					shiftBegin->second.byteOffsetEnd += triangleDifference;
					shiftBegin++;
				}

				// update the replaced collection with new data
				shiftBegin = terrainBegin; // reset shift begin back to the found element
				shiftBegin->second.replaceable = 1;		// set it as being not replaceable
				shiftBegin->second.collectionKey = in_terrainJobResults.collectionKey;		// replace the key
				shiftBegin->second.byteSize += triangleDifference;
				shiftBegin->second.byteOffsetEnd += triangleDifference;	// set the byteOffsetEnd appropriately
				tempMemoryMeta.byteOffsetEnd = shiftBegin->second.byteOffsetEnd;
			}
			//shiftBegin->second.replaceable = 1;		// set it as being not replaceable
			//shiftBegin->second.collectionKey = in_terrainJobResults.collectionKey;		// replace the key
			else if (triangleDifference == 0)
			{
				std::map<int, TerrainMemoryMeta>::iterator shiftBegin = terrainBegin;
				shiftBegin->second.replaceable = 1;											// set it as being not replaceable
				shiftBegin->second.collectionKey = in_terrainJobResults.collectionKey;		// replace the key
			}
		}

		// if it already existed, we will update the already existing value
		else if (alreadyExists == 1)
		{
			std::map<int, TerrainMemoryMeta>::iterator matchingElement = terrainTracker.find(alreadyExistingID);
			std::map<int, TerrainMemoryMeta>::reverse_iterator lastElement = terrainTracker.rbegin();
			int lastElementID = lastElement->first;
			std::map<int, TerrainMemoryMeta>::iterator trueLastElement = terrainTracker.find(lastElementID);

			//std::cout << " !! Already existing element found !! --- > (" << matchingElement->second.collectionKey.x << ", " << matchingElement->second.collectionKey.y << ", " << matchingElement->second.collectionKey.z << ") " << std::endl;
			int triangleDifference = 0;																										// the difference in the number of bytes, between the newly inserted element and the old one
			triangleDifference = (in_terrainJobResults.numberOfTrianglesProduced*triangleSizeInGLBytes) - matchingElement->second.byteSize;	// calculate the difference


			if (triangleDifference != 0) // ...but only if the difference isn't 0.
			{


				std::map<int, TerrainMemoryMeta>::iterator shiftBegin = matchingElement;  // copy terrainBegin's iterator to shiftBegin.
				shiftBegin++; // iterate to the next one.
				while (shiftBegin != terrainEnd)	// only loop until we are at the end; won't even iterate if shiftBegin is the last element in the map
				{
					shiftBegin->second.byteOffsetBegin += triangleDifference;
					shiftBegin->second.byteOffsetEnd += triangleDifference;
					shiftBegin++;
				}



				// update the replaced collection with new data
				shiftBegin = matchingElement;												// reset shift begin back to the found element
				shiftBegin->second.replaceable = 1;											// set it as being not replaceable, if it was already
				shiftBegin->second.byteSize += triangleDifference;							// set triangle difference, if there is even any
				shiftBegin->second.byteOffsetEnd += triangleDifference;						// set the byteOffsetEnd appropriately
				tempMemoryMeta.byteOffsetEnd = shiftBegin->second.byteOffsetEnd;			// set appropriate return variable values	
			}
			else if (triangleDifference == 0)	// ...and if there is no difference in triangles, we don't need to do anything except return the byteOffsetEnd of the element pointed to by shiftBegin.
			{
				std::map<int, TerrainMemoryMeta>::iterator shiftBegin = matchingElement;	// copy terrainBegin's iterator to shiftBegin.
				tempMemoryMeta.byteOffsetEnd = shiftBegin->second.byteOffsetEnd;			// set appropriate return variable values	
			}

		}


		// if there were no replaceable elements, we'll just insert a completely new one in.
		else if (wasReplaceableFound == 0)
		{
			std::map<int, TerrainMemoryMeta>::reverse_iterator lastElement = terrainTracker.rbegin();
			int newElementID = lastElement->first + 1;	// get the ID of the newest value to insert
			tempMemoryMeta.collectionKey = in_terrainJobResults.collectionKey;		// set the appropriate collection key
			tempMemoryMeta.byteOffsetBegin = lastElement->second.byteOffsetEnd;		// the new beginning will be the last element in the map's end value
			tempMemoryMeta.byteSize = in_terrainJobResults.numberOfTrianglesProduced*triangleSizeInGLBytes;
			tempMemoryMeta.byteOffsetEnd = tempMemoryMeta.byteOffsetBegin + tempMemoryMeta.byteSize;		// the ending offset value is equivalent to the beginning byte offset + the byte offset of the triangle mesh
			terrainTracker[newElementID] = tempMemoryMeta;							// insert the new element
			maxIndex++;
		}
		return tempMemoryMeta.byteOffsetEnd;
	}
}

TerrainMemoryMoveMeta TerrainMemoryTracker::removeUnusedReplaceablesAndShift()
{
	// declare return value
	TerrainMemoryMoveMeta returnMeta;


	// search for a replaceable
	int replaceAbleCount = 0;	// default is 0, no replaceables.
	int lastElementKey = terrainTracker.rbegin()->first;	// get the element of the last key in the map
	int foundElementKey = 0;	// the key of the found element
	std::map<int, TerrainMemoryMeta>::iterator terrainBegin = terrainTracker.begin();
	std::map<int, TerrainMemoryMeta>::iterator terrainEnd = terrainTracker.end();
	for (terrainBegin; terrainBegin != terrainEnd; terrainBegin++)
	{
		if (terrainBegin->second.replaceable == 0) // we found the key, time to do stuff
		{
			foundElementKey = terrainBegin->first;
			replaceAbleCount++;
		}
	}

	// a replaceable exist; get the memory range of everything above this element and save it (as long as it isn't the last element)
	if (replaceAbleCount != 0)
	{
		//std::cout << "Replaceable count is not 0..." << std::endl;
		if (foundElementKey != lastElementKey)	// check: only do shifting for a replaceable removal, when the targeted replaceable isn't the last element in the ordered list.
		{
			//std::cout << "Unused replaceable detected (not last element)!!! --> (" << foundElementKey << ")" << std::endl;
			std::map<int, TerrainMemoryMeta>::iterator targetedElement = terrainTracker.find(foundElementKey);
			int shiftCount = 0;																						// for storing the number of elements to shift from here until the end of the map; if there is one element to shift above the targetElement, the value would be 1 (see below)
			std::map<int, TerrainMemoryMeta>::reverse_iterator lastElement = terrainTracker.rbegin();				// use reverse iter to get the value of the last element in the map
			int lastElementID = lastElement->first;																	// get the ID of the last element in the ordered map
			std::map<int, TerrainMemoryMeta>::iterator endElement = terrainTracker.find(lastElementID);				// put the ending value in a normal iterator

			// before we iterate, store the return values.
			returnMeta.byteOffset = targetedElement->second.byteOffsetEnd;											// offset's start is the targetedElement's byteOffsetEnd
			returnMeta.byteSize = (lastElement->second.byteOffsetEnd - targetedElement->second.byteOffsetEnd);		// subtract the end element's byteOffsetEnd from  the target element's byteOffsetBegin to get the size of the data to copy
			returnMeta.copyBackOffset = targetedElement->second.byteOffsetBegin;									// the offset to copy back the data when we are done shifting (for OpenGL)
			returnMeta.containsMovement = 1;																		// flag for movement being valid, since we found an element to shift

			for (targetedElement; targetedElement != endElement; targetedElement++)		// find the value of shiftCount
			{
				shiftCount++;
			}
			//std::cout << "Shift count is: " << shiftCount << std::endl;

			// reset target element iterator, and spawn the iterator for the next element
			targetedElement = terrainTracker.find(foundElementKey);		// reset this iterator
			std::map<int, TerrainMemoryMeta>::iterator nextElement = targetedElement;
			nextElement++;	// the "next element" is the target element + 1

			// now, loop
			for (int x = 0; x < shiftCount; x++)
			{
				// calculate the offset change
				int memoryOffsetChange = 0;
				int nextElementSize = nextElement->second.byteSize;			// the next element's byte size
				int currentElementSize = targetedElement->second.byteSize;	// the current element's byte size
				memoryOffsetChange = nextElementSize - currentElementSize;	// get the difference


				if ((shiftCount > 1))				// F-005 
				//if ((memoryOffsetChange < 0) && (shiftCount > 1))				// F-005 
				{
					if ((shiftCount - x) > 1)	// only issue the following function if there are  more elements in the map that exist beyond the "next" element
					{
						//std::cout << "!!!!###!  Warning, negative offset detected; subsequent elements must be modified!!! " << std::endl;
						int negativeShiftCount = shiftCount - 1;	// get the number of elements to negative shift
						std::map<int, TerrainMemoryMeta>::iterator negativeShiftStart = nextElement;	// set the start of the negative shifting to be at the iterator of the current next element.
						negativeShiftCount = lastElementKey - negativeShiftStart->first;
						//std::cout << "Current next Element ID: " << negativeShiftStart->first << " | " << nextElement->second.collectionKey.x << ", " << nextElement->second.collectionKey.y << ", " << nextElement->second.collectionKey.z << std::endl;
						//std::cout << "Last element ID: " << lastElementKey << std::endl;
						//std::cout << "Number of negative shifts to perform: " << negativeShiftCount << std::endl;

						//std::cout << "!!!!##!  Iterator Found, OK" << std::endl;
						for (int x = 0; x < negativeShiftCount; x++)
						{
							negativeShiftStart++;	// iterate one time
							negativeShiftStart->second.byteOffsetBegin += memoryOffsetChange;
							negativeShiftStart->second.byteOffsetEnd += memoryOffsetChange;
						}
					}
				}


				//std::cout << "++++++++++++Before changes: " << std::endl;
				//std::cout << targetedElement->first << "): beginOffset: " << targetedElement->second.byteOffsetBegin << " | size: " << targetedElement->second.byteSize << " | endOffset: " << targetedElement->second.byteOffsetEnd << std::endl;
				//std::cout << nextElement->first << "): beginOffset: " << nextElement->second.byteOffsetBegin << " | size: " << nextElement->second.byteSize << "| endOffset: " << nextElement->second.byteOffsetEnd << std::endl;
				//std::cout << std::endl;

				// change appropriate values in the current element
				targetedElement->second.replaceable = 1;	// flag it as being not replaceable, if it wasn't already
				targetedElement->second.collectionKey = nextElement->second.collectionKey;
				targetedElement->second.byteSize += memoryOffsetChange;
				targetedElement->second.byteOffsetEnd += memoryOffsetChange;
				//std::cout << "!!! Right before offset change: " << nextElement->second.byteOffsetBegin << std::endl;
				nextElement->second.byteOffsetBegin += memoryOffsetChange;
				nextElement->second.byteOffsetEnd += memoryOffsetChange;
				//std::cout << ">> memory offset change was: " << memoryOffsetChange << std::endl;
				//std::cout << ">> next element's begin offset will be: ### " << nextElement->second.byteOffsetBegin << std::endl;
				//std::cout << "downshifted: " << targetedElement->first << "| Begin: " << targetedElement->second.byteOffsetBegin << " | End: " << targetedElement->second.byteOffsetEnd << std::endl;
				//std::cout << std::endl;
				targetedElement++;
				nextElement++;

			}

			// remove the last element in the map
			int removalID = lastElement->first;
			terrainTracker.erase(removalID);
			maxIndex--;

		}
		else if (foundElementKey == lastElementKey)
		{
			//std::cout << "----> !! Pre-erase element count is: " << terrainTracker.size() << std::endl;
			//std::cout << "Unused Replaceable detected (last element)!!!" << std::endl;
			terrainTracker.erase(foundElementKey);	// erase the element
			//std::cout << "----> !! Post-erase element count is: " << terrainTracker.size() << std::endl;
			maxIndex--;		// decrement the max index
		}
	}
	else if (replaceAbleCount == 0)
	{
		std::cout << "No unused replaceables..." << std::endl;
	}
	return returnMeta;
}

void TerrainMemoryTracker::outputAllElements()
{
	std::map<int, TerrainMemoryMeta>::iterator terrainBegin = terrainTracker.begin();
	std::map<int, TerrainMemoryMeta>::iterator terrainEnd = terrainTracker.end();
	for (terrainBegin; terrainBegin != terrainEnd; terrainBegin++)
	{
		//std::cout << terrainBegin->first << ": Key: (" << terrainBegin->second.collectionKey.x << ", " << terrainBegin->second.collectionKey.y << ", " << terrainBegin->second.collectionKey.z << ") | BeginOffset: " << terrainBegin->second.byteOffsetBegin << " | Byte Size: " << terrainBegin->second.byteSize << " | EndOffset: " << terrainBegin->second.byteOffsetEnd << std::endl;
	}
}

int TerrainMemoryTracker::getCollectionOffset(EnclaveKeyDef::EnclaveKey in_collectionKey)
{
	std::map<int, TerrainMemoryMeta>::iterator terrainBegin = terrainTracker.begin();
	std::map<int, TerrainMemoryMeta>::iterator terrainEnd = terrainTracker.end();
	for (terrainBegin; terrainBegin != terrainEnd; terrainBegin++)
	{
		if (in_collectionKey == terrainBegin->second.collectionKey)
		{
			//std::cout << "Target key acquired for offset: (" << in_collectionKey.x << ", " << in_collectionKey.y << ", " << in_collectionKey.z << std::endl;
			return terrainBegin->second.byteOffsetBegin;
		}
	}

	return 0;	// ++should never return, makes compiler happy (won't produce a warning)
}

int TerrainMemoryTracker::getNumberOfUnusedReplaceables()
{
	int totalReplaceables = 0;	// the current number of replacables remaining
	std::map<int, TerrainMemoryMeta>::iterator terrainBegin = terrainTracker.begin();
	std::map<int, TerrainMemoryMeta>::iterator terrainEnd = terrainTracker.end();
	for (terrainBegin; terrainBegin != terrainEnd; terrainBegin++)
	{
		if (terrainBegin->second.replaceable == 0) // we found the key, time to do stuff
		{
			totalReplaceables++;
		}
	}
	return totalReplaceables;
}

TerrainMemoryMoveMeta TerrainMemoryTracker::checkForMemoryMovements(TerrainJobResults in_terrainJobResults)
{
	TerrainMemoryMoveMeta returnMoveMeta;
	// check to see if there are any replaceable elements
	std::map<int, TerrainMemoryMeta>::iterator terrainBegin = terrainTracker.begin();
	std::map<int, TerrainMemoryMeta>::iterator terrainEnd = terrainTracker.end();
	int wasReplaceableFound = 0;		// will indicate if there was a replaceable element found
	int foundKey = 0;					// the key of the found element, that we will start 
	for (terrainBegin; terrainBegin != terrainEnd; terrainBegin++)
	{
		if (terrainBegin->second.replaceable == 0) // we found the key, time to do stuff
		{
			wasReplaceableFound = 1;	// set replaceable flag
			foundKey = terrainBegin->first;
			break;						// break out of the loop
		}
	}

	// check to see if any elements match the parameter key
	std::map<int, TerrainMemoryMeta>::iterator terrainBegin2 = terrainTracker.begin();
	int doesElementAlreadyExist = 0;
	int alreadyExistingKey = 0;
	for (terrainBegin2; terrainBegin2 != terrainEnd; terrainBegin2++)
	{
		if (terrainBegin2->second.collectionKey == in_terrainJobResults.collectionKey)
		{
			doesElementAlreadyExist = 1;
			alreadyExistingKey = terrainBegin2->first;
			break;
		}
	}

	// if there was a replaceable found, check if it is the last element
	if (wasReplaceableFound == 1 && doesElementAlreadyExist == 0)
	{
		std::map<int, TerrainMemoryMeta>::reverse_iterator terrainEndCheck = terrainTracker.rbegin();
		int lastID = terrainEndCheck->first;
		if (lastID == foundKey)	 // this would indicate it's the last element in the list
		{
			returnMoveMeta.containsMovement = 0;
		}
		else if (lastID != foundKey)	// otherwise, it's not the last element in the list, so we need to get the values.
		{
			int totalJobBytes = in_terrainJobResults.numberOfTrianglesProduced*triangleSizeInGLBytes;
			if (terrainTracker[foundKey].byteSize != totalJobBytes)
			{
				int beginningOffset = terrainBegin->second.byteOffsetEnd;		// get the beginning of the chunk of data to swap to the buffer
				int endingOffset = terrainEndCheck->second.byteOffsetEnd;
				returnMoveMeta.byteOffset = beginningOffset;
				returnMoveMeta.byteSize = endingOffset - beginningOffset;
				returnMoveMeta.containsMovement = 1;
			}
			else if (terrainTracker[foundKey].byteSize == totalJobBytes)
			{
				returnMoveMeta.containsMovement = 0;
			}
		}
	}

	// if the element already exists, do this instead
	else if (doesElementAlreadyExist == 1)
	{
		std::map<int, TerrainMemoryMeta>::reverse_iterator terrainEndCheck = terrainTracker.rbegin();
		int lastID = terrainEndCheck->first;
		if (lastID == alreadyExistingKey)	 // this would indicate it's the last element in the list
		{
			returnMoveMeta.containsMovement = 0;
		}
		else if (lastID != alreadyExistingKey)
		{
			int totalJobBytes = in_terrainJobResults.numberOfTrianglesProduced*triangleSizeInGLBytes;
			if (terrainTracker[alreadyExistingKey].byteSize != totalJobBytes)	// flag for movement, only if the byte sizes are different
			{
				int beginningOffset = terrainBegin2->second.byteOffsetEnd;		// get the beginning of the chunk of data to swap to the buffer
				int endingOffset = terrainEndCheck->second.byteOffsetEnd;
				returnMoveMeta.byteOffset = beginningOffset;
				returnMoveMeta.byteSize = endingOffset - beginningOffset;
				returnMoveMeta.containsMovement = 1;
			}
			else if (terrainTracker[alreadyExistingKey].byteSize == totalJobBytes)
			{
				returnMoveMeta.containsMovement = 0;
			}
		}
	}

	return returnMoveMeta;
}