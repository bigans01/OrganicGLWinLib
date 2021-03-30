#include "stdafx.h"
#include "MassZoneShellSlice.h"

void MassZoneShellSlice::setShellSliceDebugLevel(PolyDebugLevel in_polyDebugLevel)
{
	shellSlicePolyLogger.setDebugLevel(in_polyDebugLevel);
	shellSlicePolyLoggerDebugLevel = in_polyDebugLevel;
}

void MassZoneShellSlice::runAnalysis()
{
	//std::cout << "!!! ||||||||||||| #### Running analysis..." << std::endl;
	//std::cout << "!!! ||||||||||||| Point to compare -> " << relationshipPointToCompare.x << ", " << relationshipPointToCompare.y << ", " << relationshipPointToCompare.z << std::endl;
	//std::cout << "!!! ||||||||||||| Base Triangle point A: " << shellSliceBaseTriangle->triangleLines[0].pointA.x << ", " << shellSliceBaseTriangle->triangleLines[0].pointA.y << ", " << shellSliceBaseTriangle->triangleLines[0].pointA.z << std::endl;
	//std::cout << "!!! ||||||||||||| Base Triangle point B: " << shellSliceBaseTriangle->triangleLines[1].pointA.x << ", " << shellSliceBaseTriangle->triangleLines[1].pointA.y << ", " << shellSliceBaseTriangle->triangleLines[1].pointA.z << std::endl;
	//std::cout << "!!! ||||||||||||| Base Triangle point C: " << shellSliceBaseTriangle->triangleLines[2].pointA.x << ", " << shellSliceBaseTriangle->triangleLines[2].pointA.y << ", " << shellSliceBaseTriangle->triangleLines[2].pointA.z << std::endl;
	//std::cout << "!!! ||||||||||||| Shell slice empty normal: " << shellSliceBaseEmptyNormal.x << ", " << shellSliceBaseEmptyNormal.y << ", " << shellSliceBaseEmptyNormal.z << std::endl;

	shellSlicePolyLogger.log("(MassZoneShellSlice): |||| |||||||||----->> BEGIN Running analysis for a shell slice based off of SPoly ", baseSPolyID, ", STriangle ", baseSTriangleID, "\n");
	shellSlicePolyLogger.log("(MassZoneShellSlice): |||| ----->> Point to compare against the shell is: ", relationshipPointToCompare.x, ", ", relationshipPointToCompare.y, ", ", relationshipPointToCompare.z, "\n");
	shellSlicePolyLogger.log("(MassZoneShellSlice): |||| ----->> Shell slice base triangle points are:", "\n");
	shellSlicePolyLogger.log("(MassZoneShellSlice): point 0: ", shellSliceBaseTriangle->triangleLines[0].pointA.x, ", ", shellSliceBaseTriangle->triangleLines[0].pointA.y, ", ", shellSliceBaseTriangle->triangleLines[0].pointA.z, "\n");
	shellSlicePolyLogger.log("(MassZoneShellSlice): point 1: ", shellSliceBaseTriangle->triangleLines[1].pointA.x, ", ", shellSliceBaseTriangle->triangleLines[1].pointA.y, ", ", shellSliceBaseTriangle->triangleLines[1].pointA.z, "\n");
	shellSlicePolyLogger.log("(MassZoneShellSlice): point 2: ", shellSliceBaseTriangle->triangleLines[2].pointA.x, ", ", shellSliceBaseTriangle->triangleLines[2].pointA.y, ", ", shellSliceBaseTriangle->triangleLines[2].pointA.z, "\n");
	shellSlicePolyLogger.log("(MassZoneShellSlice): |||| ----->> Shell slice empty normal: ", shellSliceBaseEmptyNormal.x, ", ", shellSliceBaseEmptyNormal.y, ", ", shellSliceBaseEmptyNormal.z, "\n");

	// first, check the relationship. 
	PointToMassRelationshipType analyzedType = QuatUtils::findPointToMassRelationship(relationshipPointToCompare,
																						shellSliceBaseTriangle->triangleLines[0].pointA,
																						shellSliceBaseTriangle->triangleLines[1].pointA,
																						shellSliceBaseTriangle->triangleLines[2].pointA,
																						shellSliceBaseEmptyNormal,
																						PolyDebugLevel::NONE);
	// if it's exactly in the STriangle's area, we're good.
	if (analyzedType == PointToMassRelationshipType::COPLANAR_TO_STRIANGLE)
	{
		//std::cout << "---> Point found as being COPLANAR_TO_STRIANGLE. " << std::endl;
		shellSlicePolyLogger.log("(MassZoneShellSlice): |||| Notice, relationshipPointToCompare found as being COPLANAR_TO_STRIANGLE (compared against slice base triangle)", "\n");
		analysisResult = PointToMassRelationshipType::COPLANAR_TO_STRIANGLE;
	}
	// if it's WITHIN_MASS (as in, behind the empty normal), 
	// then we must do a raycast against all of the SPolys in the shellSliceClippingShellMap.
	// the ray begins at the point to compare to, and is cast in the direction of the empty normal.
	// Line of sight is considered maintained if the distance between the compared point and the shellSliceBaseTriangle is the shortest among all interceptions produced as a result of raycasting.
	else if (analyzedType == PointToMassRelationshipType::WITHIN_MASS)
	{
		checkLineOfSightForWithinMassRelationship();	// checks to see if the WITHIN_MASS relationship maintains it's line of sight.
	}
	// if it's OUTSIDE_OF_MASS (as in, on the same side of the triangle that the empty normal is facing) 
	// then we must do a raycast against all of the SPolys in the shellSliceClippingShellMap.
	// the ray begins at the point to compare to, and is cast in the OPPOSITE direction of the empty normal.
	// Line of sight is considered maintained if the distance between the compared point and the shellSliceBaseTriangle is the shortest among all interceptions produced as a result of raycasting.
	else if (analyzedType == PointToMassRelationshipType::OUTSIDE_OF_MASS)
	{
		glm::vec3 rayDirection = (shellSliceBaseEmptyNormal * -1.0f);
		Ray baseRay(relationshipPointToCompare, rayDirection);
		std::cout << "---> Entered OUTSIDE_OF_MASS branch. " << std::endl;
		std::cout << "---> Running ray cast test of point: " << relationshipPointToCompare.x << ", " << relationshipPointToCompare.y << ", " << relationshipPointToCompare.z << std::endl;
		std::cout << "---> STriangle points are: " << std::endl;
		shellSliceBaseTriangle->printPoints();
		RaycastIntersectionTest primeTest(baseRay, *shellSliceBaseTriangle);
		RayIntersectionResult result = primeTest.producedResult;
		if (result.wasIntersectFound != 0)
		{
			// we should always find an intersection with the base triangle.
			std::cout << "---> Intersection was found!! " << std::endl;
			std::cout << "---> Intersection point was: " << result.intersectedPoint.x << ", " << result.intersectedPoint.y << ", " << result.intersectedPoint.z << std::endl;

			std::vector<float>	distances;
			float primeDistance = glm::distance(relationshipPointToCompare, result.intersectedPoint);
		}
	}
	shellSlicePolyLogger.log("(MassZoneShellSlice): |||| |||||||||----->> FINISHED Running analysis for a shell slice based off of SPoly ", baseSPolyID, ", STriangle ", baseSTriangleID, "\n");
}

