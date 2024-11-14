#pragma once

#ifndef HASHEDSTRIANGLELINECOUNTER_H
#define HASHEDSTRIANGLELINECOUNTER_H

#include "HashedSTriangleLine.h"

/*

Description: This class is wrapped around and based off an instance of HashedSTriangleLine. It is designed to keep a counter
that increments whenever it matches with another line, via the checkIflinesMatch function. 

Ideally, in a mesh that is truly complete without anything missing, the matchCount value will be 2 (because an individual line 
can only be associated with two triangles).

*/

class HashedSTriangleLineCounter
{
	public:
		HashedSTriangleLineCounter();	// default constructor, for copying operations into a map, etc.
		HashedSTriangleLineCounter(HashedSTriangleLine in_line);	// the constructor that should be called when tis object is built
		bool checkIfLinesMatch(HashedSTriangleLine* in_line);	// check if the referenced in_line matches against the lineToCount member of this class.
		int getMatchCount();		// return the value of matchCount.
		void printCounterStats();	// print points of the underlying HashedSTriangleLine
	private:
		HashedSTriangleLine lineToCount;
		int matchCount = 1;	// when first initialized, vaue should always be 1.

};

#endif
