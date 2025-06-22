#include "stdafx.h"
#include "TwoDLineSegmentJudge.h"
#include "QuatUtils.h"

void TwoDLineSegmentJudge::runJudgement()
{
	if (suspectResult.intersectType == TwoDLineSegmentIntersectType::NONCOLINEAR_INTERSECT)
	{
		// check if point B of the suspectLine is found within any of the STriangles of the tracked SPoly.
		// assumes we have rotated all involved STriangles to their 2D plane (so Z = 0).


		std::cout << "########- > Running for NONCOLINEAR_INTERSECT..." << std::endl;

		bool isWithinTrackedSPoly = false;
		auto sTrianglesBegin = trackedSPolyCopyRef->triangles.begin();
		auto sTrianglesEnd = trackedSPolyCopyRef->triangles.end();
		for (; sTrianglesBegin != sTrianglesEnd; sTrianglesBegin++)
		{
			glm::vec3 converted2D = OrganicGLWinUtils::convert2DToGlmVec3(suspectLine.b);
			if
				(


					QuatUtils::checkIfPointLiesWithinTrianglePBZ(converted2D,
						sTrianglesBegin->second.triangleLines[0].fetchPointAGlmVec3Version(),
						sTrianglesBegin->second.triangleLines[1].fetchPointAGlmVec3Version(),
						sTrianglesBegin->second.triangleLines[2].fetchPointAGlmVec3Version()
					)

					== true
				)
			{
				std::cout << "!!! ---> Point B of the suspectLine lies within the tracked SPoly!! (PARTIAL_BOUND categorized line type.)" << std::endl;
			}
		}
	}
}