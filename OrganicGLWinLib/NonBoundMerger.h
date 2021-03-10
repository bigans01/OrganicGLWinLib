#pragma once

#ifndef NONBOUNDMERGER_H
#define NONBOUNDMERGER_H

#include "CategorizedLineMergeMachineBase.h"

class CleaveSequenceFactory;
class NonBoundMerger : public CategorizedLineMergeMachineBase
{
	public:
		void runMerging();
	private:
		struct PointCounter
		{
			PointCounter() {};
			PointCounter(glm::vec3 in_point)
			{
				point = in_point;
			}

			glm::vec3 point;
			int pointCount = 0;
			void incrementPointCount()
			{
				pointCount++;
			};
		};
		std::map<int, CategorizedLine> orderedNonboundMap;	// will store all of the non-bound lines in the proper order.
		std::map<int, PointCounter> pointCounterMap;

		void insertPointIntoCounterMap(glm::vec3 in_point);
		glm::vec3 findFirstPointWithSingularCount();
		void insertLineIntoOrderedNonBounds(CategorizedLine in_categorizedLine);
};

#endif
