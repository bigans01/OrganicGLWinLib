#pragma once
#ifndef COPLANARCATEGORIZEDLINEPRODUCER_H
#define COPLANARCATEGORIZEDLINEPRODUCER_H

#include "SPoly.h"
#include "LineSegment.h"
#include "LineSegmentIntersectAnalyzer.h"

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
		void performLineComparison();

		
};

#endif
