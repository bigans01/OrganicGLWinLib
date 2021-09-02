#include "stdafx.h"
#include "MassGridArray.h"

int MassGridArray::getArraySize()
{
	return dimensionSize * dimensionSize * dimensionSize;
}

bool MassGridArray::doesDownfillColumnHaveTermination(int in_x, int in_z)
{
	bool wasTerminationValid = false;

	// start from bottom, scan upwards
	bool wasUpfillFlagFound = false;
	bool wasDownfillFlagFound = false;

	int upfillCount = 0;
	int downfillCount = 0;
	int innerMassCount = 0;

	std::vector<EnclaveKeyDef::EnclaveKey> downfillKeys;
	std::vector<EnclaveKeyDef::EnclaveKey> upfillKeys;

	for (int y = 0; y < dimensionSize; y++)
	{
		MassGridSearchResult currentSearchResult = searchForCell(in_x, y, in_z);
		if (currentSearchResult.wasSearchKeyValid == true)
		{
			if (currentSearchResult.cellRef->isFlagSet(MassCellBitFlags::UPFILL_CRUST))
			{
				upfillCount++;
				EnclaveKeyDef::EnclaveKey upfillKey(in_x, y, in_z);
				upfillKeys.push_back(upfillKey);
				wasUpfillFlagFound = true;
			}
			else if (currentSearchResult.cellRef->isFlagSet(MassCellBitFlags::DOWNFILL_CRUST))
			{
				downfillCount++;
				EnclaveKeyDef::EnclaveKey downfillKey(in_x, y, in_z);
				downfillKeys.push_back(downfillKey);
				wasDownfillFlagFound = true;
			}

			// check inner mass flags
			if
				(
					currentSearchResult.cellRef->isFlagSet(MassCellBitFlags::INNER_MASS)
					)
			{
				innerMassCount++;
			}
		}
	}

	std::cout << ">>> Downfill found: " << wasDownfillFlagFound << std::endl;
	std::cout << ">>> Upfill found: " << wasUpfillFlagFound << std::endl;

	std::cout << ">: upfillCount: " << upfillCount << std::endl;
	std::cout << ">: downfillCount: " << downfillCount << std::endl;
	std::cout << ">: innerMassCount: " << innerMassCount << std::endl;

	std::cout << ">:: Downfill keys: " << std::endl;
	auto downfillKeysBegin = downfillKeys.begin();
	auto downfillKeysEnd = downfillKeys.end();
	for (; downfillKeysBegin != downfillKeysEnd; downfillKeysBegin++)
	{
		std::cout << "(" << downfillKeysBegin->x << ", " << downfillKeysBegin->y << ", " << downfillKeysBegin->z << ")" << std::endl;
	}

	std::cout << ">:: Upfill keys: " << std::endl;
	auto upfillKeysBegin = upfillKeys.begin();
	auto upfillKeysEnd = upfillKeys.end();
	for (; upfillKeysBegin != upfillKeysEnd; upfillKeysBegin++)
	{
		std::cout << "(" << upfillKeysBegin->x << ", " << upfillKeysBegin->y << ", " << upfillKeysBegin->z << ")" << std::endl;
	}


	if
		(
			wasUpfillFlagFound == true
			&&
			wasDownfillFlagFound == true
			)
	{
		wasTerminationValid = true;
	}
	return wasTerminationValid;
}

