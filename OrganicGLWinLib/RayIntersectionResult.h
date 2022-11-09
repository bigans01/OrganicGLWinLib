#pragma once

#ifndef RAYINTERSECTIONRESULT_H
#define RAYINTERSECTIONRESULT_H


class RayIntersectionResult
{
	public:
		short wasIntersectFound = 0;	// was there an actual intersect?
										// values are: 0 -> no intersect, 1 -> intersect, 2 -> point was found within triangle, 3 -> both ray points equal a line in the triangle
													
		short wasIntersectOnBorderLine = 0;		// was this intersect caused by a border line?
		int borderLineID = 0;			// if the intersect was on a border line, what was the border line's ID?
		glm::vec3 intersectedPoint;		// what was the point that was intersected?

		void setResult(int in_result);
		int wasResultSet = 0;			// indicates if a result was already set

		bool liesWithinTrianglePlane = false; // indicates that the line was detected as being within the triangles plane (but not necessarily within the triangle.
		bool liesWithinTriangleZone = false; // can only be true if liesWithinTrianglePlane is true, and it lies within the "zone" of the triangle.
		bool matchesPointExact = false;			// indicates that point B of the ray matches a point exactly
};

#endif
