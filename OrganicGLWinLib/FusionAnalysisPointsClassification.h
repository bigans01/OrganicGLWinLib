#pragma once

#ifndef FUSIONANALYSISPOINTSCLASSIFICATION_H
#define FUSIONANALYSISPOINTSCLASSIFICATION_H

enum class FusionAnalysisPointsClassification
{
	TRIPLE_NO_COPLANARS,
	TRIPLE_COPLANAR_BORDERLINE,		// would be immediately invalid if this was found
	TRIPLE_COPLANAR_NONBORDERLINE,

	DOUBLE_NO_COPLANARS,
	DOUBLE_COPLANAR_BORDERLINE,		// would be immediately invalid if this was found
	DOUBLE_COPLANAR_NONBORDERLINE,

	SINGLE,

	NONE
};

#endif
