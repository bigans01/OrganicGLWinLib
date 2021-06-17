#pragma once

#ifndef MASSGRIDARRAY_H
#define MASSGRIDARRAY_H

#include <mutex>
#include "MassGridArrayCell.h"
#include "MassGridSearchResult.h"
#include "MassGridArrayCellScanArea.h"

class MassGridArray
{
	public:
		void buildArray(int in_dimensionSize)
		{
			dimensionSize = in_dimensionSize;
			massCellArray.reset(new MassGridArrayCell[dimensionSize*dimensionSize*dimensionSize]);
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
							if (currentRef->isFlagSet(MassCellBitFlags::LINE_MASS))
							{
								std::cout << "!!! Notice: cell discovered. " << std::endl;
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
		std::unique_ptr<MassGridArrayCell[]> massCellArray;
		int dimensionSize = 0;
		MassGridArrayCell* getMassArrayDataRef(int in_x, int in_y, int in_z)
		{
			int x = in_x * (dimensionSize * dimensionSize);
			int y = in_y * (dimensionSize);
			return &massCellArray[x + y + in_z];
		}
};

#endif
