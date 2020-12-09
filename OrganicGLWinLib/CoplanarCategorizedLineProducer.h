#pragma once
#ifndef COPLANARCATEGORIZEDLINEPRODUCER_H
#define COPLANARCATEGORIZEDLINEPRODUCER_H

#include "SPoly.h"
#include "TwoDLineSegment.h"
#include "TwoDLineSegmentIntersectAnalyzer.h"
#include "TwoDPoint.h"
#include "OrganicGLWinUtils.h"
#include "CategorizedLinePool.h"

class CoplanarCategorizedLineProducer
{
	public:
		CoplanarCategorizedLineProducer(SPoly* in_trackedSPolyRef, SPoly* in_relatedSPolyRef, CategorizedLinePool* in_categorizedLinePoolRef) :
			trackedSPolyRef(in_trackedSPolyRef),
			relatedSPolyRef(in_relatedSPolyRef),
			categorizedLinePoolRef(in_categorizedLinePoolRef)
		{
			performLineComparison();
		};
	private:
		SPoly* trackedSPolyRef = nullptr;
		SPoly* relatedSPolyRef = nullptr;
		CategorizedLinePool* categorizedLinePoolRef = nullptr;
		TwoDPoint convertGlmVec3To2D(glm::vec3 in_glmvec3);
		void performLineComparison();

		
};

#endif
