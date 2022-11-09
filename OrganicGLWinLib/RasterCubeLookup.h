#pragma once

#ifndef RASTERCUBELOOKUP_H
#define RASTERCUBELOOKUP_H

#include "RasterKeyLookupByDim.h"

class RasterCubeLookup
{
	public:
		void insertCubeKey(EnclaveKeyDef::EnclaveKey in_cubeKey)
		{
			// insert into X
			xLookup.insert(in_cubeKey.x, in_cubeKey.y, in_cubeKey.z);

			// insert into Y
			yLookup.insert(in_cubeKey.y, in_cubeKey.x, in_cubeKey.z);

			// insert into Z
			zLookup.insert(in_cubeKey.z, in_cubeKey.x, in_cubeKey.y);

		}

		bool lookupX(EnclaveKeyDef::EnclaveKey in_lookupKey)
		{
			std::cout << "Key found via X lookup." << std::endl;
			return xLookup.doesKeyExist(in_lookupKey.x, in_lookupKey.y, in_lookupKey.z);
		}

		bool lookupY(EnclaveKeyDef::EnclaveKey in_lookupKey)
		{
			std::cout << "Key found via Y lookup." << std::endl;
			return yLookup.doesKeyExist(in_lookupKey.y, in_lookupKey.x, in_lookupKey.z);
		}

		bool lookupZ(EnclaveKeyDef::EnclaveKey in_lookupKey)
		{
			std::cout << "Key found via Z lookup." << std::endl;
			return zLookup.doesKeyExist(in_lookupKey.z, in_lookupKey.x, in_lookupKey.y);
		}

		// For X dimension
		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> fetchXSlice(int in_xValue)
		{
			//std::cout << "EnclaveKeys for slice at X of " << in_xValue << "are: " << std::endl;
			std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> returnKeys;
			auto currentSlice = xLookup.lookup[in_xValue];
			auto currentSliceBegin = currentSlice.begin();
			auto currentSliceEnd = currentSlice.end();
			for (; currentSliceBegin != currentSliceEnd; currentSliceBegin++)
			{
				EnclaveKeyDef::EnclaveKey newKey(in_xValue, currentSliceBegin->a, currentSliceBegin->b);
				returnKeys.insert(newKey);
				//std::cout << "(" << newKey.x << ", " << newKey.y << ", " << newKey.z << ") " << std::endl;
			}
			return returnKeys;
		}

		void printXKeys()
		{
			auto xValueBegin = xLookup.lookup.begin();
			auto xValueEnd = xLookup.lookup.end();
			for (; xValueBegin != xValueEnd; xValueBegin++)
			{
				auto currentSlice = xValueBegin->second;
				auto currentSliceBegin = currentSlice.begin();
				auto currentSliceEnd = currentSlice.end();
				for (; currentSliceBegin != currentSliceEnd; currentSliceBegin++)
				{
					EnclaveKeyDef::EnclaveKey newKey(xValueBegin->first, currentSliceBegin->a, currentSliceBegin->b);
					std::cout << "(" << newKey.x << ", " << newKey.y << ", " << newKey.z << ") " << std::endl;
				}
			}
		}

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> fetchYSlice(int in_yValue)
		{
			//std::cout << "EnclaveKeys for slice at Y of " << in_yValue << "are: " << std::endl;
			std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> returnKeys;
			auto currentSlice = yLookup.lookup[in_yValue];
			auto currentSliceBegin = currentSlice.begin();
			auto currentSliceEnd = currentSlice.end();
			for (; currentSliceBegin != currentSliceEnd; currentSliceBegin++)
			{
				EnclaveKeyDef::EnclaveKey newKey(currentSliceBegin->a, in_yValue, currentSliceBegin->b);
				returnKeys.insert(newKey);
				//std::cout << "(" << newKey.x << ", " << newKey.y << ", " << newKey.z << ") " << std::endl;
			}
			return returnKeys;
		}

		std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> fetchZSlice(int in_zValue)
		{
			//std::cout << "EnclaveKeys for slice at Z of " << in_zValue << "are: " << std::endl;
			std::unordered_set<EnclaveKeyDef::EnclaveKey, EnclaveKeyDef::KeyHasher> returnKeys;
			auto currentSlice = zLookup.lookup[in_zValue];
			auto currentSliceBegin = currentSlice.begin();
			auto currentSliceEnd = currentSlice.end();
			for (; currentSliceBegin != currentSliceEnd; currentSliceBegin++)
			{
				EnclaveKeyDef::EnclaveKey newKey(currentSliceBegin->a, currentSliceBegin->b, in_zValue);
				returnKeys.insert(newKey);
				//std::cout << "(" << newKey.x << ", " << newKey.y << ", " << newKey.z << ") " << std::endl;
			}
			return returnKeys;
		}

		RasterKeyLookupByDim* fetchRawXLookup()
		{
			return &xLookup;
		}

	private:
		friend class RTriangle;
		RasterKeyLookupByDim xLookup;
		RasterKeyLookupByDim yLookup;
		RasterKeyLookupByDim zLookup;
};

#endif
