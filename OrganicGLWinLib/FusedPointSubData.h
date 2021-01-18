#pragma once

#ifndef FUSEDPOINTSUBDATA_H
#define FUSEDPOINTSUBDATA_H

#include "FusionCandidateOrigin.h"

class FusedPointSubData
{
	public:
		FusedPointSubData() {};
		FusedPointSubData(FusionCandidateOrigin in_fusionCandidateOrigin, int in_triangleLineIndex, int in_isBorderLine, int in_borderLineValue) :
			origin(in_fusionCandidateOrigin),
			triangleLineIndex(in_triangleLineIndex),
			isBorderLine(in_isBorderLine),
			borderLineValue(in_borderLineValue)
		{};
		FusionCandidateOrigin origin = FusionCandidateOrigin::NONE;
		int triangleLineIndex = 0;
		int isBorderLine = 0;
		int borderLineValue = 0;
};

#endif
