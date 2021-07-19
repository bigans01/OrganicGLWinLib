#pragma once

#ifndef RTRIANGLE_H
#define RTRIANGLE_H

#include "RTriangleLine.h"
#include "MassGridArray.h"
#include "LookupByDimRegister.h"
#include <mutex>
#include <algorithm>
#include "RScanDim.h"

class RTriangle
{
	public:
		RTriangle();
		RTriangle(RTriangleLine in_line0, RTriangleLine in_line1, RTriangleLine in_line2);
		RTriangle(const RTriangle& in_triangleB);
		RTriangle& operator=(const RTriangle& in_triangleB)
		{
			for (int x = 0; x < 3; x++)
			{
				rLines[x] = in_triangleB.rLines[x];
			}

			areRegistersSet = in_triangleB.areRegistersSet;
			if (areRegistersSet == true)
			{
				
				xScanMeta.numberOfScans = in_triangleB.xScanMeta.numberOfScans;
				yScanMeta.numberOfScans = in_triangleB.yScanMeta.numberOfScans;
				zScanMeta.numberOfScans = in_triangleB.zScanMeta.numberOfScans;

				// copy data for the X dim
				xDimRegister.reset(new LookupByDimRegister[xScanMeta.numberOfScans]);
				for (int x = 0; x < xScanMeta.numberOfScans; x++)
				{
					xDimRegister[x] = in_triangleB.xDimRegister[x];
				}

				// "" Y dim 
				yDimRegister.reset(new LookupByDimRegister[yScanMeta.numberOfScans]);
				for (int x = 0; x < yScanMeta.numberOfScans; x++)
				{
					yDimRegister[x] = in_triangleB.yDimRegister[x];
				}

				// "" Z dim
				zDimRegister.reset(new LookupByDimRegister[zScanMeta.numberOfScans]);
				for (int x = 0; x < zScanMeta.numberOfScans; x++)
				{
					zDimRegister[x] = in_triangleB.zDimRegister[x];
				}
				
			}
			return *this;
		}

		void printRPoints();
		void traceRasterLines();
		void traceRasterLinesIntoGrid(MassGridArray* in_massGridArrayRef,
			glm::vec3 in_triangleEmptyNormal,
			float in_rPolyRCubeDimLength,
			float in_rPolyTilesPerDim,
			float in_rPolyTileWeightToHundredthFloatRatio);
	private:
		struct DimScanMeta
		{
			DimScanMeta() {};
			DimScanMeta(int in_numberOfScans, int in_dimStartvalue, int in_dimEndValue) :
				numberOfScans(in_numberOfScans),
				dimStartValue(in_dimStartvalue),
				dimEndValue(in_dimEndValue)
			{};

			int numberOfScans = 0;
			int dimStartValue = 0;
			int dimEndValue = 0;

			void swapToMin()	// the start value must always be less than the end value, for proper iteration.
			{
				if (dimStartValue > dimEndValue)
				{
					int tempDim = dimEndValue;
					dimEndValue = dimStartValue;
					dimStartValue = tempDim;
				}
			}
		};


		DimScanMeta xScanMeta, yScanMeta, zScanMeta;
		DimScanMeta determineScanMeta(int in_point0DimValue, int in_point1DimValue, int in_point2DimValue);
		void initializeXYZDimRegisters();
		void runXDimRegisterScan(MassGridArray* in_massGridArrayRef,
								float in_rPolyRCubeDimLength,
								float in_rPolyTilesPerDim,
								float in_rPolyTileWeightToHundredthFloatRatio, short in_downfillCrustBitValue,
			short in_upfillCrustBitValue, glm::vec3 in_emptyNormal);
		void runYDimRegisterScan(MassGridArray* in_massGridArrayRef,
								float in_rPolyRCubeDimLength,
								float in_rPolyTilesPerDim,
								float in_rPolyTileWeightToHundredthFloatRatio, short in_downfillCrustBitValue,
			short in_upfillCrustBitValue, glm::vec3 in_emptyNormal);
		void runZDimRegisterScan(MassGridArray* in_massGridArrayRef,
								float in_rPolyRCubeDimLength,
								float in_rPolyTilesPerDim,
								float in_rPolyTileWeightToHundredthFloatRatio, short in_downfillCrustBitValue,
			short in_upfillCrustBitValue, glm::vec3 in_emptyNormal);

		RTriangleLine rLines[3];
		std::unique_ptr<LookupByDimRegister[]> xDimRegister;
		std::unique_ptr<LookupByDimRegister[]> yDimRegister;
		std::unique_ptr<LookupByDimRegister[]> zDimRegister;
		bool areRegistersSet = false;

		void buildRegisters();
};

#endif
