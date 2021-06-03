#pragma once

#ifndef CLEAVESEQUENCERESOLVER_H
#define CLEAVESEQUENCERESOLVER_H

#include "EnclaveKeyDef.h"
#include <iostream>
#include "SPoly.h"

class CleaveSequenceResolver
{
	public:
		CleaveSequenceResolver() {};
		CleaveSequenceResolver(int in_tilesPerDimension, float in_dimensionLimit) :
			numberOfTilesPerDimension(in_tilesPerDimension),
			dimensionLimit(in_dimensionLimit)
		{
			tileGridWidth = dimensionLimit / numberOfTilesPerDimension;	//  i.e., for Encalve resolution, this would be:
																		// dimensionLimit = 4.0f
																		// numberOfTilesPerDiemnsion = 400
																		// tileGridWith = 0.01f
		};

		void addSPolyToResolve(SPoly in_sPolyToResolve);
		void addSPolyToCompare(SPoly in_comparingSPoly);
		EnclaveKeyDef::EnclaveKey convertSPolyPointToBlockCoord(glm::vec3 in_pointToConvert);

	private:
		int numberOfTilesPerDimension = 0;
		float dimensionLimit = 0.0f;
		float tileGridWidth;
		unsigned short convertBlockCoordsToUShort(int in_x, int in_y, int in_z);

};

#endif
