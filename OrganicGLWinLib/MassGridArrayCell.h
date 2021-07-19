#pragma once

#ifndef MASSGRIDARRAYCELL_H
#define MASSGRIDARRAYCELL_H

#include "MassCellBitFlags.h"
#include <glm/glm.hpp>
#include <vector>

class MassGridArrayCell
{
	public:
		unsigned char cellData = 0;
		void setFlag(MassCellBitFlags in_flagToSet, int in_bitValue)
		{
			int bitToSet = 0;
			switch (in_flagToSet)
			{
				case(MassCellBitFlags::LINE_MASS):  { bitToSet = 8; break; }
				case(MassCellBitFlags::CRUST_MASS): { bitToSet = 7; break; }
				case(MassCellBitFlags::INNER_MASS): { bitToSet = 6; break; }
				case(MassCellBitFlags::DOWNFILL_CRUST): { bitToSet = 5; break; }
				case(MassCellBitFlags::UPFILL_CRUST): {bitToSet = 4; break; }
			}

			int exponent = bitToSet - 1;
			int bitValue = int(pow(2, exponent));
			if (in_bitValue == 1)
			{
				cellData |= (1 << exponent);
			}
			else if (in_bitValue == 0)
			{
				cellData &= ~(1 << exponent);
			}
		}

		bool isFlagSet(MassCellBitFlags in_flagToSet)
		{
			bool wasSet = false;
			int bitToSet = 0;
			switch (in_flagToSet)
			{
				case(MassCellBitFlags::LINE_MASS): { bitToSet = 8; break; }
				case(MassCellBitFlags::CRUST_MASS): { bitToSet = 7; break; }
				case(MassCellBitFlags::INNER_MASS): { bitToSet = 6; break; }
				case(MassCellBitFlags::DOWNFILL_CRUST): { bitToSet = 5; break; }
				case(MassCellBitFlags::UPFILL_CRUST): {bitToSet = 4; break; }
			}
			int bitShiftcount = bitToSet - 1;
			int resultValue = (cellData >> bitShiftcount) & 1;
			if (resultValue == 1)
			{
				//std::cout << "Bit " << bitToSet << " was set!" << std::endl;
				wasSet = true;
			}

			//else if (resultValue == 0)
			//{
				//std::cout << "Bit " << bitToSet << " was NOT set!" << std::endl;
			//}
			return wasSet;
		}

		bool isDownfillRunnable()
		{
			bool isRunnable = false;

			// it's only runnable, ONLY if the DOWNFILL_CRUST is set.
			if
			(
				(isFlagSet(MassCellBitFlags::DOWNFILL_CRUST) == true)
				&&
				(isFlagSet(MassCellBitFlags::UPFILL_CRUST) == false)
			)
			{
				isRunnable = true;
			}
			return isRunnable;
		}

		void insertEmptyNormal(glm::vec3 in_emptyNormal)
		{
			emptyNormalVector.push_back(in_emptyNormal);
		}
	private:
		std::vector<glm::vec3> emptyNormalVector;
};

#endif
