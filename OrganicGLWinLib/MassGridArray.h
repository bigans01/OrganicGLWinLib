#pragma once

#ifndef MASSGRIDARRAY_H
#define MASSGRIDARRAY_H

#include <mutex>
#include "MassGridArrayCell.h"
#include "MassGridSearchResult.h"
#include "MassGridArrayCellScanArea.h"
#include "EnclaveKeyDef.h"
#include <chrono>

class MassGridArray
{
	public:
		int getArraySize()
		{
			return dimensionSize * dimensionSize * dimensionSize;
		}

		void executeDownfills()
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

					EnclaveKeyDef::EnclaveKey cellLocation = convertIndexToBlockKey(x);
					//std::cout << "!!! found down fill to run; location is: (" << cellLocation.x << ", " << cellLocation.y << ", " << cellLocation.z << ") " << std::endl;
					//int foundDownfill = 3;
					//std::cin >> foundDownfill;

					// don't bother doing a downfill if the y is at 0.
					if (cellLocation.y != 0)
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

			std::cout << "!!! Downfill runtime: " << downfillElapsed.count() << std::endl;
			std::cout << "!!! Number of downfill crust bits set: " << sillyCount << std::endl;
			int downfillSillyWait = 3;
			std::cin >> downfillSillyWait;
		}

		void buildArray(int in_dimensionSize)
		{
			dimensionSize = in_dimensionSize;
			massCellArray.reset(new MassGridArrayCell[dimensionSize*dimensionSize*dimensionSize]);
		}

		MassGridArrayCell* getArrayRef()
		{
			return massCellArray.get();
		}

		MassGridSearchResult searchForCell(int in_x, int in_y, int in_z)
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
				std::cout << "!!! Warning, key NOT VALID; key was " << in_x << ", " << in_y << ", " << in_z << " | dim size was: " << dimensionSize << std::endl;
			}
			return searchResult;
		}

		bool wereCellsDiscoveredInArea(MassGridArrayCellScanArea in_scanArea)
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

			for (int x = start_x; x < end_x; x++)
			{
				for (int y = start_y; y < end_y; y++)
				{
					for (int z = start_z; z < end_z; z++)
					{
						MassGridSearchResult currentSearchResult = searchForCell(x, y, z);
						if (currentSearchResult.wasSearchKeyValid == true)
						{
							MassGridArrayCell* currentRef = currentSearchResult.cellRef;
							if (currentRef->isFlagSet(MassCellBitFlags::INNER_MASS))
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
		
	private:
		friend class RCollisionPointArray;
		std::unique_ptr<MassGridArrayCell[]> massCellArray;
		int dimensionSize = 0;
		MassGridArrayCell* getMassArrayDataRef(int in_x, int in_y, int in_z)
		{
			int x = in_x * (dimensionSize * dimensionSize);
			int y = in_y * (dimensionSize);
			return &massCellArray[x + y + in_z];
		}

		EnclaveKeyDef::EnclaveKey convertIndexToBlockKey(int in_index)
		{
			int x = in_index / (dimensionSize * dimensionSize);
			int remainder_x = in_index % (dimensionSize * dimensionSize);

			int y = remainder_x / dimensionSize;
			int remainder_y = remainder_x % dimensionSize;

			int z = remainder_y;

			EnclaveKeyDef::EnclaveKey returnKey(x,y,z);
			return returnKey;
		}
};

#endif
