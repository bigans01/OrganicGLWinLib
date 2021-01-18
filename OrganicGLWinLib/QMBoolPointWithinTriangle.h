#pragma once

#ifndef QMBOOLPOINTWITHINTRIANGLE_H
#define QMBOOLPOINTWITHINTRIANGLE_H

#include "QMBase.h"
#include "ThreeDLineSegment.h"

class QMBoolPointWithinTriangle : public QMBase
{
	public:
		bool solve(QuatRotationPoints* in_quatRotationPointsRef);
};

#endif
