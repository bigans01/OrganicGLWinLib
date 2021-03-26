#pragma once

#ifndef POINTTOMASSRELATIONSHIPTYPE_H
#define POINTTOMASSRELATIONSHIPTYPE_H

enum class PointToMassRelationshipType
{
	WITHIN_MASS,
	NO_LINE_OF_SIGHT,
	COPLANAR_TO_SHELL_SPOLY,
	COPLANAR_TO_STRIANGLE,
	OUTSIDE_OF_MASS,			// would be grounds for immediate disqualification, if it is set
	NOVAL
};

#endif

