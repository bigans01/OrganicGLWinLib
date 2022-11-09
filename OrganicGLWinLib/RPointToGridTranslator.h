#pragma once

#ifndef RPOINTTOGRIDTRANSLATOR
#define RPOINTTOGRIDTRANSLATOR


class RPointToGridTranslator
{
	public:
		RPointToGridTranslator() {};
		void setTranslationParameters(int in_numberOfTilesPerDimension, float in_dimensionLimit)
		{
			numberOfTilesPerDimension = in_numberOfTilesPerDimension;
			dimensionLimit = in_dimensionLimit;
			dimPerTile = dimensionLimit / numberOfTilesPerDimension ;	// I.e, 4.0f / 40.
		};

		EnclaveKeyDef::EnclaveKey translatePointToGrid(glm::vec3 in_pointToTranslate)
		{
			EnclaveKeyDef::EnclaveKey returnKey;

			// X calcs
			if (in_pointToTranslate.x == 0.0f)
			{
				returnKey.x = 0;
			}
			else if (in_pointToTranslate.x == dimensionLimit)
			{
				returnKey.x = numberOfTilesPerDimension - 1;
			}
			else
			{
				returnKey.x = int((in_pointToTranslate.x / dimPerTile) - 1);
			}

			// Y calcs
			if (in_pointToTranslate.y == 0.0f)
			{
				returnKey.y = 0;
			}
			else if (in_pointToTranslate.y == dimensionLimit)
			{
				returnKey.y = numberOfTilesPerDimension - 1;
			}
			else
			{
				returnKey.y = int((in_pointToTranslate.y / dimPerTile) - 1);
			}

			// Z calcs
			if (in_pointToTranslate.z == 0.0f)
			{
				returnKey.z = 0;
			}
			else if (in_pointToTranslate.z == dimensionLimit)
			{
				returnKey.z = numberOfTilesPerDimension - 1;
			}
			else
			{
				returnKey.z = int((in_pointToTranslate.z / dimPerTile) - 1);
			}

			return returnKey;
		}

	private:
		friend class PTriangleMeshPointCalibrator;
		int numberOfTilesPerDimension = 0;
		float dimensionLimit = 0.0f;
		float dimPerTile = 0.0f;
};

#endif
