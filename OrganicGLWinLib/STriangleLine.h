#pragma once

#ifndef STRIANGLELINE_H
#define STRIANGLELINE_H

#include "FTrianglePoint.h"

/*
* 
* Description: an STriangleLine is part of an STriangle, and essentially forms it's lines.
* This class received an encapsulation pass on 6/20/2025, as part of a larger effort to rework the STriangle class
* to be utilize FTrianglePoint objects, instead of the basic glm::vec3.
* 
* After this pass, the points of the STriangle are no longer publically available, allowing the STriangleLine class to manage
* what it returns with these calls, on it's own terms. This should also allow for less time being spent on clearing out errors
* that were utilizing direct access to the members of this class.
*
* This class will need to undergo yet another refactoring pass, once the members pointA/pointB switch to being a FTrianglePoint
* instead of glm::vec3.

*/


class STriangleLine
{
	public:
		unsigned char isBorderLine = 0;		// indicates whether or not it is a border line of the STriangleSet this triangle belongs in
		unsigned char borderLineID = 0;		// indicates the borderLineID, if it is indeed a border line
		unsigned char pointAOnBorderLine = 0;	// is point A on border line? (checked when borderLineID is 1)
		unsigned char pointBOnBorderLine = 0;	// is point B on border line? " " " 

		// dimension retrieval functions; primarily used by debug output logging utilities, but also 
		// used by various comparison functions/code.
		float getPointAx() { return pointA.x; };
		float getPointAy() { return pointA.y; };
		float getPointAz() { return pointA.z; };

		float getPointBx() { return pointB.x; };
		float getPointBy() { return pointB.y; };
		float getPointBz() { return pointB.z; };

		// point setting functions; these make it easier to find what code is actually instantiating 
		// instances of STriangleLine
		void setPointA(float in_x, float in_y, float in_z)
		{
			pointA.x = in_x;
			pointA.y = in_y;
			pointA.z = in_z;
		}

		void setPointB(float in_x, float in_y, float in_z)
		{
			pointB.x = in_x;
			pointB.y = in_y;
			pointB.z = in_z;
		}

		// Retrieval functions; allows for various forms of x/y/z to be returned, to be 
		// compatible with other code. Currently allows for the return of glm::vec3 and FTrianglePoint versions.
		//
		// The glmvec3 version retrieval functions are used extensively by many classes; the FTrianglePoint
		// version is used by SPolyUtils::produceFansFromSupergroup for future usage.
		glm::vec3 fetchPointAGlmVec3Version() { return pointA; }
		glm::vec3 fetchPointBGlmVec3Version() { return pointB; }
		FTrianglePoint fetchPointAFTrianglePoint() { return FTrianglePoint(pointA); }
		FTrianglePoint fetchPointBFTrianglePoint() { return FTrianglePoint(pointB); }

		// These load a hundredth-rounded version of either A or B from another line,
		// into this line.
		//
		// Currently only utilized by SPolySupergroup::roundAllSTrianglesToHundredths().
		void loadHundredthRoundedPointAFromOtherLine(STriangleLine* in_sTriangleLine)
		{
			float pointAx = float(floor(in_sTriangleLine->getPointAx() * 100 + 0.5) / 100);
			float pointAy = float(floor(in_sTriangleLine->getPointAy() * 100 + 0.5) / 100);
			float pointAz = float(floor(in_sTriangleLine->getPointAz() * 100 + 0.5) / 100);

			setPointA(pointAx, pointAy, pointAz);
		}

		void loadHundredthRoundedPointBFromOtherLine(STriangleLine* in_sTriangleLine)
		{
			float pointBx = float(floor(in_sTriangleLine->getPointBx() * 100 + 0.5) / 100);
			float pointBy = float(floor(in_sTriangleLine->getPointBy() * 100 + 0.5) / 100);
			float pointBz = float(floor(in_sTriangleLine->getPointBz() * 100 + 0.5) / 100);

			setPointB(pointBx, pointBy, pointBz);
		}

		// Sets the swapped version of the points of another line as the values in this line.
		// Currently only used by FusionCandidateProducer::produceCandidate.
		void setPointsAsSwappedFromOtherLine(STriangleLine* in_lineRef)
		{
			pointA = in_lineRef->pointB;
			pointB = in_lineRef->pointA;
		}

		// Point equivalence testing functions; this encapsulation feature allows for checking of point values
		// without modifying the actual points themselves.
		bool doesPointAEqualValue(glm::vec3 in_vec3ToCompare) { return compareVec3OfAtoVec3(in_vec3ToCompare); }
		bool doesPointBEqualValue(glm::vec3 in_vec3ToCompare) { return compareVec3OfBtoVec3(in_vec3ToCompare); }

		// Reference retrieval functions, should only be used in a very limited manner;
		// Currently, as of 6/21/2025, this is being used by the following classes/functions:
		// 
		// -SPoly (for QuatRotationManager usage, multiple functions)
		// -CategorizedLineColinearTester::runColinearTests
		//
		// These will need to be updated when transitioning to using the FTrianglePoint class as the point storage objects for A/B (this is in progess/dev)
		glm::vec3* getPointARef() { return &pointA; }
		glm::vec3* getPointBRef() { return &pointB; }

	private:
		glm::vec3 pointA;	// the beginning of the line
		glm::vec3 pointB;	// the end of the line

		bool compareVec3OfAtoVec3(glm::vec3 in_vec3ToCompare) { return (glm::vec3(getPointAx(), getPointAy(), getPointAz()) == in_vec3ToCompare); }
		bool compareVec3OfBtoVec3(glm::vec3 in_vec3ToCompare) { return (glm::vec3(getPointBx(), getPointBy(), getPointBz()) == in_vec3ToCompare); }


};

#endif
