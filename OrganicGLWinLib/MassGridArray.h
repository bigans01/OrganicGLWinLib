#pragma once

#ifndef MASSGRIDARRAY_H
#define MASSGRIDARRAY_H

#include "MassGridArrayCell.h"
#include "MassGridSearchResult.h"
#include "MassGridArrayCellScanArea.h"
#include <chrono>

class MassGridArray
{
	public:
		int getArraySize();
		bool doesDownfillColumnHaveTermination(int in_x, int in_z);
		void executeDownfills();
		void buildArray(int in_dimensionSize);
		bool wereCellsDiscoveredInArea(MassGridArrayCellScanArea in_scanArea);
		bool wasFlagDiscoveredInArea(MassGridArrayCellScanArea in_scanArea, MassCellBitFlags in_bitFlag);
		int getNumberOfPopulatedCells();
		void getPercentAgeOfCellsWithDownfillAtTopYLayer();

		//debug functions
		void printColumnMaterialValues(int in_x, int in_z);		// prints the materials of each cell in a x/z column, starting from y = 0 and going up.
		void getStatsOfXZCellsWithValidDownfill();				// prints the total number of x/z columns with valid downfill.
		void getStatsOfXZCellsWithDownfillFlags();				// prints the total number of x/z columns having the downfill flag set.

		MassGridSearchResult searchForCell(int in_x, int in_y, int in_z);
		MassGridArrayCell* getArrayRef();
	private:
		bool doesCellLocationHaveTerminatingUpfill(EnclaveKeyDef::EnclaveKey in_keyToScan);
		MassGridArrayCell* getMassArrayDataRef(int in_x, int in_y, int in_z);
		EnclaveKeyDef::EnclaveKey convertIndexToBlockKey(int in_index);

		friend class RCollisionPointArray;

		std::unique_ptr<MassGridArrayCell[]> massCellArray;
		int dimensionSize = 0;
};

#endif
