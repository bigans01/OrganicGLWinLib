#pragma once

#ifndef SPOLYHIERARCHYLOCATION_H
#define SPOLYHIERARCHYLOCATION_H

class SPolyHierarchyLocation
{
	public:
		int primalPolyID = 0;
		int pieceID = 0;
		int indexInSPolySet = 0;
		SPolyHierarchyLocation() {};
		SPolyHierarchyLocation(int in_primalPolyID, int in_pieceID, int in_indexInSPolySet) :
			primalPolyID(in_primalPolyID),
			pieceID(in_pieceID),
			indexInSPolySet(in_indexInSPolySet)
		{};
};

#endif
