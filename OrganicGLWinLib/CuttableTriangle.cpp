#include "stdafx.h"
#include "CuttableTriangle.h"

CuttableTriangle::CuttableTriangle(STriangle in_cuttableTriangle)
{
	// find the centroid
	glm::vec3 pointA = cuttableTriangleLines[0].pointA;
	glm::vec3 pointB = cuttableTriangleLines[1].pointA;
	glm::vec3 pointC = cuttableTriangleLines[2].pointA;
	glm::vec3 centroid = OrganicGLWinUtils::findTriangleCentroid(pointA, pointB, pointC);

	// find the centroid facing normal for each line
	for (int x = 0; x < 3; x++)
	{
		QuatRotationPoints points;	// point 0 = pointA of line
									// point 1 = pointB of line
									// point 2 = centroid
		glm::vec3 pointACopy = in_cuttableTriangle.triangleLines[x].pointA;
		glm::vec3 pointBCopy = in_cuttableTriangle.triangleLines[x].pointB;
		glm::vec3 centroidCopy = centroid;
		points.pointsRefVector.push_back(&pointACopy);
		points.pointsRefVector.push_back(&pointBCopy);
		points.pointsRefVector.push_back(&centroidCopy);

		QMVec3FindCentroidFacingNormal normalFinder;
		glm::vec3 centroidFacingNormal = normalFinder.solve(&points, PolyDebugLevel::NONE);
		CuttableTriangleLine newLine(in_cuttableTriangle.triangleLines[x].pointA, in_cuttableTriangle.triangleLines[x].pointB, centroidFacingNormal);
		cuttableTriangleLines[x] = newLine;
	}
}

void CuttableTriangle::compareAgainstCuttingTriangle(CuttingTriangle* in_cuttingTriangleRef)
{
	// first, run each CuttableTriangleLine against each CuttingTriangleLine.
	for (int currentCuttableTriangleLineID = 0; currentCuttableTriangleLineID < 3; currentCuttableTriangleLineID++)
	{
		for (int currentCuttingTriangleLineID = 0; currentCuttingTriangleLineID < 3; currentCuttingTriangleLineID++)
		{
			compareCuttableTriangleLineToCuttingTriangleLine(currentCuttableTriangleLineID, 
															&cuttableTriangleLines[currentCuttableTriangleLineID], 
															currentCuttingTriangleLineID, 
															&in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID]);
			TwoDLineSegment cuttableSegment(cuttableTriangleLines[currentCuttableTriangleLineID].pointA.x,
											cuttableTriangleLines[currentCuttableTriangleLineID].pointA.y,
											cuttableTriangleLines[currentCuttableTriangleLineID].pointB.x,
											cuttableTriangleLines[currentCuttableTriangleLineID].pointB.y);
			TwoDLineSegment cuttingSegment(in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA.x,
											in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointA.y,
											in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB.x,
											in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].pointB.y);

			std::cout << ":::: cuttableSegment points: A> " << cuttableSegment.a.x << ", " << cuttableSegment.a.y << " | B> " << cuttableSegment.b.x << ", " << cuttableSegment.b.y << std::endl;
			std::cout << ":::: cuttingSegment points: A> " << cuttingSegment.a.x << ", " << cuttingSegment.a.y << " | B> " << cuttingSegment.b.x << ", " << cuttingSegment.b.y << std::endl;

			TwoDLineSegmentIntersectAnalyzerV2 analyzerV2(cuttableSegment, cuttingSegment, PolyDebugLevel::NONE);
			if (analyzerV2.analyzedResult.intersectType == TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECT)
			{
				glm::vec3 convertedPoint = convert2DpointTo3D(analyzerV2.analyzedResult.intersectedPoint);
				// insert the ID of the cutting line, into the appropriate line in the cuttable triangle
				cuttableTriangleLines[currentCuttableTriangleLineID].cuttableIntersectionManager.insertRecord(currentCuttingTriangleLineID, convertedPoint);

				// insert the ID of the cuttable line, into the appropriate line in the cutting triangle
				in_cuttingTriangleRef->cuttingLines[currentCuttingTriangleLineID].cuttingIntersectionManager.insertRecord(currentCuttableTriangleLineID, convertedPoint);

				std::cout << "!!! Non-colinear intersection detected; cuttable ID is: " << currentCuttableTriangleLineID
					<< " | cutting ID is: " << currentCuttingTriangleLineID
					<< " | point is: " << analyzerV2.analyzedResult.intersectedPoint.x << ", " << analyzerV2.analyzedResult.intersectedPoint.y << std::endl;
				int outputVal = 3;
				std::cin >> outputVal;



			}
		}
	}

	// second, check for any slicing conditions that might have been met -- this would happen when any cutting line in the cutting triangle has 2 intersections
	// registered to it. Update the cuttableIntersetionManagers, by removing records that refer to the cutting line which had 2 lines.
	checkForSlicingCondition(in_cuttingTriangleRef);
}

void CuttableTriangle::checkForSlicingCondition(CuttingTriangle* in_cuttingTriangleRef)
{
	for (int x = 0; x < 3; x++)
	{
		if (in_cuttingTriangleRef->cuttingLines[x].cuttingIntersectionManager.numberOfRecords() == 2)
		{
			//std::cout << "!!! SLICE detected in cutting triangle line, line ID is: " << x << std::endl;
			//int sliceVal = 3;
			//std::cin >> sliceVal;

			auto beginIntersectionLineID = in_cuttingTriangleRef->cuttingLines[x].cuttingIntersectionManager.recordMap.begin();
			auto endIntersectionLineID = in_cuttingTriangleRef->cuttingLines[x].cuttingIntersectionManager.recordMap.rbegin();
			TwoDCrawlingAttempt slicingAttempt(TwoDCrawlingType::SLICE, x, beginIntersectionLineID->first, endIntersectionLineID->first);
			crawlingAttemptsVector.push_back(slicingAttempt);

			// remove corresponding entries from the cuttable triangle lines
			cuttableTriangleLines[beginIntersectionLineID->first].cuttableIntersectionManager.eraseRecord(x);
			cuttableTriangleLines[endIntersectionLineID->first].cuttableIntersectionManager.eraseRecord(x);

			//std::cout << "!! Size of cuttableIntersectionManager, post modification, for beginIntersectionLineID: " << cuttableTriangleLines[beginIntersectionLineID->first].cuttableIntersectionManager.numberOfRecords() << std::endl;
			//std::cout << "!! Size of cuttableIntersectionManager, post modification, for endIntersectionLineID: " << cuttableTriangleLines[endIntersectionLineID->first].cuttableIntersectionManager.numberOfRecords() << std::endl;
		}
	}
}

void CuttableTriangle::compareCuttableTriangleLineToCuttingTriangleLine(int in_cuttableIndex, CuttableTriangleLine* in_cuttableTriangleLineRef, int in_cuttingIndex, CuttingTriangleLine* in_cuttingTriangleLineRef)
{

}

void CuttableTriangle::printCuttableLineIntersections()
{
	for (int x = 0; x < 3; x++)
	{
		std::cout << "Data for line with index " << x << ": " << std::endl;
		cuttableTriangleLines[x].printCuttingIntersections();
	}
}

glm::vec3 CuttableTriangle::convert2DpointTo3D(TwoDPoint in_2dPoint)
{
	glm::vec3 returnVec;
	returnVec.x = in_2dPoint.x;
	returnVec.y = in_2dPoint.y;
	return returnVec;
}