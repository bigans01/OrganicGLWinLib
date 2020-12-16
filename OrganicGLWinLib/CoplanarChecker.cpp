#include "stdafx.h"
#include "CoplanarChecker.h"

void CoplanarChecker::performCoPlanarAnalysis()
{
	// load the coplanar comparison points for each STriangle we've gotten from each SPoly.
	STriangle* triangleARef = &polyAPtr->triangles[0];
	STriangle* triangleBRef = &polyBPtr->triangles[0];
	CoplanarComparisonCoandidatePoints triangleACandidates(triangleARef);
	CoplanarComparisonCoandidatePoints triangleBCandidates(triangleBRef);

	// compare candidates of A to candidates of B; if there's a match, remove the matched points from their respective positions in each candidate set.
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			if (triangleACandidates.points[x] == triangleBCandidates.points[y])
			{
				triangleACandidates.removeCandidate(x);
				triangleBCandidates.removeCandidate(y);
			}
		}
	}

	//std::cout << "++++++++++++ performing co planar analysis (1) " << std::endl;

	// get the appropriate points
	if
	(
		(triangleACandidates.availableCandidates.size() > 0)
		&&
		(triangleBCandidates.availableCandidates.size() > 0)
	)
	{
		// the first point we find in triangleACandidates must be compared to all remaining candidates of triangle B.
		glm::vec3 comparisonOriginPoint = triangleACandidates.getFirstAvailableCandidate();
		PointTranslationCheck pointCheck;
		pointCheck.performCheck(comparisonOriginPoint);
		auto triangleBCandidateSetBegin = triangleBCandidates.availableCandidates.begin();
		auto triangleBCandidateSetEnd = triangleBCandidates.availableCandidates.end();

		//std::cout << "++++++++++++ performing co planar analysis (2) " << std::endl;

		int requiredCoplanarityCount = int(triangleBCandidates.availableCandidates.size());
		//std::cout << "!! Required coplanarity count is: " << requiredCoplanarityCount << std::endl;

		int currentCoplanarityCount = 0;
		for (; triangleBCandidateSetBegin != triangleBCandidateSetEnd; triangleBCandidateSetBegin++)
		{
			glm::vec3 comparisonOriginPointCopy = comparisonOriginPoint;
			glm::vec3 currentComparedPointInTriangleB = triangleBCandidates.points[*triangleBCandidateSetBegin];

			//std::cout << "######## Comparison origin point copy: " << comparisonOriginPointCopy.x << ", " << comparisonOriginPointCopy.y << ", " << comparisonOriginPointCopy.z << std::endl;
			//std::cout << "######## Current compared point in Triangle b: " << currentComparedPointInTriangleB.x << ", " << currentComparedPointInTriangleB.y << ", " << currentComparedPointInTriangleB.z << std::endl;

			if (pointCheck.requiresTranslation == 1)
			{
				glm::vec3 translationValue = pointCheck.getTranslationValue();
				comparisonOriginPointCopy += translationValue;
				currentComparedPointInTriangleB += translationValue;
			}

			// add the two points to compare, plus the empty normal from SPoly A.
			glm::vec3 sPolyAEmptyNormal = polyAPtr->polyEmptyNormal;

			QuatRotationPoints rotationPoints;
			rotationPoints.pointsRefVector.push_back(&comparisonOriginPointCopy);
			rotationPoints.pointsRefVector.push_back(&currentComparedPointInTriangleB);		
			rotationPoints.pointsRefVector.push_back(&sPolyAEmptyNormal);					// used by quat rotation manager to get the normal to X = 1, 0, 0

			QuatRotationManager rotationManager;
			bool isComparisonCoplanar = rotationManager.initializeAndRunForCheckingCoplanarity(&rotationPoints);
			if (isComparisonCoplanar == true)
			{
				//std::cout << "!! Comparison is coplanar. " << std::endl;
				currentCoplanarityCount++;
			}

			if (currentCoplanarityCount == requiredCoplanarityCount)
			{
				coplanarityDetected = true;
				//std::cout << ">>>>>>>>>>>>>>> Poly A is Co-planar to Poly B! " << std::endl;
			}
		}
	}
}