void MassZoneShellSlice::checkLineOfSightForWithinMassRelationship()
{
	glm::vec3 rayDirection = shellSliceBaseEmptyNormal;
	Ray baseRay(relationshipPointToCompare, rayDirection);

	//std::cout << "---> Entered WITHIN_MASS branch. " << std::endl;
	//std::cout << "---> Ray direction is: " << rayDirection.x << ", " << rayDirection.y << ", " << rayDirection.z << std::endl;
	//std::cout << "---> Running ray cast test of point: " << relationshipPointToCompare.x << ", " << relationshipPointToCompare.y << ", " << relationshipPointToCompare.z << std::endl;
	//std::cout << "---> STriangle points are: " << std::endl;
	//shellSliceBaseTriangle->printPoints();

	shellSlicePolyLogger.log("(MassZoneShellSlice): checking line of sight for WITHIN_MASS relationship.", "\n");
	shellSlicePolyLogger.log("(MassZoneShellSlice): Ray direction is: ", rayDirection.x, ", ", rayDirection.y, ", ", rayDirection.z, "\n");
	shellSlicePolyLogger.log("(MassZoneShellSlice): Running initial ray cast towards the MassZoneShellSlice base triangle (shellSliceBaseTriangle), with ray origin at: ", relationshipPointToCompare.x, ", ", relationshipPointToCompare.y, ", ", relationshipPointToCompare.z, "\n");
	shellSlicePolyLogger.log("(MassZoneShellSlice): shellSliceBaseTriangle points are:", "\n");
	if (shellSlicePolyLogger.isLoggingSet())
	{
		shellSliceBaseTriangle->printPoints();
	}

	RaycastIntersectionTest primeTest(baseRay, *shellSliceBaseTriangle);
	RayIntersectionResult result = primeTest.producedResult;
	if (result.wasIntersectFound != 0)
	{
		// we should always find an intersection with the base triangle.
		//std::cout << "---> Intersection was found!! " << std::endl;
		//std::cout << "---> Intersection point was: " << result.intersectedPoint.x << ", " << result.intersectedPoint.y << ", " << result.intersectedPoint.z << std::endl;
		shellSlicePolyLogger.log("(MassZoneShellSlice): Ray cast successfully intersected with MassZoneShellSlice base triangle (shellSliceBaseTriangle), at the point ", result.intersectedPoint.x, ", ", result.intersectedPoint.y, ", ", result.intersectedPoint.z, "\n");
		shellSlicePolyLogger.log("(MassZoneShellSlice): Now beginning RayCastIntersectionTests against all other SPolys in the shellClippingMap.", "\n");

		// push back the initial distance value; this has to be checked later
		std::vector<float>	distances;
		float primeDistance = glm::distance(relationshipPointToCompare, result.intersectedPoint);
		distances.push_back(primeDistance);

		// now do RaycastIntersectionTests for all the other SPolys, and push them into the vector.
		auto shellClippingMapBegin = shellSliceClippingShellMap.begin();
		auto shellClippingMapEnd = shellSliceClippingShellMap.end();
		for (; shellClippingMapBegin != shellClippingMapEnd; shellClippingMapBegin++)
		{
			auto currentSTriangleBegin = shellClippingMapBegin->second->triangles.begin();
			auto currentSTriangleEnd = shellClippingMapBegin->second->triangles.end();
			for (; currentSTriangleBegin != currentSTriangleEnd; currentSTriangleBegin++)
			{
				// run a test against the current STriangle.
				RaycastIntersectionTest currentTest(baseRay, currentSTriangleBegin->second);
				RayIntersectionResult currentResult = currentTest.producedResult;
				if
				(
					(currentResult.wasIntersectFound != 0)
				)
				{
					//std::cout << "|| intersectFound; code was: " << currentResult.wasIntersectFound << std::endl;
					//std::cout << "!! Ray points: " << baseRay.raySegmentBeginPoint.x << ", " << baseRay.raySegmentBeginPoint.y << ", " << baseRay.raySegmentBeginPoint.z << " | "
						//<< baseRay.raySegmentEndPoint.x << ", " << baseRay.raySegmentEndPoint.y << ", " << baseRay.raySegmentEndPoint.z << std::endl;
					//std::cout << "!! Ray was casted against STriangle: " << std::endl;
					//currentSTriangleBegin->second.printPoints();

					shellSlicePolyLogger.log("(MassZoneShellSlice): Ray cast intersected with STriangle having ID [", currentSTriangleBegin->first, "], in shell SPoly having ID [", shellClippingMapBegin->first, "\n");
					if (shellSlicePolyLogger.isLoggingSet())
					{
						shellSlicePolyLogger.log("(MassZoneShellSlice): Printing points of last intersected triangle: ", "\n");
						currentSTriangleBegin->second.printPoints();
					}

					float intersectedDistance = glm::distance(relationshipPointToCompare, currentResult.intersectedPoint);
					distances.push_back(intersectedDistance);
				}
			}
		}

		// find whatever point in the vector had the shortest distance; this is the one that has line of sight. If the one
		// that maintains line of sight isn't the first in the vector (which is the shell slice's base), there is NO_LINE_OF_SIGHT between it and the point.
		float currentShortestDistance = 10000.f;
		int currentShortestIndex = -1;
		int currentIndex = 0;
		auto distancesBegin = distances.begin();
		auto distancesEnd = distances.end();
		for (; distancesBegin != distancesEnd; distancesBegin++)
		{
			//std::cout << "Distance value at index: " << currentIndex << ": " << *distancesBegin << std::endl;
			shellSlicePolyLogger.log("(MassZoneShellSlice): Distance value at index: ", currentIndex, ": ", *distancesBegin, "\n");
			if (*distancesBegin < currentShortestDistance)
			{
				currentShortestIndex = currentIndex;
				currentShortestDistance = *distancesBegin;
				//std::cout << "!!! Notice: currentShortestIndex is now: " << currentIndex << std::endl;
				shellSlicePolyLogger.log("(MassZoneShellSlice): !!! Notice: currentShortestIndex is now: ", currentIndex, "\n");
			}
			currentIndex++;
		}

		// check if the first one (at index 0 in the vector) is the shortest distance, by checking the value of currentShortestIndex. If anything other than 0, 
		// then there is no line of sight.
		if (currentShortestIndex != 0)
		{
			//std::cout << "!!! Notice: no line of sight between the MassZoneShellSlice' base STriangle and the compared-to point!" << std::endl;
			shellSlicePolyLogger.log("(MassZoneShellSlice): !!! Notice: no line of sight between the MassZoneShellSlice' base STriangle and the compared-to point!", "\n");
			analysisResult = PointToMassRelationshipType::NO_LINE_OF_SIGHT;
		}

		// otherwise, line of sight is maintained. 
		else if (currentShortestIndex == 0)
		{
			analysisResult = PointToMassRelationshipType::WITHIN_MASS;
		}
	}
	else
	{
		std::cout << "########### !!!!! WARNING, didn't enter appropriate branch. " << std::endl;
	}
}

PointToMassRelationshipType MassZoneShellSlice::getAnalysisResult()
{
	return analysisResult;
}