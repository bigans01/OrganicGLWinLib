#pragma once

#ifndef RMORPHABLEMESHCORNERARRAY_H
#define RMORPHABLEMESHCORNERARRAY_H

#include "RCollisionPoint.h"

class RMorphableMeshCornerArray
{
	public:
		RMorphableMeshCornerArray() {};
		RMorphableMeshCornerArray(RCollisionPoint* in_point0Ref,
			RCollisionPoint* in_point1Ref,
			RCollisionPoint* in_point2Ref,
			RCollisionPoint* in_point3Ref,
			RCollisionPoint* in_point4Ref,
			RCollisionPoint* in_point5Ref,
			RCollisionPoint* in_point6Ref,
			RCollisionPoint* in_point7Ref
		)
		{
			pointRefArray[0] = in_point0Ref;
			pointRefArray[1] = in_point1Ref;
			pointRefArray[2] = in_point2Ref;
			pointRefArray[3] = in_point3Ref;
			pointRefArray[4] = in_point4Ref;
			pointRefArray[5] = in_point5Ref;
			pointRefArray[6] = in_point6Ref;
			pointRefArray[7] = in_point7Ref;
		}

		RCollisionPoint* pointRefArray[8] = { nullptr };

};

#endif