void MassGridArray::executeDownfills()
{
	auto downfillExecutionBegin = std::chrono::high_resolution_clock::now();
	int arraySize = dimensionSize * dimensionSize* dimensionSize;
	int sillyCount = 0;
	for (int x = 0; x < arraySize; x++)
	{
		//if (massCellArray[x].isFlagSet(MassCellBitFlags::DOWNFILL_CRUST) == true)
		//{
			//sillyCount++;
		//}

		if (massCellArray[x].isDownfillRunnable() == true)
		{
			bool debugDownfillFlag = false;

			EnclaveKeyDef::EnclaveKey cellLocation = convertIndexToBlockKey(x);
			//std::cout << "!!! found down fill to run; location is: (" << cellLocation.x << ", " << cellLocation.y << ", " << cellLocation.z << ") " << std::endl;
			//int foundDownfill = 3;
			//std::cin >> foundDownfill;

			// In order for a downfill to execute, the cellLocation can't be at the bottom value, 
			// AND 
			// there must be a terminiating upfill value in this same X/Z column.
			if
				(
				(cellLocation.y != 0)
					&&
					(doesCellLocationHaveTerminatingUpfill(cellLocation) == true)
					)
			{
				EnclaveKeyDef::EnclaveKey lastFilledCell(cellLocation.x, cellLocation.y - 1, cellLocation.z);
				MassGridSearchResult currentSearchResult = searchForCell(lastFilledCell.x, lastFilledCell.y, lastFilledCell.z);
				bool continueDownfillProcess = true;
				if (currentSearchResult.wasSearchKeyValid == true)
				{
					currentSearchResult.cellRef->setFlag(MassCellBitFlags::INNER_MASS, 1);
					if (currentSearchResult.cellRef->isFlagSet(MassCellBitFlags::UPFILL_CRUST) == true)	// stop running the downfill if we encounter the upfill crust bit.
					{
						continueDownfillProcess = false;
					}
				}

				// decrement the y value of the lastFilledCell
				lastFilledCell.y--;

				// keep running until either the lastFilledCell.y becomes 0, or the run terminates.
				while
					(
					(lastFilledCell.y >= 0)
						&&
						(continueDownfillProcess == true)
						)
				{
					MassGridSearchResult fillSearchResult = searchForCell(lastFilledCell.x, lastFilledCell.y, lastFilledCell.z);
					if (fillSearchResult.wasSearchKeyValid == true)
					{
						fillSearchResult.cellRef->setFlag(MassCellBitFlags::INNER_MASS, 1);
						if (fillSearchResult.cellRef->isFlagSet(MassCellBitFlags::UPFILL_CRUST) == true)	// stop running the downfill if we encounter the upfill crust bit.
						{
							continueDownfillProcess = false;
						}
					}
					lastFilledCell.y--;	// decrement for next loop.
				}
			}
		}



	}
	auto downfillExecutionEnd = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> downfillElapsed = downfillExecutionEnd - downfillExecutionBegin;
	//std::cout << "--------------" << std::endl;

	//std::cout << "!!! Downfill runtime: " << downfillElapsed.count() << std::endl;
	//std::cout << "!!! Number of downfill crust bits set: " << sillyCount << std::endl;
	//int downfillSillyWait = 3;
	//std::cin >> downfillSillyWait;
}

void MassGridArray::buildArray(int in_dimensionSize)
{
	dimensionSize = in_dimensionSize;
	massCellArray.reset(new MassGridArrayCell[dimensionSize*dimensionSize*dimensionSize]);
}

MassGridArrayCell* MassGridArray::getArrayRef()
{
	return massCellArray.get();
}

bool MassGridArray::doesCellLocationHaveTerminatingUpfill(EnclaveKeyDef::EnclaveKey in_keyToScan)
{
	bool hasTerminatingUpfill = false;
	int startX = in_keyToScan.x;
	int startY = in_keyToScan.y;
	int startZ = in_keyToScan.z;

	for (int y = startY; y > -1; y--)
	{
		MassGridSearchResult attemptedSearch = searchForCell(startX, y, startZ);
		if (attemptedSearch.wasSearchKeyValid == true)
		{
			if (attemptedSearch.cellRef->isFlagSet(MassCellBitFlags::UPFILL_CRUST) == true)
			{
				hasTerminatingUpfill = true;
			}
		}
	}
	return hasTerminatingUpfill;
}

