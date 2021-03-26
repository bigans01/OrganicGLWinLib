#include "stdafx.h"
#include "MassZoneShellSlice.h"

void MassZoneShellSlice::runAnalysis()
{
	// first, check the relationship. 
	PointToMassRelationshipType analyzedType = QuatUtils::findPointToMassRelationship(relationshipPointToCompare,
																						shellSliceBaseTriangle->triangleLines[0].pointA,
																						shellSliceBaseTriangle->triangleLines[1].pointA,
																						shellSliceBaseTriangle->triangleLines[2].pointA,
																						shellSliceBaseEmptyNormal,
																						PolyDebugLevel::NONE);
	// if it's exactly in the STriangle's area, we're good.
	if (analyzedType == PointToMassRelationshipType::COPLANAR_TO_STRIANGLE)
	{

	}
	// if it's WITHIN_MASS (as in, behind the empty normal)
	else if (analyzedType == PointToMassRelationshipType::WITHIN_MASS)
	{

	}
	// if it's OUTSIDE_OF_MASS (as in, on the same side of the triangle that the empty normal is facing)
	else if (analyzedType == PointToMassRelationshipType::OUTSIDE_OF_MASS)
	{

	}
}