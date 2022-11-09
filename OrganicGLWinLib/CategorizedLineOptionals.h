#pragma once

#ifndef CATEGORIZEDLINEOPTIONALS_H
#define CATEGORIZEDLINEOPTIONALS_H

#include "IRPointType.h"
#include "FusedPointSubData.h"

class CategorizedLineOptionals
{
	public:
		void insertFusedPointSubDataForPoint(IRPointType in_irPointType, std::vector<FusedPointSubData> in_fusedPointSubDataVector)
		{
			subdataVectorMap[in_irPointType] = in_fusedPointSubDataVector;
		};
		std::map<IRPointType, std::vector<FusedPointSubData>> subdataVectorMap;
		std::vector<FusedPointSubData> getPointASubData()
		{
			return subdataVectorMap[IRPointType::POINT_A];
		}

};

#endif
