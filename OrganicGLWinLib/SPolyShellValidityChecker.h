#pragma once

#ifndef SPOLYSHELLVALIDITYCHECKER_H
#define SPOLYSHELLVALIDITYCHECKER_H

#include "SPoly.h"
#include "HashedSTriangleLineCounter.h"

/*

Description: The intent of this class is to determine if all the lines inserted into the counterTracker map 
of this class each have a count of 2, within each instance of HashedSTriangleLineCounter found within
said map. If this is true, it would signal that the lines that make up the mesh are part of a complete mesh 
(i.e, each line is involved with exactly 2 triangles -- no more, and no less).

*/

class SPolyShellValidityChecker
{
	public:
		SPolyShellValidityChecker() {};		
		void insertLine(HashedSTriangleLine in_lineToInsert);	// insert a line into the counterTracker
		bool fetchValidity();	// run analysis on counterTracker to determine if the lines are invovled with a valid mesh.
		void printLineStats();	// print the count of each line in the counterTracker, as well as the stats of each one.
	private:
		std::map<int, HashedSTriangleLineCounter> counterTracker;	// map where all tracking of lines is done.
		std::map<int, int> dumbMap;
		bool calculateValidity();	// determine whether or not all HashedSTriangleLineCounter instances in counterTracker have a value of 2.
									// If they do, the mesh is complete.

};

#endif
