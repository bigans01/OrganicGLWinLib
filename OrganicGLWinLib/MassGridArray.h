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
		int getArraySize();
		bool doesDownfillColumnHaveTermination(int in_x, int in_z);
		void executeDownfills();
		void buildArray(int in_dimensionSize);
		MassGridArrayCell* getArrayRef();
		void getPercentAgeOfCellsWithDownfillAtTopYLayer();
		MassGridSearchResult searchForCell(int in_x, int in_y, int in_z);
		bool wereCellsDiscoveredInArea(MassGridArrayCellScanArea in_scanArea);
		int getNumberOfPopulatedCells();
	private:
		bool doesCellLocationHaveTerminatingUpfill(EnclaveKeyDef::EnclaveKey in_keyToScan);
		MassGridArrayCell* getMassArrayDataRef(int in_x, int in_y, int in_z);
		EnclaveKeyDef::EnclaveKey convertIndexToBlockKey(int in_index);

		friend class RCollisionPointArray;

		std::unique_ptr<MassGridArrayCell[]> massCellArray;
		int dimensionSize = 0;
};

#endif
