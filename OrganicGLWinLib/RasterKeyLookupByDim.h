#pragma once

#ifndef RASTERKEYLOOKUPBYDIM_H
#define RASTERKEYLOOKUPBYDIM_H


class RasterKeyLookupByDim
{
	public:
		void insert(int in_lookupDimKey, int in_otherValueA, int in_otherValueB)
		{
			EnclaveKeyDef::Enclave2DKey otherTwoDims(in_otherValueA, in_otherValueB);
			lookup[in_lookupDimKey].insert(otherTwoDims);
		};


		bool doesKeyExist(int in_lookupDimKey, int in_otherValueA, int in_otherValueB)
		{

			bool wasKeyFound = false;
			auto lookupKeyFinder = lookup.find(in_lookupDimKey);
			if (lookupKeyFinder != lookup.end())	// it was found
			{
				//EnclaveKeyDef::Enclave2DKey keyToMatchAgainst(in_otherValueA, in_otherValueB);
				auto otherValuesLookupBegin = lookupKeyFinder->second.begin();
				auto otherValuesLookupEnd = lookupKeyFinder->second.end();
				for (; otherValuesLookupBegin != otherValuesLookupEnd; otherValuesLookupBegin++)
				{
					EnclaveKeyDef::Enclave2DKey currentKey = *otherValuesLookupBegin;
					if (currentKey.equals(in_otherValueA, in_otherValueB) == true)
					{
						wasKeyFound = true;
						break;
					}
				}

			}
			return wasKeyFound;

		}

		std::map<int, std::unordered_set<EnclaveKeyDef::Enclave2DKey, EnclaveKeyDef::KeyHasher>> lookup;
};

#endif
