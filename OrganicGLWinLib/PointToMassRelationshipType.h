#pragma once

#ifndef POINTTOMASSRELATIONSHIPTYPE_H
#define POINTTOMASSRELATIONSHIPTYPE_H

enum class PointToMassRelationshipType
{
	WITHIN_MASS,
	COPLANAR_TO_SHELL_SPOLY,
	OUTSIDE_OF_MASS,			// would be grounds for immediate disqualification, if it is set
	NOVAL
};

#endif

