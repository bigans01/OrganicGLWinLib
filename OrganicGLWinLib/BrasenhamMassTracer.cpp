#include "stdafx.h"
#include "BrasenhamMassTracer.h"

void BrasenhamMassTracer::setDownfillCrustBit(short in_downfillCrustBitValue)
{
	downfillCrustBitValue = in_downfillCrustBitValue;
}

void BrasenhamMassTracer::setUpfillCrustBit(short in_upfillCrustBitValue)
{
	upfillCrustBitValue = in_upfillCrustBitValue;
}

EnclaveKeyDef::EnclaveKey BrasenhamMassTracer::getCellLocation(TileLocation in_tileLocation)
{
	EnclaveKeyDef::EnclaveKey returnKey;
	switch (massTracingDim)
	{
		case BrasenhamMassTracerDim::X: 
		{
			EnclaveKeyDef::EnclaveKey selectedKey(massTracingSelectedDimLocation, in_tileLocation.x, in_tileLocation.y);
			returnKey = selectedKey;
			break;
		};
		case BrasenhamMassTracerDim::Y:
		{
			EnclaveKeyDef::EnclaveKey selectedKey(in_tileLocation.x, massTracingSelectedDimLocation, in_tileLocation.y);
			returnKey = selectedKey;
			break;
		};
		case BrasenhamMassTracerDim::Z:
		{
			EnclaveKeyDef::EnclaveKey selectedKey(in_tileLocation.x, in_tileLocation.y, massTracingSelectedDimLocation);
			returnKey = selectedKey;
			break;
		};
	};
	return returnKey;
}

void BrasenhamMassTracer::executeRun()
{
	if (abs(tracingLine.y1 - tracingLine.y0) < abs(tracingLine.x1 - tracingLine.x0))
	{
		if (tracingLine.x0 > tracingLine.x1)
		{
			// low, but swapped
			//std::cout << ">>> running low, swapped. " << std::endl;
			tracingLine.swapLine();
			plotLineLow();
		}
		else
		{
			// low
			//std::cout << ">>> running low, no swap. " << std::endl;
			plotLineLow();
		}
	}
	else
	{
		if (tracingLine.y0 > tracingLine.y1)
		{
			// high, but swapped
			//std::cout << ">>> running high, swapped. " << std::endl;
			tracingLine.swapLine();
			plotLineHigh();
		}
		else
		{
			//std::cout << ">>> running high, no swap. " << std::endl;
			//std::cout << ":::: calling high, no swap. " << std::endl;
			// high
			plotLineHigh();
		}
	}
}

void BrasenhamMassTracer::plotLineLow()
{
	int dx = tracingLine.x1 - tracingLine.x0;
	int dy = tracingLine.y1 - tracingLine.y0;
	int yi = 1;		// represents the y direction, can be 1 or -1
	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}
	int D = (2 * dy) - dx;
	int y = tracingLine.y0;
	int previous_x = tracingLine.x0;
	for (int x = tracingLine.x0; x < tracingLine.x1 + 1; x++)
	{
		// update tile
		TileLocation currentLocation(x, y);
		EnclaveKeyDef::EnclaveKey currentCellKey = getCellLocation(currentLocation);
		updateMassGridCell(currentCellKey);
		//std::cout << "plotLineLow, tile location to insert is: " << currentLocation.x << ", " << currentLocation.y << std::endl;

		if (D > 0)
		{
			// be sure to fill previous tile location
			int previous_y = y;
			y = y + yi;
			D = D + (2 * (dy - dx));

			// Line fattening -- can only do these operations if we aren't on the last iteration
			if (x < tracingLine.x1)
			{

				TileLocation previousTileLocation(previous_x, y);
				EnclaveKeyDef::EnclaveKey previousTileLocationKey = getCellLocation(previousTileLocation);
				updateMassGridCell(previousTileLocationKey);
				//std::cout << "plotLineLow, option #2 insert: " << previousTileLocation.x << ", " << previousTileLocation.y << std::endl;

				if (x < (brasenhamTilesPerDim - 1))
				{
					TileLocation previousTileLocation2(x + 1, previous_y);
					EnclaveKeyDef::EnclaveKey previousTileLocation2Key = getCellLocation(previousTileLocation2);
					updateMassGridCell(previousTileLocation2Key);
					//std::cout << "plotLineLow, option #3 insert: " << previousTileLocation2.x << ", " << previousTileLocation2.y << std::endl;
				}
			}
		}
		else
		{
			D = D + 2 * dy;
		}
		previous_x = x;
	}
}

void BrasenhamMassTracer::plotLineHigh()
{
	int dx = tracingLine.x1 - tracingLine.x0;
	int dy = tracingLine.y1 - tracingLine.y0;
	int xi = 1;
	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}
	int D = (2 * dx) - dy;
	int x = tracingLine.x0;
	int previous_y = tracingLine.y0;
	for (int y = tracingLine.y0; y < tracingLine.y1 + 1; y++)
	{
		TileLocation currentLocation(x, y);
		EnclaveKeyDef::EnclaveKey currentCellKey = getCellLocation(currentLocation);
		updateMassGridCell(currentCellKey);
		//std::cout << "plotLineHigh, tile location to insert is: " << currentLocation.x << ", " << currentLocation.y << std::endl;

		if (D > 0)
		{
			// be sure to fill previous tile location
			//std::cout << "Shift detected..." << std::endl;

			int previous_x = x;
			x = x + xi;
			D = D + (2 * (dx - dy));

			//std::cout << "new x: " << x << std::endl;
			//std::cout << "previous y: " << previous_y << std::endl;


			// Line fattening -- can only do these operations if we aren't on the last iteration
			if (y < tracingLine.y1)
			{
				TileLocation previousTileLocation(x, previous_y);
				EnclaveKeyDef::EnclaveKey previousTileLocationKey = getCellLocation(previousTileLocation);
				updateMassGridCell(previousTileLocationKey);
				//std::cout << "plotLineHigh, option #2 insert: " << previousTileLocation.x << ", " << previousTileLocation.y << " | current y value: " << y << " | y-max: " << tracingLine.y1 + 1 << std::endl;

				if (y < (brasenhamTilesPerDim - 1))
				{
					TileLocation previousTileLocation2(previous_x, y + 1);
					EnclaveKeyDef::EnclaveKey previousTileLocation2Key = getCellLocation(previousTileLocation2);
					updateMassGridCell(previousTileLocation2Key);
					//std::cout << "plotLineHigh, option #3 insert: " << previousTileLocation2.x << ", " << previousTileLocation2.y << std::endl;
				}
			}
		}
		else
		{
			D = D + 2 * dx;
		}
		previous_y = y;
	}
}

void BrasenhamMassTracer::updateMassGridCell(EnclaveKeyDef::EnclaveKey in_key)
{
	MassGridSearchResult result = massGridArrayRef->searchForCell(in_key.x, in_key.y, in_key.z);
	if (result.wasSearchKeyValid == true)
	{

		result.cellRef->setFlag(MassCellBitFlags::CRUST_MASS, 1);
		result.cellRef->setFlag(MassCellBitFlags::INNER_MASS, 1);
		result.cellRef->setFlagNoOverrideIfActive(MassCellBitFlags::DOWNFILL_CRUST, downfillCrustBitValue);
		result.cellRef->setFlagNoOverrideIfActive(MassCellBitFlags::UPFILL_CRUST, upfillCrustBitValue);
		result.cellRef->setCellMaterialID(brasenhamMaterialID);
	}

}