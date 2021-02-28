#pragma once

#ifndef SPOLYHIERARCHYLOCATIONMAP_H
#define SPOLYHIERARCHYLOCATIONMAP_H

#include <map>
#include "SPolyHierarchyLocation.h"

class SPolyHierarchyLocationMap
{
	public:
		// the "int" represents the corresponding ID of the SPoly in the SPolySet that the SPoly is inserted into 
		// -- this allows us map the corresponding SPolySuperGroup in the SPolySet to the original parentpoly/subgroup within the parent it came from, before fracturing, since SPolys are only
		// known by their index in an SPolySet.
		//
		// the "SPolyHierarchyLocation" represents the parent and sub group that this SPoly was pulled from.
		// when the SPolySet is done, the contents its produces for an SPoly -- the contents of which are found in a corresponding SPolySupergroup -- will be compared against the corresponding EnclaveTriangleSkeletonContainer, 
		// for update.

		// An SPoly spawned as a "primal" -- such as in when it is produced in a terrain mesh -- always starts out as its own self, exactly one piece (SPoly). But may be further
		// broken down into more. The parentPolyID argument used in the funtions of this class represents the int that uniquely identifies this "primal" ID; the primal ID 
		// should always stay the same and unique, (immutable) for the duration it exists in the locationMap.

		// A primal poly's number of subgroups always starts at 0, as it always starts out as exactly 1 SPoly. Assuming it is unmodified, 
		// this will always result in exactly 1 SPolySuperGroup with exactly 1 STriangleContainer. When the "parent" or "primal" needs to take itself and divide into two or more pieces,
		// the number of resulting STriangleContainers will be equivalent to the number of pieces that exist as a result. For example, 1 SPolySuperGroup would contain 2 STriangleContainers,
		// if the result of the fracturing caused a single piece to be divided into two.

		// the locationMap maps the index of the SPoly as it exists in the SPolySet it is sent into, to the parentPolyID and subgroupId within said parentPolyID.
		// For instance, a "primal" SPoly will always have one child, of group 0 (group 0 = a EnclaveTriangleSkeleton). It's uniqueID is 7, and it contains the very first
		// SPoly that is about to go into an SPolySet, the values would be:
		//       
		//     in_parentPolyID = 7
		//     in_subgroupID = 0		(beceause there's only one piece, at subgroup 0)
		//     locationMap key value = 0 (because it's the very first SPoly going into the SPolySet.
		//
		//		Resulting map:
		//			locationMap[0] = location(7, 0)


		// In another case, pretend that parent polyID 7 has already been divided into a two pieces from a previous run through an SPolySet; it's two entries would look like this
		// (if we assume its pieces were the first two to be entered into the SPolySet):
		//
		//  Piece 1:
		//     in_parentPolyID = 7
		//     in_subgroupID = 0			(the first piece that belongs to poly ID 7)
		//     locationMap key value = 0    (it's the very first SPoly in the SPolySet)
		//
		//  Piece 2:
		//     in_parentPolyID = 7
		//     in_subgroupID = 1			(the second piece that belongs to poly ID 7)
		//     locationMap key value = 1	( the second piece in the SPolySet)
		//
		//		Resulting map:
		//			locationMap[0] = location(7, 0)	
		//          locationMap[1] = location(7, 1)
		



		// CASE 1: a SPoly with parentPolyID 0 gets passed into the SPolySet. The result is that the SPoly got divided into two SPolys, piece 0 and piece 1 -- both of which
		//         would be found as their own STriangleContainer container in an SPolySupergroup.
		//
		//         Piece 1 didn't exist, so it is appended; Piece 0 is then subsequently updated with the new values for that piece Id. This is done by checking the contents of
		//			the corresponding SPolySuperGroup that is mapped to the same locationMapKey Value, which can be found in the SPolySet after its run is complete.


		std::map<int, SPolyHierarchyLocation> locationMap;		
		void insertHierarchyLocation(int in_primalPolyID, int in_pieceID, int in_sPolySetIndex)
		{
			SPolyHierarchyLocation location(in_primalPolyID, in_pieceID, in_sPolySetIndex);
			locationMap[int(locationMap.size())] = location;
		}

		void printLocations()
		{
			auto locationsBegin = locationMap.begin();
			auto locationsEnd = locationMap.end();
			for (; locationsBegin != locationsEnd; locationsBegin++)
			{
				std::cout << "Primal poly ID: [" << locationsBegin->second.primalPolyID << "], piece ID: [" << locationsBegin->second.pieceID << "], sPolySetIndex: " << locationsBegin->second.indexInSPolySet << std::endl;
			}
		}
};

#endif
