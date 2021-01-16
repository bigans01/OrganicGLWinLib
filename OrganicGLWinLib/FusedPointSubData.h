#pragma once

#ifndef FUSEDPOINTSUBDATA_H
#define FUSEDPOINTSUBDATA_H

class FusedPointSubData
{
	public:
		FusedPointSubData() {};
		FusedPointSubData(int in_triangleLineIndex, int in_isBorderLine, int in_borderLineValue) :
			triangleLineIndex(in_triangleLineIndex),
			isBorderLine(in_isBorderLine),
			borderLineValue(in_borderLineValue)
		{};
		int triangleLineIndex = 0;
		int isBorderLine = 0;
		int borderLineValue = 0;
};

#endif