void MassGridArray::getPercentAgeOfCellsWithDownfillAtTopYLayer()
{
	int totalCells = dimensionSize * dimensionSize;
	int downfillCount = 0;
	for (int x = 0; x < dimensionSize; x++)
	{
		for (int z = 0; z < dimensionSize; z++)
		{
			MassGridSearchResult currentYCell = searchForCell(x, dimensionSize - 1, z);
			if (currentYCell.wasSearchKeyValid == true)
			{
				if (currentYCell.cellRef->isDownfillRunnable() == true)
				{
					downfillCount++;
				}
			}
		}
	}

	float percentage = float(downfillCount) / float(totalCells);

	std::cout << ">>>>>>> Percentage of downfill'ed cells at top Y: " << percentage << std::endl;
}

MassGridSearchResult MassGridArray::searchForCell(int in_x, int in_y, int in_z)
{
	MassGridSearchResult searchResult;
	if
		(
			// search must be within valid bounds of array
			!(
			(in_x < 0)
				||
				(in_x > (dimensionSize - 1))

				||
				(in_y < 0)
				||
				(in_y > (dimensionSize - 1))

				||
				(in_z < 0)
				||
				(in_z > (dimensionSize - 1))
				)
			)
	{
		MassGridSearchResult foundResult(true, getMassArrayDataRef(in_x, in_y, in_z));
		searchResult = foundResult;
	}
	else
	{
		//std::cout << "!!! Warning, key NOT VALID; key was " << in_x << ", " << in_y << ", " << in_z << " | dim size was: " << dimensionSize << std::endl;
	}
	return searchResult;
}

bool MassGridArray::wereCellsDiscoveredInArea(MassGridArrayCellScanArea in_scanArea)
{
	bool wereCellsDiscovered = false;

	//std::cout << "Start scan point: " << in_scanArea.scanKeyA.x << ", " << in_scanArea.scanKeyA.y << ", " << in_scanArea.scanKeyA.z << std::endl;
	//std::cout << "End scan point: " << in_scanArea.scanKeyB.x << ", " << in_scanArea.scanKeyB.y << ", " << in_scanArea.scanKeyB.z << std::endl;

	int start_x = in_scanArea.scanKeyA.x;
	int start_y = in_scanArea.scanKeyA.y;
	int start_z = in_scanArea.scanKeyA.z;

	int end_x = in_scanArea.scanKeyB.x;
	int end_y = in_scanArea.scanKeyB.y;
	int end_z = in_scanArea.scanKeyB.z;

	// this was a nasty off-by-one error, corrected on 8/25/2021 (x/y/z end values weren't set as + 1).
	for (int x = start_x; x < end_x + 1; x++)
	{
		for (int y = start_y; y < end_y + 1; y++)
		{
			for (int z = start_z; z < end_z + 1; z++)
			{
				MassGridSearchResult currentSearchResult = searchForCell(x, y, z);
				if (currentSearchResult.wasSearchKeyValid == true)
				{
					MassGridArrayCell* currentRef = currentSearchResult.cellRef;
					//if (currentRef->isFlagSet(MassCellBitFlags::INNER_MASS))
					if (currentRef->getNumberOfFlagsSet() > 0)
					{
						wereCellsDiscovered = true;
						goto END;
					}
				}
			}
		}
	}
	END:return wereCellsDiscovered;
}

int MassGridArray::getNumberOfPopulatedCells()
{
	int arraySize = dimensionSize * dimensionSize * dimensionSize;
	int count = 0;
	for (int x = 0; x < arraySize; x++)
	{
		if (massCellArray[x].getNumberOfFlagsSet() > 0)
		{
			count++;
		}
	}
	return count;
}

MassGridArrayCell* MassGridArray::getMassArrayDataRef(int in_x, int in_y, int in_z)
{
	int x = in_x * (dimensionSize * dimensionSize);
	int y = in_y * (dimensionSize);
	return &massCellArray[x + y + in_z];
}

EnclaveKeyDef::EnclaveKey MassGridArray::convertIndexToBlockKey(int in_index)
{
	int x = in_index / (dimensionSize * dimensionSize);
	int remainder_x = in_index % (dimensionSize * dimensionSize);

	int y = remainder_x / dimensionSize;
	int remainder_y = remainder_x % dimensionSize;

	int z = remainder_y;

	EnclaveKeyDef::EnclaveKey returnKey(x, y, z);
	return returnKey;
}