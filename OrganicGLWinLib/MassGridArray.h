#pragma once

#ifndef MASSGRIDARRAY_H
#define MASSGRIDARRAY_H

#include <mutex>
#include "MassGridArrayCell.h"
#include "MassGridSearchResult.h"

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
