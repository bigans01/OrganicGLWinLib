#pragma once

#ifndef FUSIONANALYSISPRIMARYCLASSIFICATION_H
#define FUSIONANALYSISPRIMARYCLASSIFICATION_H

enum class FusionAnalysisPrimaryClassification
{
	// the number of processed lines in the group; i.e., the number of lines which intersected the other triangle in any way.
	TRIPLE,
	DOUBLE,
	SINGLE,
	NONE
};

#endif
