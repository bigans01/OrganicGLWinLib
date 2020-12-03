#pragma once
#ifndef COPLANARCATEGORIZEDLINEPRODUCER_H
#define COPLANARCATEGORIZEDLINEPRODUCER_H

#include "SPoly.h"
#include "LineSegment.h"
#include "LineSegmentIntersectAnalyzer.h"
#include "TwoDPoint.h"

class CoplanarCategorizedLineProducer
{
	public:
		CoplanarCategorizedLineProducer(SPoly* in_trackedSPolyRef, SPoly* in_relatedSPolyRef) :
			trackedSPolyRef(in_trackedSPolyRef),
			relatedSPolyRef(in_relatedSPolyRef)
		{
			performLineComparison();
		};
	private:
		SPoly* trackedSPolyRef = nullptr;
		SPoly* relatedSPolyRef = nullptr;
		TwoDPoint convertGlmVec3To2D(glm::vec3 in_glmvec3);
		void performLineComparison();

		
};

#endif
