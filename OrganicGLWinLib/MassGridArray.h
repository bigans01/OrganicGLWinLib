#pragma once

#ifndef MASSGRIDARRAY_H
#define MASSGRIDARRAY_H

#include <mutex>
#include "MassGridArrayCell.h"

class MassGridArray
{
	public:
		void buildArray(int in_dimensionSize)
		{
			dimensionSize = in_dimensionSize;
			massCellArray.reset(new MassGridArrayCell[dimensionSize* dimensionSize* dimensionSize]);
		}

		MassGridArrayCell* getMassArrayDataRef(int in_x, int in_y, int in_z)
		{
			int x = in_x * (dimensionSize * dimensionSize);
			int y = in_y * (dimensionSize);
			return &massCellArray[x + y + in_z];
		}
	private:
		std::unique_ptr<MassGridArrayCell[]> massCellArray;
		int dimensionSize = 0;
};

#endif
